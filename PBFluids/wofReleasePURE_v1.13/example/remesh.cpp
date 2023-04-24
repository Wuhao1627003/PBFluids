// Copyright (C) Geom Software e.U, Bernhard Kornberger, Graz/Austria
//
// This file is part of the WOF software. WOF is commercial software.
// Users holding a license may use this file in accordance with the
// License Agreement.
//
// This software is provided AS IS with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE.
//
// Please contact the author if any conditions of this licensing are
// not clear to you.
//
// Author: Bernhard Kornberger, bkorn (at) geom.at
// http://www.geom.at

#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "wof_api.h"

using namespace std;
using namespace GEOM_WOF;

void finalizeForRemeshed(MeshPtr pMesh); // postprocessing
double determineSpacingFromRange(std::vector<GEOM_WOF::Point3> vInCorners,double resolution); // resolution helper
void postProcForRemesh1(MeshPtr pMesh);
void postProcForRemesh2(MeshPtr pMesh);
int operationRemesh(char choice);



void infoRemesh()
{
	cout<<"\n\n\n\tHow do you want your mesh (postprocessing)"<<endl;
	cout<<"\t---------------------------------------------"<<endl<<endl;
	cout<<"\t1...Mesh simplification medium - strategy1"<<endl;
	cout<<"\t2...Mesh simplification strong - strategy2"<<endl;
	cout<<"\t3...No postprocessing"<<endl;
	cout<<"\tq...quit"<<endl;
}

int operationRemesh()
{
	// * 1 *   Choose a postprocessing strategy
	while(true)
	{
		infoRemesh();
		cout << "\n\n\tChoose a postprocessing strategy [1-3],q: ";
		char choice(0);
		cin>>choice;
		cout<<"\n\n\n"<<endl<<endl;
		switch(choice)
		{
			case '1':
			case '2':
			case '3':
				return operationRemesh(choice);
			case 'q':
				exit(0);
			default:
				continue;
		}
	}
}



// Create a point cloud from a mesh and reconstruct the mesh from the
// point cloud. This process is also known als 'melting': Small gaps
// are closed, parts inside a solid boundary are removed and self-
// intersections are removed. Melting degrades the visual appearance
// but can greatly reduce the complexity of the mesh.
int operationRemesh(char choice)
{
	// * 1 *   Read the input mesh, 3 corners per triangle
	std::string inFile("../../data/Music_Art_org.ply");
	GEOM_WOF::TimerC timerRead;
	std::vector<GEOM_WOF::Point3> vInCorners;
	switch(GEOM_WOF::getFileType(inFile))
	{
		case GEOM_WOF::FT_STL:
			GEOM_WOF::readSTL_ASCII(inFile.c_str(),vInCorners);
			break;
		case GEOM_WOF::FT_PLY:
			GEOM_WOF::readPly(inFile.c_str(),false,vInCorners);
			break;
		default:
			cerr<<"Unsupported input file format"<<endl;
			return 1;
	}
	cout<<"operationRemesh(): "<<endl;
	cout<<"  "<<inFile<<": "<<vInCorners.size()/3<<" triangles"<<endl;
	timerRead.stop();

	if(vInCorners.empty())
	{
		cout<<"No input points"<<endl;
		return 1;
	}

	// Choose the spacing value carefully (time and memory!)
	double resolution(200.0);
	double spacingAbs=determineSpacingFromRange(vInCorners,resolution); // Helper function below
	double featureAngle=15.0; // Edges with larger dihedral angles are feature edges, points are placed there

	// Melt the mesh
	GEOM_WOF::TimerC timerMelt;
	MeshPtr pMesh=GEOM_WOF::melt(vInCorners,spacingAbs,featureAngle);
	timerMelt.stop();

	// Postprocessing
	GEOM_WOF::TimerC timerPP;
	switch(choice)
	{
		case '1': postProcForRemesh1(pMesh);break;
		case '2': postProcForRemesh2(pMesh);break;
		default: break;
	}
	timerPP.stop();


	// Write the output file
	std::string outFilename("result_Music_Art_remeshed.ply");
	pMesh->printStatistics(outFilename);
	bool bUseASCII(true);
	GEOM_WOF::TimerC timerWrite;
	bool bWriteOK=writeMesh_auto(outFilename,pMesh,bUseASCII);
	if(!bWriteOK)
	{
		cout<<"Writing "<<outFilename<<" failed"<<endl;
	}
	timerWrite.stop();

	pMesh->printStatistics("Final mesh");

	// Show timing
	cout<<"\nTime:"<<endl;
	cout<<"  I/O Read: "<<timerRead.get()<<" s"<<endl;
	cout<<"  Remeshing:"<<timerMelt.get()<<" s"<<endl;
	cout<<"  Postproc: "<<timerPP.get()<<" s"<<endl;
	cout<<"  I/O Write:"<<timerWrite.get()<<" s"<<endl;
	cout<<endl<<endl;
	cout<<"DONE!"<<endl;
	if(bWriteOK) return 0; // Success
		else return 1;
}





void postProcForRemesh1(MeshPtr pMesh)
{
	// * 1 *   Edge flips
	edgeFlips(pMesh);

	// * 2 *   Simplification
	double maxAngleDev(1.0);
	double avgLen(pMesh->getAverageEdgeLength());
	reduce(pMesh,4*avgLen,maxAngleDev);

	// * 3 *   EdgeFlip-optimization
	edgeFlipsSimA(pMesh);
}


void postProcForRemesh2(MeshPtr pMesh)
{
	// * 1 *   Edge flips
	edgeFlips(pMesh);

	// * 2 *   Simplification
	double maxAngleDev(1.0);
	reduce(pMesh,DBL_MAX,maxAngleDev);

	// * 3 *   EdgeFlip-optimization
	edgeFlipsSimA(pMesh);
}



// Helper function to compute the absolute spacing value from the
// resolution and the bounding box of the object
double determineSpacingFromRange(std::vector<GEOM_WOF::Point3> vInCorners,double resolution)
{
	if(vInCorners.empty() || resolution <=0.0)
	{
		cerr<<"determineSpacingFromRange, invalid parameters"<<endl;
		return -1.0;
	}
	double minx,miny,minz,maxx,maxy,maxz;
	vInCorners[0].xyz(minx,miny,minz);
	vInCorners[0].xyz(maxx,maxy,maxz);
	double x,y,z;
	for(size_t i=1;i<vInCorners.size();++i)
	{
		GEOM_WOF::Point3& p(vInCorners[i]);
		p.xyz(x,y,z);
		if(x<minx) minx=x;
			else if(x>maxx) maxx=x;
		if(y<miny) miny=y;
			else if(y>maxy) maxy=y;
		if(z<minz) minz=z;
			else if(z>maxz) maxz=z;
	}
	double rangex(maxx-minx);
	double rangey(maxy-miny);
	double rangez(maxz-minz);
	double maxRange((std::max)({rangex,rangey,rangez}));
	double spacing(maxRange/resolution);

	cout<<"rangex="<<rangex<<", rangey="<<rangey<<", rangez="<<rangez<<endl;
	cout<<"Resolution="<<resolution<<", spacing="<<spacing<<endl;
	return spacing;
}

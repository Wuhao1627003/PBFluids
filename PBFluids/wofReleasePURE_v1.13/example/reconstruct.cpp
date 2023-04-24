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
// Operation examples
int operationReconstruct(char choice); // Reconstruct a mesh from a point cloud
void postProcStrategy_moderate(MeshPtr pMesh);
void postProcStrategy_max(MeshPtr pMesh);


void infoRec()
{
	cout<<"\n\n\n\tPostprocessing: How do you want your mesh?"<<endl;
	cout<<"\t------------------------------------------"<<endl;
	cout<<"\t1...Moderate mesh simplification and smoothing"<<endl;
	cout<<"\t2...Strong mesh simplification and smoothing"<<endl;
	cout<<"\t3...No postprocessing"<<endl;
	cout<<"\tq...quit"<<endl;
}

int operationReconstruct()
{
	// * 1 *   Choose a postprocessing strategy
	while(true)
	{
		infoRec();
		cout << "\n\n\tChoose a postprocessing strategy [1-3],q: ";
		char choice(0);
		cin>>choice;
		cout<<"\n\n\n"<<endl<<endl;
		switch(choice)
		{
			case '1':
			case '2':
			case '3':
				return operationReconstruct(choice);
			case 'q': exit(0);
			default: continue;
		}
	}

}



// Reconstruct a triangle mesh from a point cloud
int operationReconstruct(char choice)
{
	// * 1 *   Read the input point cloud
	std::string inFilename("../../data/Telecaster_cloud.ply"); // You can use *.ply, *.bin, (*.asc,*.txt,*.xyz)
	vector<GEOM_WOF::Point3> vPoints;
	GEOM_WOF::TimerC timerRead;
	bool bReadOK=GEOM_WOF::readPoints_auto(inFilename,vPoints);
	timerRead.stop();
	cout<<"Input points from ["<<inFilename<<"]: "<<vPoints.size()<<endl<<endl;

	if(!bReadOK || vPoints.empty())
	{
		cout<<"No points, stop"<<endl;
		return 1;
	}

	// * 2 *   Reconstruct
	GEOM_WOF::TimerC timerReco;
	bool bAutomaticDensity(true); // Choose true for automatic estimation or false to specify an absolute value
	bool bAllowSmoothing(true); // Choose true or false
	std::shared_ptr<GEOM_WOF::Mesh> pMesh;
	int numGrowSteps(20); // Hole closing: Up to 20 grow steps from the borders to close holes automatically
	if(bAutomaticDensity)
	{
		double spacingFactor(1.5); // Default is 2.0
		pMesh=GEOM_WOF::reconstruct_auto(vPoints,bAllowSmoothing,spacingFactor,numGrowSteps);
	}
	else
	{
		double estimatedSpacing(estimateAvgSpacing(vPoints));
		cout<<"estimated spacing in the cloud: "<<estimatedSpacing<<endl;
		double absoluteSpacing(2.0*estimatedSpacing); // Should be at least as large as the estimated spacing or larger to account for inhomogeneous sampling
		cout<<"used spacing: "<<absoluteSpacing<<endl;
		pMesh=GEOM_WOF::reconstruct_abs(vPoints,bAllowSmoothing,absoluteSpacing,numGrowSteps);
	}
	timerReco.stop();

	// * 3 *   Postprocessing
	GEOM_WOF::TimerC timerPP;
	switch(choice)
	{
		case '1': postProcStrategy_moderate(pMesh);break;
		case '2': postProcStrategy_max(pMesh);break;
		default: break;// No postprocessing
	}
	timerPP.stop();

	// * 3 *   Output
	bool bASCII(true); // Use ASCII format if both, binary and ASCII are available
	std::string outFilename("Telecaster_result.ply"); // Use *.ply, *.stl, *.list (STL is always ASCII, not binary!)
	GEOM_WOF::TimerC timerWrite;
	bool bWriteOK=GEOM_WOF::writeMesh_auto(outFilename,pMesh,bASCII);
	if(!bWriteOK)
	{
		cout<<"File "<<outFilename<<" not written!"<<endl;
	}
	timerWrite.stop();

	pMesh->printStatistics(outFilename);

	// * 4 *   Show the time consumption
	cout<<"\nTime:"<<endl;
	cout<<"  I/O Read: "<<timerRead.get()<<" s"<<endl;
	cout<<"  Meshing:  "<<timerReco.get()<<" s"<<endl;
	cout<<"  PostProc: "<<timerPP.get()<<" s"<<endl;
	cout<<"  I/O Write:"<<timerWrite.get()<<" s"<<endl;
	cout<<endl;

	if(bWriteOK) return 0; // Success
		else return 1;
}


void postProcStrategy_max(MeshPtr pMesh)
{
	// * 1 *   Edge flips (fits edges to the thought surface)
	edgeFlips(pMesh);

	// * 2 *   Laplacian smoothing, numIterations=2
	laplacianSmoothing(pMesh,3);

	// * 3 *   Simplification
	double simplifyLen(DBL_MAX); // longer edges are ignored
	double maxAngleDev(4.0); // maximum angle deviation (quality)
	reduce(pMesh,simplifyLen,maxAngleDev);

	// * 4 *   Edge flips (simulated annealing)
	edgeFlipsSimA(pMesh); // better but slower than edgeFlips()
}

void postProcStrategy_moderate(MeshPtr pMesh)
{
	// * 1 *   Edge flips (fits edges to the thought surface)
	edgeFlips(pMesh);

	// * 2 *   Laplacian smoothing, numIterations=2
	laplacianSmoothing(pMesh,2);

	// * 3 *   Simplification
	double avgLen(pMesh->getAverageEdgeLength());
	double simplifyLen(4*avgLen); // longer edges are ignored
	double maxAngleDev(4.0); // maximum angle deviation (quality)
	reduce(pMesh,simplifyLen,maxAngleDev);

	// * 4 *   Edge flips (simulated annealing)
	edgeFlipsSimA(pMesh); // better but slower than edgeFlips()

	// * 5 *   Simplification again to avoid ultra short edges
	reduce(pMesh,0.25*avgLen,1.5*maxAngleDev);

	// * 6 *   Laplacian smoothing again
	laplacianSmoothing(pMesh,1);
}

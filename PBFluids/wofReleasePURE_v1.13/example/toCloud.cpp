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



int operationToCloud()
{
	std::string inFile("S:/Maya2022/devkitBase/plugins/plugins/vertexPositions.xyz");

	// * 1 *   Read the triangle corners
	GEOM_WOF::TimerC timerRead;
	std::vector<GEOM_WOF::Point3> vInCorners;
	switch(GEOM_WOF::getFileType(inFile))
	{
		case GEOM_WOF::FT_XYZ:
			GEOM_WOF::readPoints_auto(inFile,vInCorners);
			break;
		case GEOM_WOF::FT_PLY:
			GEOM_WOF::readPly(inFile.c_str(),false,vInCorners);
		break;

		default:
			cerr<<"Unsupported input file format"<<endl;
			return 1;
	}
	cout<<"operationToCloud:"<<endl;
	cout<<"  "<<inFile<<": "<<vInCorners.size()/3<<" triangles"<<endl;
	timerRead.stop();

	if(vInCorners.empty())
	{
		cout<<"No input triangles"<<endl;
		return 1;
	}

	double spacingAbs(0.3); // Average spacing
	double featureThreshold(15.0); // Edges with larger dihedral angles are feature edges (points are placed there)

	// Get the cloud
	GEOM_WOF::TimerC timerToCloud;
		vector<GEOM_WOF::Point3> vPointsOut;
		toCloud(vInCorners,spacingAbs,featureThreshold,vPointsOut);
	timerToCloud.stop();


	// Write the output file
	std::string outFilename("result_Music_Art_cloud.xyz"); // You can also use *.ply or *.bin
	cout<<"  "<<outFilename<<": "<<vPointsOut.size()<<" points"<<endl;
	bool bUseASCII(true);
	GEOM_WOF::TimerC timerWrite;
	bool bWriteOK=writePoints_auto(outFilename,vPointsOut,bUseASCII);
	if(!bWriteOK)
	{
		cout<<"Writing "<<outFilename<<" failed"<<endl;
	}
	timerWrite.stop();

	// Show timing
	cout<<"\nTime:"<<endl;
	cout<<"  I/O Read: "<<timerRead.get()<<" s"<<endl;
	cout<<"  toCloud:  "<<timerToCloud.get()<<" s"<<endl;
	cout<<"  I/O Write:"<<timerWrite.get()<<" s"<<endl;
	cout<<endl<<endl;
	cout<<"DONE!"<<endl;

	if(bWriteOK) return 0; // Success
		else return 1;
}

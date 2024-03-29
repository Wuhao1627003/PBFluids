#pragma once

#include "Grid.h"
#include "Utils/sphere.h"
#include "wof_api.h"

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MFnMesh.h>

#include <maya/MPxNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MFnMeshData.h>
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>
#include <experimental/filesystem>

using namespace std;

#define McheckErr(stat,msg)			\
	if (MS::kSuccess != stat) {	    \
		cerr << msg;				\
		return MS::kFailure;		\
	}

const string objectNames[] = { "mass", "radius", "viscosity", "dt", "time", "numParticles", "width", "height", "container", "outputGeometry" };

class MPBFluids :
	public MPxNode
{
public:
	MPBFluids() {};
	virtual ~MPBFluids() override {};
	MStatus compute(const MPlug &plug, MDataBlock &data) override;
	static void *creator() { return new MPBFluids; }
	static MStatus initialize();
	
	// mass, radius, viscosity, dt, time, numParticles, width, height, container, outputGeometry;
	static MObject inputObjects[10];
	static MTypeId id;
	static string vxFilePath;
	Grid grid;
	static bool gridInitialized;
	static int lastTime;
};

#pragma once

#include "LSystemCmd.h"
#include "cylinder.h"

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

#include <maya/MIOStream.h>

#define McheckErr(stat,msg)			\
	if (MS::kSuccess != stat) {	    \
		cerr << msg;				\
		return MS::kFailure;		\
	}

class LSystemNode :
	public MPxNode
{
public:
	LSystemNode() {};
	virtual ~LSystemNode() override {};
	MStatus compute(const MPlug &plug, MDataBlock &data) override;
	static void *creator() { return new LSystemNode; }
	static MStatus initialize();

	static MObject defaultStepSize_, defaultAngle_, outputGeometry, grammarFile_, numIter_;
	static MTypeId id;
};


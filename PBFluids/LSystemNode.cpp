#include "LSystemNode.h"

#include <fstream>

MObject LSystemNode::defaultAngle_;
MObject LSystemNode::defaultStepSize_;
MObject LSystemNode::numIter_;
MObject LSystemNode::grammarFile_;
MObject LSystemNode::outputGeometry;
MTypeId LSystemNode::id(0x80000);

MStatus LSystemNode::initialize()
{
	MStatus returnStatus;

	MFnNumericAttribute defaultAngle_, defaultStepSize_;
	MFnTypedAttribute grammarFile_, outputGeometry;
	MFnUnitAttribute numIter_;

	LSystemNode::defaultAngle_ = defaultAngle_.create("defaultAngle", "da", MFnNumericData::kDouble, 1, &returnStatus);
	LSystemNode::defaultStepSize_ = defaultStepSize_.create("defaultStepSize", "ds", MFnNumericData::kDouble, 1, &returnStatus);
	LSystemNode::numIter_ = numIter_.create("numIter", "n", MFnUnitAttribute::Type::kTime, 0, &returnStatus);
	LSystemNode::grammarFile_ = grammarFile_.create("grammar", "g", MFnData::kString, MObject::kNullObj, &returnStatus);
	LSystemNode::outputGeometry = outputGeometry.create("outputGeometry", "o", MFnData::kMesh, MObject::kNullObj, &returnStatus);

	returnStatus = addAttribute(LSystemNode::defaultAngle_);
	McheckErr(returnStatus, "ERROR adding defaultAngle_ attribute\n");
	returnStatus = addAttribute(LSystemNode::defaultStepSize_);
	McheckErr(returnStatus, "ERROR adding defaultStepSize_ attribute\n");
	returnStatus = addAttribute(LSystemNode::numIter_);
	McheckErr(returnStatus, "ERROR adding numIter_ attribute\n");
	returnStatus = addAttribute(LSystemNode::grammarFile_);
	McheckErr(returnStatus, "ERROR adding grammar attribute\n");
	returnStatus = addAttribute(LSystemNode::outputGeometry);
	McheckErr(returnStatus, "ERROR adding outputGeometry attribute\n");

	returnStatus = attributeAffects(LSystemNode::defaultAngle_, LSystemNode::outputGeometry);
	McheckErr(returnStatus, "ERROR in defaultAngle_ attributeAffects\n");
	returnStatus = attributeAffects(LSystemNode::defaultStepSize_, LSystemNode::outputGeometry);
	McheckErr(returnStatus, "ERROR in defaultStepSize_ attributeAffects\n");
	returnStatus = attributeAffects(LSystemNode::numIter_, LSystemNode::outputGeometry);
	McheckErr(returnStatus, "ERROR in numIter_ attributeAffects\n");
	returnStatus = attributeAffects(LSystemNode::grammarFile_, LSystemNode::outputGeometry);
	McheckErr(returnStatus, "ERROR in grammar attributeAffects\n");

	return returnStatus;
}

MStatus LSystemNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus returnStatus;

	if (plug == outputGeometry)
	{
		// Input handles
		MDataHandle grammarFileHandle = data.inputValue(grammarFile_, &returnStatus);
		McheckErr(returnStatus, "Error getting grammarFile_ data handle\n");
		MString grammarFileString = grammarFileHandle.asString();
		if (grammarFileString == "")
		{
			return MS::kSuccess;
		}

		MDataHandle defaultAngleHandle = data.inputValue(defaultAngle_, &returnStatus);
		McheckErr(returnStatus, "Error getting defaultAngle_ data handle\n");
		double defaultAngle = defaultAngleHandle.asDouble();
		MDataHandle defaultStepSizeHandle = data.inputValue(defaultStepSize_, &returnStatus);
		McheckErr(returnStatus, "Error getting defaultStepSize_ data handle\n");
		double defaultStepSize = defaultStepSizeHandle.asDouble();
		MDataHandle numIterHandle = data.inputValue(numIter_, &returnStatus);
		McheckErr(returnStatus, "Error getting numIter_ data handle\n");
		int numIter = (int) numIterHandle.asTime().value();

		// Output handle
		MDataHandle outputGeometryHandle = data.outputValue(outputGeometry, &returnStatus);
		McheckErr(returnStatus, "ERROR getting geometry data handle\n");

		// Mesh manipulation
		MFnMeshData dataCreator;
		MObject newOutputData = dataCreator.create(&returnStatus);
		McheckErr(returnStatus, "ERROR creating outputData");

		// Get file path as string
		std::string filePath = grammarFileString.asChar();

		LSystem system = LSystem();
		system.loadProgram(filePath);
		system.setDefaultAngle((float)defaultAngle);
		system.setDefaultStep((float)defaultStepSize);

		std::vector<LSystem::Branch> branches;
		system.process(numIter, branches);
		std::vector<MPoint> worldPositions;

		for each (LSystem::Branch branch in branches)
		{
			MPoint curveStart = MPoint(branch.first[0], branch.first[1], branch.first[2]);
			worldPositions.push_back(curveStart);
		}

		if (worldPositions.size() > 0)
		{
			// Get start and end point of first cylinder (will serve as base of mesh)
			MPoint initialStart = MPoint(worldPositions[0].x, worldPositions[0].y, worldPositions[0].z);
			MPoint initialEnd = MPoint(worldPositions[1].x, worldPositions[1].y, worldPositions[1].z);
			CylinderMesh baseCylinder = CylinderMesh(initialStart, initialEnd, 1);

			// Get cylinder data
			MPointArray points;
			MIntArray faceCounts;
			MIntArray faceConnects;
			baseCylinder.getMesh(points, faceCounts, faceConnects);

			// Add rest of cylinders to mesh
			for (int i = 1; i < worldPositions.size() - 1; i++)
			{
				MPoint startPoint = worldPositions[i];
				MPoint endPoint = worldPositions[i + 1];
				CylinderMesh cylinder = CylinderMesh(startPoint, endPoint, 1);

				cylinder.appendToMesh(points, faceCounts, faceConnects);
			}

			// Create mesh
			MFnMesh	meshFS;
			meshFS.create(points.length(), faceCounts.length(),
						  points, faceCounts, faceConnects, newOutputData, &returnStatus);
			McheckErr(returnStatus, "ERROR creating new geometry");
		}

		// Sets output geometry data to newly processed data
		outputGeometryHandle.set(newOutputData);
		data.setClean(plug);
	}
	else
		returnStatus = MS::kUnknownParameter;

	return returnStatus;
}
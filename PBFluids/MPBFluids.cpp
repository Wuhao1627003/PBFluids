#include "MPBFluids.h"

#include <fstream>

MObject MPBFluids::radiusObj;
MObject MPBFluids::densityObj;
MObject MPBFluids::viscosityObj;
MObject MPBFluids::dtObj;
MObject MPBFluids::timeObj;
MObject MPBFluids::numParticlesObj;
MObject MPBFluids::widthObj;
MObject MPBFluids::heightObj;
MObject MPBFluids::outputGeometry;
MTypeId MPBFluids::id(0x80000);

MStatus MPBFluids::initialize()
{
	MStatus returnStatus;

	MFnNumericAttribute nattr;
	MFnTypedAttribute tattr;
	MFnUnitAttribute uattr;

	radiusObj = nattr.create("radius", "r", MFnNumericData::kDouble, 1, &returnStatus);
	densityObj = nattr.create("density", "ro", MFnNumericData::kDouble, 1, &returnStatus);
	viscosityObj = nattr.create("viscosity", "vs", MFnNumericData::kDouble, 1, &returnStatus);
	dtObj = nattr.create("deltaT", "dt", MFnNumericData::kDouble, 1, &returnStatus);
	timeObj = uattr.create("timeStep", "t", MFnUnitAttribute::Type::kTime, 0, &returnStatus);
	numParticlesObj = uattr.create("numParticles", "n", MFnUnitAttribute::Type::kLast, 0, &returnStatus);
	widthObj = uattr.create("width", "w", MFnUnitAttribute::Type::kLast, 0, &returnStatus);
	heightObj = uattr.create("height", "h", MFnUnitAttribute::Type::kLast, 0, &returnStatus);
	outputGeometry = tattr.create("outputGeometry", "o", MFnData::kMesh, MObject::kNullObj, &returnStatus);

	returnStatus = addAttribute(radiusObj);
	McheckErr(returnStatus, "ERROR adding radius attribute\n");
	returnStatus = addAttribute(densityObj);
	McheckErr(returnStatus, "ERROR adding density attribute\n");
	returnStatus = addAttribute(viscosityObj);
	McheckErr(returnStatus, "ERROR adding viscosity attribute\n");
	returnStatus = addAttribute(dtObj);
	McheckErr(returnStatus, "ERROR adding deltaT attribute\n");
	returnStatus = addAttribute(timeObj);
	McheckErr(returnStatus, "ERROR adding timeStep attribute\n");
	returnStatus = addAttribute(numParticlesObj);
	McheckErr(returnStatus, "ERROR adding numParticles attribute\n");
	returnStatus = addAttribute(widthObj);
	McheckErr(returnStatus, "ERROR adding width attribute\n");
	returnStatus = addAttribute(heightObj);
	McheckErr(returnStatus, "ERROR adding height attribute\n");
	returnStatus = addAttribute(outputGeometry);
	McheckErr(returnStatus, "ERROR adding outputGeometry attribute\n");

	returnStatus = attributeAffects(radiusObj, outputGeometry);
	McheckErr(returnStatus, "ERROR in radius attributeAffects\n");
	returnStatus = attributeAffects(densityObj, outputGeometry);
	McheckErr(returnStatus, "ERROR in density attributeAffects\n");
	returnStatus = attributeAffects(viscosityObj, outputGeometry);
	McheckErr(returnStatus, "ERROR in viscosity attributeAffects\n");
	returnStatus = attributeAffects(dtObj, outputGeometry);
	McheckErr(returnStatus, "ERROR in deltaT attributeAffects\n");
	returnStatus = attributeAffects(timeObj, outputGeometry);
	McheckErr(returnStatus, "ERROR in timeStep attributeAffects\n");
	returnStatus = attributeAffects(numParticlesObj, outputGeometry);
	McheckErr(returnStatus, "ERROR in numParticles attributeAffects\n");
	returnStatus = attributeAffects(widthObj, outputGeometry);
	McheckErr(returnStatus, "ERROR in width attributeAffects\n");
	returnStatus = attributeAffects(heightObj, outputGeometry);
	McheckErr(returnStatus, "ERROR in height attributeAffects\n");

	return returnStatus;
}

MStatus MPBFluids::compute(const MPlug &plug, MDataBlock &data)
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
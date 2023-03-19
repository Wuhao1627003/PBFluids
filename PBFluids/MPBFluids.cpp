#include "MPBFluids.h"

#include <fstream>

MObject MPBFluids::inputObjects[];
MTypeId MPBFluids::id(0x80000);

MStatus MPBFluids::initialize()
{
	MStatus returnStatus;

	MFnNumericAttribute nattr;
	MFnTypedAttribute tattr;
	MFnUnitAttribute uattr;

	inputObjects[0] = nattr.create("radius", "r", MFnNumericData::kDouble, 1, &returnStatus);
	inputObjects[1] = nattr.create("density", "ro", MFnNumericData::kDouble, 1, &returnStatus);
	inputObjects[2] = nattr.create("viscosity", "vs", MFnNumericData::kDouble, 1, &returnStatus);
	inputObjects[3] = nattr.create("deltaT", "dt", MFnNumericData::kDouble, 1, &returnStatus);
	inputObjects[4] = uattr.create("timeStep", "t", MFnUnitAttribute::Type::kTime, 0, &returnStatus);
	inputObjects[5] = uattr.create("numParticles", "n", MFnUnitAttribute::Type::kLast, 0, &returnStatus);
	inputObjects[6] = uattr.create("width", "w", MFnUnitAttribute::Type::kLast, 0, &returnStatus);
	inputObjects[7] = uattr.create("height", "h", MFnUnitAttribute::Type::kLast, 0, &returnStatus);
	inputObjects[8] = tattr.create("outputGeometry", "o", MFnData::kMesh, MObject::kNullObj, &returnStatus);

	for (size_t objCount = 0; objCount < 9; objCount ++) {
		returnStatus = addAttribute(inputObjects[objCount]);
		McheckErr(returnStatus, "ERROR adding " + objectNames[objCount] + " attribute\n");
	}

	for (size_t objCount = 0; objCount < 8; objCount++) {
		returnStatus = attributeAffects(inputObjects[objCount], inputObjects[8]);
		McheckErr(returnStatus, "ERROR adding " + objectNames[objCount] + " attributeAffects\n");
	}

	return returnStatus;
}

MStatus MPBFluids::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus returnStatus;

	if (plug == inputObjects[8])
	{
		int width, height, time;
		float density, dt, radius, viscosity;
		long numParticles;

		// Input handles
		MDataHandle dataHandle[9];
		for (size_t objCount = 0; objCount < 8; objCount++) {
			dataHandle[objCount] = data.inputValue(inputObjects[objCount], &returnStatus);
			McheckErr(returnStatus, "Error getting " + objectNames[objCount] + " data handle\n");
		}

		// radius, density, viscosity, dt, time, numParticles, width, height, outputGeometry;
		radius = dataHandle[0].asFloat();
		density = dataHandle[1].asFloat();
		viscosity = dataHandle[2].asFloat();
		dt  = dataHandle[3].asFloat();
		time = dataHandle[4].asInt();
		numParticles = (long) (dataHandle[5].asInt64());
		width = dataHandle[6].asInt();
		height = dataHandle[7].asInt();

		// Output handle
		dataHandle[8] = data.outputValue(inputObjects[8], &returnStatus);
		McheckErr(returnStatus, "ERROR getting geometry data handle\n");

		// Mesh manipulation
		MFnMeshData dataCreator;
		MObject newOutputData = dataCreator.create(&returnStatus);
		McheckErr(returnStatus, "ERROR creating outputData");

		Grid grid = Grid(width, height, density, viscosity, numParticles, dt, radius);
		for (int t = 0; t < time; t++) {
			grid.step();
		}

		std::vector<MPoint> worldPositions;
		for (Particle p : grid.particles) {
			MPoint particlePos = MPoint(p.pos[0], p.pos[0], p.pos[2]);
			worldPositions.push_back(particlePos);
		}

		SphereMesh baseSphere = SphereMesh(worldPositions[0], radius);

		if (worldPositions.size() > 0) {
			// Get start point of first sphere (will serve as base of mesh)
			MPoint initialStart = MPoint(worldPositions[0].x, worldPositions[0].y, worldPositions[0].z);
			SphereMesh baseSphere = SphereMesh(initialStart, radius);

			// Get sphere data
			MPointArray points;
			MIntArray faceCounts;
			MIntArray faceConnects;
			baseSphere.getMesh(points, faceCounts, faceConnects);

			// Add rest of spheres to mesh
			for (MPoint point : worldPositions) {
				SphereMesh sphere = SphereMesh(point, radius);
				sphere.appendToMesh(points, faceCounts, faceConnects);
			}

			// Create mesh
			MFnMesh	meshFS;
			meshFS.create(points.length(), faceCounts.length(),
						  points, faceCounts, faceConnects, newOutputData, &returnStatus);
			McheckErr(returnStatus, "ERROR creating new geometry");
		}

		// Sets output geometry data to newly processed data
		dataHandle[8].set(newOutputData);
		data.setClean(plug);
	}
	else
		returnStatus = MS::kUnknownParameter;

	return returnStatus;
}
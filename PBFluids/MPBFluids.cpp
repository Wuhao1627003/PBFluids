#include "MPBFluids.h"

#include <fstream>

MObject MPBFluids::inputObjects[10];
MTypeId MPBFluids::id(0x80080);
bool MPBFluids::gridInitialized(false);
int MPBFluids::lastTime(0);

MString MPointToMString(MPoint p)
{
	return MString((std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "\n").c_str());
}

MStatus MPBFluids::initialize()
{
	MGlobal::displayInfo("Start init");
	MStatus returnStatus;

	MFnNumericAttribute nattr;
	MFnTypedAttribute tattr;
	MFnUnitAttribute uattr;

	inputObjects[0] = nattr.create("radius", "r", MFnNumericData::kDouble, 1.0, &returnStatus);
	inputObjects[1] = nattr.create("density", "ro", MFnNumericData::kDouble, 1.0, &returnStatus);
	inputObjects[2] = nattr.create("viscosity", "vs", MFnNumericData::kDouble, 1.0, &returnStatus);
	inputObjects[3] = nattr.create("deltaT", "dt", MFnNumericData::kDouble, 1.0, &returnStatus);

	inputObjects[4] = uattr.create("timeStep", "t", MFnUnitAttribute::Type::kTime, 0, &returnStatus);
	inputObjects[5] = uattr.create("numParticles", "n", MFnUnitAttribute::Type::kTime, 0, &returnStatus);
	inputObjects[6] = uattr.create("width", "w", MFnUnitAttribute::Type::kTime, 0, &returnStatus);
	inputObjects[7] = uattr.create("height", "h", MFnUnitAttribute::Type::kTime, 0, &returnStatus);

	inputObjects[8] = tattr.create("container", "c", MFnData::kMesh, MObject::kNullObj, &returnStatus);

	inputObjects[9] = tattr.create("outputGeometry", "o", MFnData::kMesh, MObject::kNullObj, &returnStatus);

	for (size_t objCount = 0; objCount < 10; objCount ++) {
		returnStatus = addAttribute(inputObjects[objCount]);
		McheckErr(returnStatus, "ERROR adding " + objectNames[objCount] + " attribute\n");
	}

	for (size_t objCount = 0; objCount < 9; objCount++) {
		returnStatus = attributeAffects(inputObjects[objCount], inputObjects[9]);
		McheckErr(returnStatus, "ERROR adding " + objectNames[objCount] + " attributeAffects\n");
	}
	gridInitialized = false;
	lastTime = 0;
	MGlobal::displayInfo("Finish init");
	return returnStatus;
}

MStatus MPBFluids::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus returnStatus;

	if (plug == inputObjects[9])
	{
		int width, height, time;
		float density, dt, radius, viscosity;
		long numParticles;

		// Input handles
		MDataHandle dataHandle[10];
		for (size_t objCount = 0; objCount < 9; objCount++) {
			dataHandle[objCount] = data.inputValue(inputObjects[objCount], &returnStatus);
			McheckErr(returnStatus, "Error getting " + objectNames[objCount] + " data handle\n");
		}

		// radius, density, viscosity, dt, time, numParticles, width, height, outputGeometry;
		radius = (float) dataHandle[0].asDouble();
		density = (float) dataHandle[1].asDouble();
		viscosity = (float) dataHandle[2].asDouble();
		dt = (float) dataHandle[3].asDouble();
		time = dataHandle[4].asTime().value();
		numParticles = (long) (dataHandle[5].asTime().value());
		width = dataHandle[6].asTime().value();
		height = dataHandle[7].asTime().value();
		MObject containerObject = dataHandle[8].asMesh();
		MFnMesh containerMesh(containerObject, &returnStatus);

		// Output handle
		dataHandle[9] = data.outputValue(inputObjects[9], &returnStatus);
		McheckErr(returnStatus, "ERROR getting geometry data handle\n");

		// Mesh manipulation
		MFnMeshData dataCreator;
		MObject newOutputData = dataCreator.create(&returnStatus);
		McheckErr(returnStatus, "ERROR creating outputData");

		if (!gridInitialized || time == lastTime) {
			this->grid = Grid(width, height, density, viscosity, numParticles, dt, radius);

			// TODO: process container mesh
			vector<vec3> containerTriangles;
			this->grid.addContainer(containerTriangles);

			gridInitialized = true;
		}
		if (lastTime < time) {
			for (int t = lastTime; t < time; t++) {
				grid.step();
			}
		}
		else {
			this->grid = Grid(width, height, density, viscosity, numParticles, dt, radius);
			for (int t = 0; t < time; t++) {
				grid.step();
			}
		}
		lastTime = time;

		std::vector<MPoint> worldPositions;
		for (Particle p : grid.particles) {
			MPoint particlePos = MPoint(p.pos[0], p.pos[2], p.pos[1]);
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
			for (size_t i = 1; i < worldPositions.size(); i++) {
				SphereMesh sphere = SphereMesh(worldPositions[i], radius);
				sphere.appendToMesh(points, faceCounts, faceConnects);
			}

			// Create mesh
			MFnMesh	meshFS;
			meshFS.create(points.length(), faceCounts.length(),
						  points, faceCounts, faceConnects, newOutputData, &returnStatus);
			McheckErr(returnStatus, "ERROR creating new geometry");
		}

		// Sets output geometry data to newly processed data
		dataHandle[9].set(newOutputData);
		data.setClean(plug);
	}
	else
		returnStatus = MS::kUnknownParameter;

	return returnStatus;
}
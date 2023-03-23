#include "MPBFluids.h"
#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus   status = MStatus::kSuccess;
	MFnPlugin plugin(obj, "MPBFluids", "1.0", "Any");

	// Register node
	status = plugin.registerNode("MPBFluids", MPBFluids::id, MPBFluids::creator, MPBFluids::initialize);
	if (!status)
	{
		status.perror("registerNode");
		return status;
	}

	// Auto-register Mel menu script
	char buffer[2048];
	MString pluginPath = plugin.loadPath() + MString("/") + MString("PBFluidsMel.mel\"");
	MString menuPath = MString("source \"") + pluginPath;
	sprintf_s(buffer, 2048, menuPath.asChar());
	MGlobal::executeCommand(buffer, true);

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus   status = MStatus::kSuccess;
	MFnPlugin plugin(obj);

	status = plugin.deregisterNode(MPBFluids::id);
	if (!status)
	{
		status.perror("deregisterNode");
		return status;
	}

	return status;
}



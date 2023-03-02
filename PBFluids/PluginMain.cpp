#include "LSystemNode.h"

#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus   status = MStatus::kSuccess;
	MFnPlugin plugin(obj, "MyPlugin", "1.0", "Any");

	// Register Command
	status = plugin.registerCommand("LSystemCmd", LSystemCmd::creator);
	if (!status)
	{
		status.perror("registerCommand");
		return status;
	}

	// Register node
	status = plugin.registerNode("LSystemNode", LSystemNode::id, LSystemNode::creator, LSystemNode::initialize);
	if (!status)
	{
		status.perror("registerNode");
		return status;
	}

	// Auto-register Mel menu script
	char buffer[2048];
	MString pluginPath = plugin.loadPath() + MString("/") + MString("Register.mel\"");
	MString menuPath = MString("source \"") + pluginPath;
	sprintf_s(buffer, 2048, menuPath.asChar());
	MGlobal::executeCommand(buffer, true);

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus   status = MStatus::kSuccess;
	MFnPlugin plugin(obj);

	status = plugin.deregisterCommand("LSystemCmd");
	if (!status)
	{
		status.perror("deregisterCommand");
		return status;
	}

	status = plugin.deregisterNode(LSystemNode::id);
	if (!status)
	{
		status.perror("deregisterNode");
		return status;
	}

	return status;
}



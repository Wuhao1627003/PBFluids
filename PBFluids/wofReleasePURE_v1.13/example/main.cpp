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

// Operation examples
int operationReconstruct(); // Reconstruct a mesh from a point cloud (in reconstruct.cpp)
int operationToCloud(); // Mesh-to-Cloud (in toCloud.cpp)
int operationRemesh(); // Mesh-to-Cloud-to-Mesh (in remesh.cpp)

void info();      // Menu
bool greeting();  // License check

// An example progress bar derived from ProgressBase
class TerminalProgress:public GEOM_WOF::ProgressBase
{
public:
	// WOF calls this method with d={0.0,...,1.0}
	void update(const std::string& s,double d)
	{
		if(s!=lastMessage)
		{
			cout<<"\n"; // New message, line feed
			lastMessage=s;
		}
		cout<<("TerminalProgress (\""+s+"\"): ")<<d*100.0<<" %                                \r"<<flush;
		if(d>=1.0) cout<<endl; // 100 %, line feed
	}
protected:
	std::string lastMessage;
};


int main()
{
	// Check and print the license
	bool bOK=greeting();
	if(!bOK) return 80;

	// Connect your own progress bar
	TerminalProgress myProgress; // Note: No early delete of this object!
	GEOM_WOF::subscribe(&myProgress);

	// Choose an example
	try
	{
		while(true)
		{
			info();
			char choice(0);
			cout << "\n\n\tChoose an example [a-c],q: ";
			cin>>choice;
			cout<<"\n\n\n"<<endl<<endl;
			switch(choice)
			{
					case 'a': return operationReconstruct();
					case 'b': return operationToCloud();
					case 'c': return operationRemesh();
					case 'q':
					default:
							return 0;
			}
		}
	}
	catch(...)
	{
		 cerr<<"Unexpected end"<<endl;
		 return 1;
	}
}



void info()
{
	cout<<"\n\n\n\tWelcome to the WOF examples"<<endl;
	cout<<"\t---------------------------"<<endl;
	cout<<"\ta...Reconstruct a mesh from a point cloud"<<endl;
	cout<<"\tb...Convert a mesh to a point cloud"<<endl;
	cout<<"\tc...Convert a mesh to a point cloud and reconstruct the mesh (Mesh Melting)"<<endl;
	cout<<"\tq...quit"<<endl;
}


///////////////////////////////////////////////////
// Version and license-related example functions //
// (you don't need them for your tests)          //
///////////////////////////////////////////////////

// Check the license
bool greeting()
{
	// Print version and license information
	GEOM_WOF::printLicense();

	// Which license do we have?
	switch(GEOM_WOF::getLicenseState())
	{
		case WOFLIC_ACTIVATED:
			cout<<", License: active"<<endl;
			return true;
		case WOFLIC_TRIAL:
			cout<<endl; // Remaining trial days are printed
			return true;
		case WOFLIC_GRACE_OK:
			cout<<", License: No internet connection (grace period)"<<endl;
			return true;
		case WOFLIC_GRACE_EXPIRED:
			cout<<"License: No internet connection, grace period expired, re-verification required"<<endl;
			return false;
		case WOFLIC_PURE:
			cout<<"License: PURE"<<endl;
			return true;
		case WOFLIC_INVALID:
		default:
			cout<<"License: None"<<endl;
			return false;
	}
}


// System-wide activation requires admin priviledges. Note: Activation
// shall only be made once. Then the software is activated and another
// call to this method will fail. You can check the licensing state
// with GEOM_WOF::getLicenseState().
int activate_system_wide()
{
	const char* yourKey("ABCD EFGH ...");
	if(GEOM_WOF::activateWof(yourKey,true))
	{
		return 0; // Success
	}
	else
	{
		return 80; // License error
	}
}

// Activation for the current user. Note: Activation shall only be made
// once. Then the software is activated and another call to this method
// will fail. You can check the licensing state with
// GEOM_WOF::getLicenseState().
int activate_user_only()
{
	const char* yourKey("ABCD EFGH ...");
	if(GEOM_WOF::activateWof(yourKey,false))
	{
		return 0; // Success
	}
	else
	{
		return 80; // License error
	}
}

// Deactivate so that the key can be used for another machine. Do this
// only when an old computer is replaced. The number of deactivations
// is limited.
int deactivate()
{
	if(GEOM_WOF::deactivateWof())
	{
		return 0; // Success
	}
	else
	{
		return 80; // License error
	}
}

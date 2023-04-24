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
/// @file wof_api_definitions.h
#pragma once

namespace GEOM_WOF {


// VERSION NUMBERS
#define WOF_VER_MAJOR 1
#define WOF_VER_MINOR 13


// DLL IMPORT/EXPORT MACRO
#if defined (_WIN32)
	// ** WINDOWS **
	#ifdef WOF_STATIC
		// Static
		#define CLASS_DECLSPEC
	#else
		// DLL
		#if defined(WOF_EXPORT)
			#define  CLASS_DECLSPEC __declspec(dllexport)
		#else
			#define  CLASS_DECLSPEC __declspec(dllimport)
		#endif
	#endif
#else
	// ** LINUX, MAC and other **
	#define CLASS_DECLSPEC
#endif

/** \brief License-Exception
 *
 * The WofLicenseException is thrown in case of an invalid license
 * state. If your trial has expired but you still need the software for
 * your non-commercial research, contact Geom Software: bkorn@geom.at
 * for a trial extension code. Include information about your project.
*/
class WofLicenseException: public std::exception
{
	virtual const char* what() const throw()
	{
		return "WOF License Exception - no valid license";
	}
};


/** \brief Bug-Exception
 *
 * The WofBugException is thrown in case of unexpected states caused by
 * invalid input <b>OR</b> a bug. Software quality is very important for
 * this project. If you find a bug, send a bug report and it will be
 * processed immediately.
*/
struct WofBugException: public std::exception
{
	virtual const char* what() const throw()
	{
		return "WOF Exception - Bug *OR* wrong usage. Report a bug and it will be fixed quickly: bkorn@geom.at";
	}
};


/**  \brief Base class for progress subscribers
*
* A progress subscriber class can be derived from ProgressBase to
* receive progress updates from the WOF library.
*
* A simple terminal progress bar could be derived like this:
* @code
*
* class MyProgressBar:public GEOM_WOF::ProgressBase
* {
* public:
* 	// WOF calls the update method with d={0.0,...,1.0}
* 	void update(const std::string& s,double d)
* 	{
* 		if(s!=lastMessage)
* 		{
* 			cout<<"\n"; // New message, line feed
* 			lastMessage=s;
* 		}
* 		cout<<("Progress(\""+s+"\"): ")<<d*100.0<<" %                                \r"<<flush;
* 		if(d>=1.0) cout<<endl;
* 	}
* protected:
* 	std::string lastMessage;
* };
* @endcode
*/

class CLASS_DECLSPEC ProgressBase
{
public:
	ProgressBase(){};
	virtual ~ProgressBase(){}

/** \brief update
*
* This method must be defined in the derived class. It is called
* whenever the progress changes and thus it should be computationally
* inexpensive.
*/
	virtual void update(const std::string& s,double d)=0;
};


}

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
/// @file wof_api.h
#pragma once
#include <vector>
#ifndef WOF_EXPORT
#include "License.h"
#endif

#include "wof_api_io.h"
#include "wof_api_functions.h"
#include "wof_api_definitions.h"
#include "TimerC.h"

namespace GEOM_WOF {

///////////////////////
// License functions //
///////////////////////
/** \brief License related functions for "LM" builds
*
* Functions related to the license: Activation, deactivation,
* trial-extension... Not active in "PURE" builds.
*/
/** \defgroup licRelated License related functions
 *  @{
 */
/** WOFLIC_ACTIVATED means the software is activated */
#define WOFLIC_ACTIVATED 10
/** WOFLIC_GRACE_OK means the software is activated but re-verification has failed (no internet, valid for a sufficiently long grace period) */
#define WOFLIC_GRACE_OK 11
/** WOFLIC_GRACE_EXPIRED means the software is activated but re-verification (no internet) has failed for a long time. Invalid. */
#define WOFLIC_GRACE_EXPIRED 12
/** WOFLIC_TRIAL means the trial period is still active */
#define WOFLIC_TRIAL 13
/** WOFLIC_INVALID means there is no valid license (trial, product-key) */
#define WOFLIC_INVALID 14
/** WOFLIC_PURE is the perpetual non-commercial license */
#define WOFLIC_PURE 15

/** \brief Check the license state
*
* @return \ref WOFLIC_ACTIVATED when the software is activated
* @return \ref WOFLIC_TRIAL during the trial period
* @return \ref WOFLIC_GRACE_OK when activated but verification has failed (no internet access) which is valid for a grace period
* @return \ref WOFLIC_GRACE_EXPIRED when activated but verification has failed (no internet) for a long time
* @return \ref WOFLIC_INVALID otherwise (trial expired, no license)
*/
CLASS_DECLSPEC
int getLicenseState();

/** \brief Print license details
 */
CLASS_DECLSPEC
void printLicense();

/** \brief Check if pure or lm build
 */
CLASS_DECLSPEC
bool isPure();


/** \brief Activate WOF license
 *
 * This function is used when you have a WOF license key. You
 * can choose to activate system-wide or only for the current user.
 *
 * @param key is the purchased software key
 * @param bSystemWide When true then the activation data is stored
 * system-wide. When false the activation is made for the current user.
 *
 * @note Activation is only done once. You can use getLicenseState()
 * to find out if the software is already activated.
 * @note When the system-wide activation is chosen (\p bSystemWide=true)
 * then the application needs admin-priviledges.
 */
CLASS_DECLSPEC
bool activateWof(const char* key,bool bSystemWide);


/** \brief Deactivate WOF license
 *
 * Deactivates the WOF license on the present computer so that the
 * key can be used on another machine. This function enables you
 * to replace a computer. Do not use over-frequently, the number
 * of deactivations is limited, it's not a floating license.
 *
 * @return true in case of success, false otherwise
 */
CLASS_DECLSPEC
bool deactivateWof();


/** \brief Extend Trial
 *
 * @param key is a Trial-Extension-key. You ask for such a key to extend the
 * trial period for your non-commercial research project (see the
 * guidelines) or for an extended commercial test periord.
 *
 * @return true in case of success, false otherwise
 */
CLASS_DECLSPEC
bool extendTrial(const char* key);

/** @}*/




///////////////////////
// Version functions //
///////////////////////
//
/** \brief Version related functions
 *
 * Functions to identify Debug- and Release builds and to fetch the version number.
 */

/** \defgroup verRelated Version related functions
 *  @{
 */

/** \brief Print version number
 *
 * Prints the version number to stdout
 */
CLASS_DECLSPEC
void printVersion();



/** \brief Get version numbers
 *
 * Returns the WOF version number
 *
 * @param versionMajor,versionMinor [out] are used to return the major and minor version number
*/
CLASS_DECLSPEC
void getVersion(int& versionMajor,int& versionMinor);



/** \brief Check if the present binary is a Release or Debug build
 *
 * @return true when the library has been compiled in release mode
 * or false otherwise
 *
 *
*/
CLASS_DECLSPEC
bool isRelease();

/** @}*/





} // Namespace

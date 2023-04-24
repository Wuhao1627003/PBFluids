
// (c) 2010 Geom Software Bernhard Kornberger, Graz/Austria. All rights reserved.
//
// This file is part of the WOF library. You can use it for your personal
// non-commercial, non-military research. Don't redistribute without explicit
// permission. 

// This software is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
// THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Please contact the author if any conditions of this licensing are not clear 
// to you.
// 
// Geom Software e.U.
// Dipl.-Ing. Dr. Bernhard Kornberger
// Raiffeisenstrasse 19
// A-8010 Graz / Austria
// bkorn@geom.at 
// https://www.geom.at/wof-point-cloud-mesher-doc/html/

//
// * Please remember to put a link to WOF on your research website *
//


#pragma once 
#include "wof_api_functions.h"

namespace{

struct License
{
	License()
	{
		GEOM_WOF::setLic(
		"[NonCommercialResearch]",
		"[LicType,student],[reco,5e5],[toCloud,5e5],[remesh,5e5]",
		"",
		"5303992d",
		"62366c4");
	}
};
License lic;
}

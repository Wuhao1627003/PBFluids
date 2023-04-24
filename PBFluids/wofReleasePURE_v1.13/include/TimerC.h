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

#pragma once
#include <chrono>
#include <string>
#include <iostream>
namespace GEOM_WOF {

/** \brief Timer class
 *
 * TimerC measures the time consumption
 */
class TimerC
{
public:
	/** \brief Constructor
	 *
	 * At construction TimerC stores the current time
	 */
	TimerC():
		t0(std::chrono::high_resolution_clock::now()),
		elapsedSeconds(0.0),
		bStopped(false)
	{}
	/** \brief Timer stop
	 *
	 * @return the elapsed time since TimerC construction in seconds
	 */
    double stop()
    {
		bStopped=true;
		elapsedSeconds = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();
		return elapsedSeconds;
	}
	/** \brief Get the elapsed time
	 *
	 * @return the elapsed time in seconds between TimerC construction
	 * and the first call to TimerC::stop(). When the timer has not
	 * been stopped then the time since construction is returned.
	 */
	double get() const
	{
		if(bStopped)
		{
			return elapsedSeconds;
		}
		else
		{
			return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();
		}
	}
	/** \brief Report
	 *
	 * Prints the time since construction or since last report. This
	 * command is intended to measure successive intervals
	 */
	void report(const std::string& s)
	{
		double elapsed(std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count());
		t0=std::chrono::high_resolution_clock::now();
		std::cout<<"\nReport ["<<s<<"]: "<<elapsed<<" s"<<std::endl;
	}
private:
    std::chrono::high_resolution_clock::time_point t0;
    double elapsedSeconds;
    bool bStopped;
};




} // Namespace

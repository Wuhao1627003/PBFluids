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

#include <math.h>
#include "Vector3.h"
#include <sstream>
namespace GEOM_WOF {


class Triangle2; // FWD

/** \brief 3D Point
*/
class CLASS_DECLSPEC Point3
{
public:

/** \brief Constructor without color
*
* @param x_,y_,z_ [in] coordinates
*/
Point3(const double x_,const double y_,const double z_):
	coordX(x_),
	coordY(y_),
	coordZ(z_),
	r(0),g(0),b(0),a(0),bHasColor(false)
{
}

/** \brief Constructor with color
*
* @param x_,y_,z_ [in] coordinates
* @param r_,g_,b_,a_ [in] color
*/
Point3(const double x_,const double y_,const double z_,unsigned  char r_,unsigned  char g_,unsigned  char b_,unsigned  char a_):
	coordX(x_),
	coordY(y_),
	coordZ(z_),
	r(r_),
	g(g_),
	b(b_),
	a(a_),
	bHasColor(true)
{
}


/** \brief Default constructor
*
* Coordinates are initialized to 0.
*/
Point3():coordX(0),coordY(0),coordZ(0),r(0),g(0),b(0),a(0),bHasColor(false)
{
}


/** \brief Copy constructor
*
* Copies the coordinates of \p p_
*/
Point3(const Point3& p_):
	coordX(p_.x()),
	coordY(p_.y()),
	coordZ(p_.z()),
	r(p_.r),g(p_.g),b(p_.b),a(p_.a),bHasColor(p_.bHasColor)
{
}

/** \brief operator=
 *
 * Assigns \p other
 */
Point3 &operator=(const Point3& other)
{
	coordX=other.x();
	coordY=other.y();
	coordZ=other.z();
	r=other.r;
	g=other.g;
	b=other.b;
	a=other.a;
	bHasColor=other.bHasColor;
	return *this;
}

/** \brief Destructor
 */
~Point3()
{
}

/** \brief Set color
*
* @param r_,g_,b_,a_ are red, green, blue, alpha in the range 0-255
*/
void setColor(unsigned  char r_,unsigned  char g_,unsigned  char b_,unsigned  char a_)
{
	r=r_;
	g=g_;
	b=b_;
	a=a_;
	bHasColor=true;
}



/** \brief Get color (unsigned char, range=0, ..., 255)
*
* @param r_,g_,b_,a_ are red, green, blue, alpha
*/
void getColor_255(unsigned char& r_,unsigned char& g_,unsigned char& b_,unsigned char& a_) const
{
	r_=r;
	g_=g;
	b_=b;
	a_=a;
}


/** \brief Get color (float, range=0, ..., 255)
*
* @param r_,g_,b_,a_ are red, green, blue, alpha
*/
void getColor_255(float& r_,float& g_,float& b_,float& a_) const
{
	r_=static_cast<float>(r);
	g_=static_cast<float>(g);
	b_=static_cast<float>(b);
	a_=static_cast<float>(a);
}

/** \brief Get color string (string, range=0.0, ..., 1.0)
*
* @param r_,g_,b_,a_ are red, green, blue, alpha
*/
std::string getColorString()
{
	float r,g,b,a;
	getColor_1(r,g,b,a);
	std::stringstream ss;
	ss<<r<<" "<<g<<" "<<b<<" "<<a;
	return ss.str();
}

/** \brief Get color (float, range=0.0, ..., 1.0)
*
* @param r_,g_,b_,a_ are red, green, blue, alpha
*/
void getColor_1(float& r_,float& g_,float& b_,float& a_) const
{
	r_=r/255.0f;
	g_=g/255.0f;
	b_=b/255.0f;
	a_=a/255.0f;
}
/** \brief Has color
 *
 * Returns whether a color has been assigned to the point
*/
bool hasColor() const
{
	return bHasColor;
}

/** \brief Get the x-coordinate
*
* @return the x-coordinate
*/
double x() const
{
	return coordX;
}


/** \brief Get the y-coordinate
*
* @return the y-coordinate
*/
double y() const
{
	return coordY;
}


/** \brief Get the z-coordinate.
*
* @return the z-coordinate
*/
double z() const
{
	return coordZ;
}


/** \brief Get the x-, y- and z-coordinate
*
* @param x_,y_,z_ [out] x,y,z-coordinates
*
* @returns all 3 coordinates at once
*
*/
void xyz(double& x_,double& y_,double& z_) const
{
	x_=coordX;
	y_=coordY;
	z_=coordZ;
}



/** \brief Add the point's coordinates to x,y,z
*
* @param x,y,z [inout] are used to accumulate the point's coordinates
*/
inline void addOwnCoords(double& x,double& y,double& z) const
{
	x+=coordX;
	y+=coordY;
	z+=coordZ;
}

/** \private*/
// Internal use: add the point's color to r_,g_,b_,a_
inline void addOwnColor(float& r_,float& g_,float& b_,float& a_) const
{
	r_+=float(r);
	g_+=float(g);
	b_+=float(b);
	a_+=float(a);
}


/** \brief Copy the color from another point
*
* @param other [in] is the point whose color is copied
*/
inline void copyColorFrom(const Point3& other)
{
	r=other.r;
	g=other.g;
	b=other.b;
	a=other.a;
	bHasColor=other.bHasColor;
}


/** \brief Copy the average color from two points
*
* @param other0,other1 [in] are the points whose color is averaged
*/
inline void copyColorFrom(const Point3& other0,const Point3& other1)
{
	if(other0.hasColor() && other1.hasColor())
	{
		r=static_cast<unsigned char>(.5*(float(other0.r)+float(other1.r))+.5f);
		g=static_cast<unsigned char>(.5*(float(other0.g)+float(other1.g))+.5f);
		b=static_cast<unsigned char>(.5*(float(other0.b)+float(other1.b))+.5f);
		a=static_cast<unsigned char>(.5*(float(other0.a+other1.a))+.5f);
		bHasColor=true;
		return;
	}
	if(other0.hasColor())
	{
		copyColorFrom(other0);
		return;
	}
	if(other1.hasColor())
	{
		copyColorFrom(other0);
		return;
	}
	bHasColor=false;
}

/** \brief Add the point's weighted coordinates to x,y,z
*
* @param weight is a factor on \p x,y,z
* @param x,y,z [inout] are used to accumulate the point's coordinates
*/
inline void addWeightedOwnCoords(double weight,double& x,double& y,double& z) const
{
	x+=(weight*coordX);
	y+=(weight*coordY);
	z+=(weight*coordZ);
}

/** \brief Less than operator
*
* @param p [in] is compared with *this
*
* @return true if the coordinates of the present point are lexicographically smaller than the ones of \p p or false otherwise
*/
	bool operator<(const Point3& p) const
	{
		if(coordX<p.coordX) return true;
		if(coordX>p.coordX) return false;
		if(coordY<p.coordY) return true;
		if(coordY>p.coordY) return false;
		return coordZ<p.coordZ;
	}

/** \brief Greater than operator
*
* @param p [in] The point whose coordinates are compared with the ones of the present point
*
*
* @return true if the coordinates of the present point are lexicographically greater than the ones of \p p or false otherwise
*/
	bool operator>(const Point3& p) const
	{
		if(coordX>p.coordX) return true;
		if(coordX<p.coordX) return false;
		if(coordY>p.coordY) return true;
		if(coordY<p.coordY) return false;
		return coordY>p.coordY;
	}
/** \brief Equality operator
*
* @param p [in] The point whose coordinates are compared with the ones of the present point
*/
	bool operator==(const Point3& p) const
	{
		if(coordX==p.coordX && coordY==p.coordY && coordZ==p.coordZ) return true;
		return false;
	}
/** \brief Inequality operator
*
* @param p [in] The point whose coordinates are compared with the ones of the present point
*/
bool operator!=(const Point3& p) const
{
	return (coordX!=p.coordX || coordY!=p.coordY || coordZ!=p.coordZ);
}


/** \brief Set the coordiantes
*
* Set the coordinates of the present point to \p x_,\p y_,\p z_.
*
* @param x_,y_,z_ [in] are the coordinates to be assigned
*/
void set(const double x_,const double y_,const double z_)
{
	coordX=x_;
	coordY=y_;
	coordZ=z_;
}

/** \brief Set the coordiantes
*
* Set the coordinates of the present point to the ones of \p pnt
*
* @param pnt carries the coordinates to be assigned
*/
void set(const Point3& pnt)
{
	coordX=pnt.x();
	coordY=pnt.y();
	coordZ=pnt.z();
}




/** \brief operator-
 *
 * @return the difference vector ( \p *this - \p other) i.e., a vector pointing
 * from the point \p other to \p *this.
 */
Vector3 operator-(const Point3& other) const
{
	double xdiff(x()-other.x());
	double ydiff(y()-other.y());
	double zdiff(z()-other.z());
	return Vector3(xdiff,ydiff,zdiff);
}

/** \brief operator+
 *
 * @return a point that corresponds to the present point moved by @p vec
 */
Point3 operator+(const Vector3& vec) const
{
	return Point3(x()+vec.x(),y()+vec.y(),z()+vec.z());
}


/** \brief operator-
 *
 * @return a point that corresponds to the present point moved by @p vec
 */
Point3 operator-(const Vector3& vec) const
{
	return Point3(x()-vec.x(),y()-vec.y(),z()-vec.z());
}


friend std::ostream &operator<<(std::ostream &stream, const Point3& pnt);
friend std::istream &operator>>(std::istream &stream, Point3& pnt);


protected:
	double coordX;
	double coordY;
	double coordZ;
	unsigned char r,g,b,a; // Color 0-255
	bool bHasColor;
};








// Free functions
inline std::ostream &operator<<(std::ostream &stream, const Point3& pnt)
{
	if(pnt.hasColor())
	{
		stream << "Point3 ("<<&pnt<<"): "<<pnt.x()<<", "<<pnt.y()<<", "<<pnt.z()<<", color("<<int(pnt.r)<<","<<int(pnt.g)<<","<<int(pnt.b)<<","<<int(pnt.a)<<")";
	}
	else
	{
		stream << "Point3 ("<<&pnt<<"): "<<pnt.x()<<", "<<pnt.y()<<", "<<pnt.z()<<", no color";
	}
	return stream;
}

inline std::istream &operator>>(std::istream &stream, Point3& pnt)
{
	stream >> pnt.coordX >> pnt.coordY >> pnt.coordZ;
	return stream;
}


/** \brief Get the squared distance between two points
*/
inline double sqDistance(const Point3& p0,const Point3& p1)
{
	double deltaX=p1.x()-p0.x();
	double deltaY=p1.y()-p0.y();
	double deltaZ=p1.z()-p0.z();
	return (deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ);
}

/** \brief Get the squared distance between two points
*/
inline double sqDistance(const Point3* p0,const Point3* p1)
{
	double deltaX=p1->x()-p0->x();
	double deltaY=p1->y()-p0->y();
	double deltaZ=p1->z()-p0->z();
	return (deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ);
}


/** \brief Get the squared distance between two points
*/
inline double distance(const Point3& p0,const Point3& p1)
{
	double deltaX=p1.x()-p0.x();
	double deltaY=p1.y()-p0.y();
	double deltaZ=p1.z()-p0.z();
	return sqrt(deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ);
}


/** \brief Midpoint of p0 and p1
*/
inline
Point3 center(const Point3& p0,const Point3& p1)
{
	Point3 center((p0.x()+p1.x())/2.0,(p0.y()+p1.y())/2.0,(p0.z()+p1.z())/2.0);
	return center;
}


} // (namespace)

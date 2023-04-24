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
#include <iostream>
#include "wof_api_definitions.h" // for CLASS_DECLSPEC

namespace GEOM_WOF {


/** \brief 3D Vector
*/
class CLASS_DECLSPEC Vector3
{
public:

/** \brief Constructor
 *
 * @param x_,y_,z_ Values to initialize the Vector
*/
Vector3(const double x_,const double y_,const double z_);

/** \brief Default constructor
*
* The vector is initialized to (0,0,0)
*/
Vector3();

/** \brief Copy constructor
*
* Copies \p v_
*/
Vector3(const Vector3& v_);

/** \brief isDegenerate
*
* @return true if the vector length is 0, false otherwise.
*/
bool isDegenerate() const;

/** \brief Get x,y,z
*
* @param x_,y_,z_ [out] Used to return the x,y,z-values of the Vector
*/
void xyz(double& x_,double& y_,double& z_) const
{
	x_=valX;
	y_=valY;
	z_=valZ;
}

/** \brief Get the x-value
 *
 * @return x
*/
inline double x() const
{
	return valX;
}


/** \brief Get the y-value
* @return y
*/
inline double y() const
{
	return valY;
}



/** \brief Get the z-value.
 * @return z
*/
inline double z() const
{
	return valZ;
}

/** \brief Set x,y,z
 *
 * Assigns values to the present Vector3
 *
 * \param x_,y_,z_ Values to assign
*/
void set(const double x_,const double y_,const double z_);

/** \brief Add a Vector3 to the present one
 *
 * \param other is added to the present Vector3
*/
void add(const Vector3& other)
{
	valX+=other.valX;
	valY+=other.valY;
	valZ+=other.valZ;
}
void sub(const Vector3& other)
{
	valX-=other.valX;
	valY-=other.valY;
	valZ-=other.valZ;
}
void div(double div)
{
	valX/=div;
	valY/=div;
	valZ/=div;
}
void mul(double mul)
{
	valX*=mul;
	valY*=mul;
	valZ*=mul;
}

/** \brief Get the squared length of the vector
 *
 * @return the squared length of the Vector3
*/
double sqLength() const;



/** \brief Get max index
*
* @return the index of the largest absolute component (0,1 or 2)
*/
int getMaxAbsIndex() const;


/** \brief Get max component
*
* \return the maximum component
*/
double getMaxComponent() const
{
	if(valX>=valY && valX>=valZ) return valX;
	if(valY>=valZ) return valY;
		else return valZ;
}
/** \brief Get max absolute component
*
* @return the maximum absolute component
*/
double getMaxAbsComponent() const;


/** \brief Get component i
*
* @return the \p i-th component
*/

double getCartesian(int i) const;

/// @private
bool isNV() const;

/** \brief Get the length of the vector
 *
 * \return the length of the vector
*/
double length() const;

/** \brief Scalar product
 *
 * \return the scalar product of the present Vector3 and \p other
*/
double operator*(const Vector3& other) const;


/** \brief Multiply by a scalar value
 *
 * \return the present Vector3 multiplied by \p val
*/
Vector3 operator*(double val) const;

/** \brief Divide by a scalar value
 *
 * \return the present Vector3 divided by \p val
*/
Vector3 operator/(double val) const;

/** \brief Equality operator
 *
 * \return true when the present Vector3 has the same x,y,z-values as \p other
*/
Vector3& operator=(const Vector3& other);

Vector3& operator+=(const Vector3& other);
Vector3& operator-=(const Vector3& other);
Vector3& operator/=(double div);
Vector3& operator*=(double mul);



protected:

double valX;
double valY;
double valZ;
};




// Free functions
inline std::ostream &operator<<(std::ostream &stream, const Vector3& vec)
{
	stream << "Vector3: "<<vec.x()<<", "<<vec.y()<<", "<<vec.z();
	return stream;
}


/** \brief Cross product
*/
inline Vector3 crossProduct(const Vector3& vec0,const Vector3& vec1)
{

	double x=vec0.y() * vec1.z() - vec0.z() * vec1.y();
	double y=vec0.z() * vec1.x() - vec0.x() * vec1.z();
	double z=vec0.x() * vec1.y() - vec0.y() * vec1.x();
	return Vector3(x,y,z);
}


/** \brief Normalize
*/
CLASS_DECLSPEC
Vector3 normalize(const Vector3& other);


inline Vector3 operator-(const Vector3& in)
{
	return Vector3(-in.x(),-in.y(),-in.z());
}


inline Vector3 operator*(double d,const Vector3& vec)
{
	return Vector3(d*vec.x(),d*vec.y(),d*vec.z());
}


inline Vector3 operator+(const Vector3& vec0,const Vector3& vec1)
{
	return Vector3(vec0.x()+vec1.x(), vec0.y()+vec1.y() , vec0.z()+vec1.z());
}


inline Vector3 operator-(const Vector3& vec0,const Vector3& vec1)
{
	return Vector3(vec0.x()-vec1.x(), vec0.y()-vec1.y() , vec0.z()-vec1.z());
}


} // (namespace)

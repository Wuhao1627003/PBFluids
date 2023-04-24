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
/// @file wof_api_io.h
#pragma once
#include <vector>
#include "Point3.h"
#include "Mesh.h"

/** Namespace GEOM_WOF
 *
 * Namespace of the WOF library
 */
namespace GEOM_WOF {
/** \brief IO functions
 *
 * Read/Write functions for point clouds and triangle meshes.
 */

/** \defgroup fileIO File I/O
 *  @{
 */

/** \brief Filetype
 */
enum FileType
{
	FT_STL, ///< FileType STL based on the filename extension [.stl]
	FT_PLY, ///< FileType PLY based on the filename extension [.ply]
	FT_XYZ, ///< FileType XYZ based on the filename extensions [.xyz|.txt|.asc]
	FT_BIN, ///< FileType BIN based on the filename extension [.bin]
	FT_LIST, ///< FileType LIST based on the filename extension [.list]
	FT_UNKNOWN  ///< FileType UNKNOWN for unknown extensions
};


/** \brief Get File Type
 *
 * @return the file type ( \ref FT_STL, \ref FT_PLY, \ref FT_XYZ, \ref FT_BIN, \ref FT_LIST, \ref FT_UNKNOWN) based on the filename extension.
 */
CLASS_DECLSPEC
FileType getFileType(const std::string& filename);



// *** Write Points: ASCII, binary, Ply(ASCII+binary) ***
/** \brief Write points to an ASCII file
 *
 * Writes points to an ASCII file, three coordinates (x y z) per line,
 * whitespace separated.
 *
 * \note Data exchange through ASCII files is easy and convenient but
 * floating point coordinates are not necessarily exact when represented
 * as decimal numbers and ASCII files are big compared to other formats.
 * Thus writing binary files using writePoints_BIN() is recommended.
 *
 * @param filename [in] is the output filename
 * @param vPoints [in] contains the points to be written
 * @return true when the operation was successful or false otherwise.
*/
CLASS_DECLSPEC
bool writePoints_ASCII(const char* filename,const std::vector<Point3>& vPoints);



/** \brief Write points to a binary file
 *
 * Writes a binary file, the format is: (int,size_t,double,...,double)\n
 * Thereby the first \p int is always 30, the size_t value is vPoints.size()
 * and the double precision values are x0,y0,z0,...,xn,yn,zn.
 *
 *
 * @param [in] filename is the output filename
 * @param [in] vPoints contains the points to be written
 * @return true when the operation was successful or false otherwise

 *
 * \see readPoints_BIN()
*/
CLASS_DECLSPEC
bool writePoints_BIN(const char* filename,std::vector<Point3>& vPoints);



/** \brief Write points to a *.ply file
 *
 * @param filename [in] is the input filename
 * @param bReadPoints [in] Use true to get only the points of the
 * *.ply file. Otherwise, when you are interested in the triangles
 * then use false to get 3 subsequent corners per triangle.
 * @param vPointsOut [out] is used to return the points
 * @return true when the operation was successful or false otherwise
 *
 * @note This function reads also colors whose ply properties are
 * named 'red','green','blue' or alternatively 'r','g','b'. Alpha
 * can be named 'alpha' or 'scalar_Intensity'. Values are stored
 * as unsigned char i.e., 0-255 and are automatically scaled up if
 * given in the range 0.0-1.0 or scaled down if >255.0.
 */
CLASS_DECLSPEC
bool readPly(const char* filename,bool bReadPoints, std::vector<Point3>& vPointsOut);

CLASS_DECLSPEC
bool writePointsPly(const std::string & filename,
					std::vector<GEOM_WOF::Point3>& vPoints,
					bool bASCII
					);



// *** Read Points ***
/** \brief Read points from an ASCII file
 *
 * Reads points from a simple ASCII file. Expected file format: Three
 * coordinates (x y z) per line, whitespace separated.
 *
 * @param filename [in] is the input filename
 * @param vPoints [out] is used to return the points
 * @return true [in] in case of success or false otherwise
*/
CLASS_DECLSPEC
bool readPoints_ASCII(const char* filename,std::vector<Point3>& vPoints);



/** \brief Read points from a binary file
 *
 * @param filename [in] is a binary input file
 * @param vPointsOut [out] is used to return the points
 *
 * @return true in case of success or false otherwise
 *
 * \see writePoints_BIN()
*/
CLASS_DECLSPEC
bool readPoints_BIN(const char* filename, std::vector<Point3>& vPointsOut);

////////////////////
// *** Read mesh ***
////////////////////
/** \brief Read a mesh from ASCII STL
 *
 * @param filename [in] is the input filename
 * @param vTriangleCorners [out] is used to return three points per triangle
 * @return true when the operation was successful or false otherwise
*/
CLASS_DECLSPEC
bool readSTL_ASCII(const char* filename,std::vector<Point3>& vTriangleCorners);



/** \brief Read points from a file (automatic detection)
 *
 * This function reads points from a *.ply-File (ASCII or binary), an
 * *.xyz-File (ASCII, 3 coordinates per line), or a *.bin-File (simple
 * binary format). The file type is automatically determined from the
 * filename extension.
 *
 * @param [in] inFilename is the input filename
 * @param [out] vPoints is used to return the points
 * @return true in case of success, false otherwise
 *
 * @see In case of a *.ply file colors are also read, see readPly()
 * for details on colors.
*/
CLASS_DECLSPEC
bool readPoints_auto(std::string& inFilename,std::vector<Point3>& vPoints);



/** \brief Write points to a file
 *
 * This function writes points to a *.ply-File, *.xyz-File (ASCII,
 * 3 coordinates per line), or a *.bin-File (simple binary format).
 * The file type is automatically determined from the filename
 * extension.
 *
 * @param [in] outFilename is the output filename
 * @param [in] vPoints contains the points to be written
 * @param [in] bASCII specifies that ASCII mode shall be used when the
 * file can be written in ASCII- or binary mode (as it is the case
 * for *.ply)
 * @return true in case of success or false otherwise
*/
CLASS_DECLSPEC
bool writePoints_auto(std::string& outFilename,std::vector<Point3>& vPoints,bool bASCII);



/** \brief Write mesh to a file
 *
 * This function writes a Mesh to file. Available formats are
 * *.ply (ASCII or binary), *.stl (only ASCII) and Geomview-*.list
 * (ASCII). The file type is automatically determined from the filename
 * extension.
 *
 * @param [in] filename is the output filename
 * @param [in] pMesh is the mesh to be written
 * @param [in] bASCII specifies that ASCII mode shall be used when the
 * file can be written in ASCII- or binary mode (as it is the case
 * for *.ply)
 * @return true in case of success or false otherwise
*/
CLASS_DECLSPEC
bool writeMesh_auto(const std::string& filename,std::shared_ptr<Mesh> pMesh,bool bASCII);


/** @} */

} // Namespace

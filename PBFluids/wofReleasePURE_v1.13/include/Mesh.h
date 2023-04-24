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

#include <vector>
#include <memory>
#include "Point3.h"

namespace GEOM_WOF {


class RMesh; // FWD
class Mesh; // FWD

/** MeshPtr is a shared pointer to Mesh */
typedef std::shared_ptr<Mesh> MeshPtr;


/** \brief 3D Mesh class
 *
 * The Mesh is a 3D triangle mesh.
 */
class CLASS_DECLSPEC Mesh
{
public:
	/** \brief Constructor
	 */
	Mesh(RMesh* pRMesh);
	/** \brief Destructor
	 */
	~Mesh();
	Mesh(const Mesh&)=delete;
    Mesh& operator=(const Mesh&)=delete;

	/** \brief Get the number of triangles
	 */
	size_t getNumT() const;

	/** \brief Get Triangles
	 *
	 * @param [out] vTriangleCorners is used to return the triangles
	 * as 3 vertex pointers per triangle. The order of the corners
	 * per triangle is counterclockwise.
	 */
	void getTriangles(std::vector<Point3*>& vTriangleCorners) const;
	/** \brief Get Points
	 *
	 * @param [out] vPoints is used to return the vertex pointers
	 */
	void getPoints(std::vector<Point3*>& vPoints) const;
	/** \brief Get the Mesh as Vertices and Indices
	 *
	 * @param vVertices [out] The vertices of the Mesh
	 * @param vCornerIndices [out] Three subsequent indices for each
	 * triangle. The indices refer to points in vVertices and thus
	 * the index range starts with 0.
	 *
	 * @note There are file formats (e.g., *.obj) where the first
	 * index must be 1, not 0! Simply increment the indices by 1 then.
	 *
	 *
	 */

	/** \brief Contrast spreading
	 *
	 * RGB contrast spreading.
	 *
	 * \param lowerSkipFrac is by default 0.01 which means the lower
	 * 1 % of all values is set to 0.
	 * \param upperSkipFrac is by default 0.01 which means the upper
	 * 1 % of all values is set to 255.
	 *
	 */
	void contrastSpread(double lowerSkipFrac=.01,double upperSkipFrac=.01);


	/** \brief Colorize filled holes
	 *
	 * Vertices in filled holes are by default black. Approximate colors
	 * from the hole boundaries.
	 */
	void colorizeFilledHoles();

	void getVertexIndexData(std::vector<Point3*>& vVertices,
							std::vector<int>& vCornerIndices) const;
	/** \brief Write Ply (Binary)
	 *
	 * Writes a binary PLY file
	 *
	 * \param name [in] is the output filename.
	 */
	bool writePly_BIN(const std::string& name) const;
	/** \brief Write Ply (ASCII)
	 *
	 * Writes an ASCII PLY file
	 *
	 * \param name [in] is the output filename.
	 */
	bool writePly_ASCII(const std::string& name) const;
	/** \brief Write Geomview
	 *
	 * Writes a file for the Geomview viewer
	 *
	 * \param name [in] is the output filename.
	 */
	bool writeGeomview_ASCII(const std::string& name) const;
	/** \brief Write STL (ASCII)
	 *
	 * Writes an ASCII STL file
	 *
	 * \param name [in] is the output filename.
	 *
	 */
	bool writeStl_ASCII(const std::string& name) const;
	/** \brief Print Statistics
	 *
	 * Prints mesh statistics to stdout
	 *
	 * @param name serves as arbitrary identifier that is also printed to stdout
	 */
	void printStatistics(const std::string& name) const;
	/** \brief Get the average edge length
	 *
	 * Computes and returns the average edge length
	 */
	double getAverageEdgeLength() const;


	/// @private
	RMesh* getRMesh();

private:
	RMesh* pRMesh;
};



} // NAMESPACE



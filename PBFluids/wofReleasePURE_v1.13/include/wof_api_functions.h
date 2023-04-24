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
/// @file wof_api_functions.h
#pragma once

#include <vector>
#include <float.h>
#include "Point3.h"
#include "Mesh.h"

namespace GEOM_WOF {

struct Dat; // FWD

/** \brief Geometry operations
 *
 * Operations to reconstruct a surface, to sample-and-reconstruct a
 * mesh and to create feature-aligned quality point clouds.
 */

/** \defgroup geometryFun Geometry functions
 *  @{
 */

/** \brief Remesh (melt) a triangle mesh
*
* This function converts a mesh into a point cloud and then reconstructs
* it from there. This way you can get rid of meshing errors and
* unnecessary complexity. This mesh-melting operation may change the
* topology of the object i.e., depending on the sample distance it may
* close holes and create additional bridges.
*
* @param [in] vInputCorners contains the input triangles (3 corner points per triangle)
* @param [in] avgLength specifies the average distance to be used.
* Choose this parameter with care to avoid an extreme number of
* elements.
* @param [in] featureThresh is an optional parameter that specifies
* that edges above this value shall be treated as feature lines on
* which points shall be placed.
*
*/
CLASS_DECLSPEC
MeshPtr melt(	std::vector<Point3>& vInputCorners,
				double avgLength,
				double featureThresh=15.0
			);


/** \brief Reconstruct with an automatic spacing value
*
* This function reconstructs a triangular mesh from a 3D point cloud.
*
* @param [in] vPoints contains the input point cloud
* @param [in] bAllowSmoothing specifies if the point cloud shall be
* smoothed before reconstruction.
* @param [in] sfactor is a factor on the estimated point cloud density.
* The average spacing in the point cloud is automatically estimated and
* multiplied by sfactor. Use larger values to avoid holes upfront and
* to create coarser meshes. By default sfactor=2.0.
* @param [in] numGrowSteps is the number of steps by which the
* boundaries of holes can grow to close the holes. The steps
* correspond approximately to the used spacing distance. Use 0
* to deactivate. Larger values close larger holes but also take more time.
* @return the reconstructed mesh.
*
*
* \sa reduce(), laplacianSmoothing(), edgeFlips(), edgeFlipsSimA()
* which are powerful postprocessing fuctions.
*/
CLASS_DECLSPEC
MeshPtr reconstruct_auto(std::vector<Point3>& vPoints,bool bAllowSmoothing,double sfactor=2.0,int numGrowSteps=10);

/** \brief Reconstruct with an absolute spacing value
*
* This function reconstructs a triangular mesh from a 3D point cloud.
*
* @param [in] vPoints contains the input point cloud
* @param [in] spacingAbs is an absolute spacing value. Large values
* @param [in] bAllowSmoothing specifies if the point cloud shall be
* smoothed before reconstruction.
* @param [in] numGrowSteps is the number of steps by which the
* boundaries of holes can grow to close the holes. The steps
* correspond approximately to the used spacing distance. Larger
* values close larger holes and can take considerably more time.
* @return the reconstructed mesh.
*
* If the absolute spacing value is unknown then better use
* reconstruct_auto() which automatically estimates the point
* cloud density.
*
* \sa reduce(), laplacianSmoothing(), edgeFlips(), edgeFlipsSimA()
* which are powerful postprocessing fuctions.

*/
CLASS_DECLSPEC
MeshPtr reconstruct_abs(std::vector<Point3>& vPoints,bool bAllowSmoothing,double spacingAbs,int numGrowSteps=10);


/** \brief Mesh-to-Cloud
*
* This function creates a 3D point cloud from an input mesh.
*
* @param [in] vCornersIn contains the input triangles (3 corners per triangle)
* @param [in] length specifies the approximate distance to be used
* @param [in] featureThresh is an optional parameter. Edges with larger
* dihedral angles are treated as feature edges and points are preferably
* placed there.
* @param [out] vCloudOut is used to return the point cloud
*/
CLASS_DECLSPEC
void toCloud(	std::vector<Point3>& vCornersIn,
				double length,
				double featureThresh,
				std::vector<Point3>& vCloudOut
				);



/** \brief Flip edges
*
* This function uses a simple and fast edge-flip algorithm to fit the
* mesh better to thought surface.
*
* @param [in] pMesh is the mesh to be improved
*
* \sa edgeFlipsSimA()
*/
CLASS_DECLSPEC
void edgeFlips(MeshPtr pMesh);

/** \brief Flip edges - simulated annealing
*
* Like edgeFlips() the present function flips edges to adapt the
* mesh better to the thought surface. But with a computationally
* more expensive simulated annealing algorithm that achieves yet
* better output quality.
*
* @param [in] pMesh is the mesh to be improved
*
* \sa edgeFlips()
*/
CLASS_DECLSPEC
void edgeFlipsSimA(MeshPtr pMesh);

/** \brief Laplacian smoothing
*
* This function applies weighted laplacian smoothing to create a
* smoother mesh.
*
* @param [in] pMesh is the mesh to be improved
* @param [in] numIterations specifies how much the mesh shall be
* smoothed.
*
*/
CLASS_DECLSPEC
void laplacianSmoothing(MeshPtr pMesh,int numIterations);

/** \brief Reduce
*
* This function reduces the number of triangles in \p pMesh.
*
* @param [in] pMesh is the mesh to be simplified
* @param [in] targetLen is a limit on the edge length (the algorithm
* will skip larger edges). Default: DBL_MAX
* @param [in] maximum angle deviation per simplification step. Default: 2.0
*
* @return the number of removed triangles
*/
CLASS_DECLSPEC
int reduce(MeshPtr pMesh,double targetLen=DBL_MAX,double maxDev=2.0);

/** \private Find holes - Tentative function
 *
 */
CLASS_DECLSPEC
void findHoles(MeshPtr pMesh);

/** \private Estimate the average spacing
 *
 * Estimates the average distance between points. If this value is
 * used as a parameter for surface reconstruction it is recommended
 * to multiply it by 2.0 to account for inhomogeneous sampling.
 */
CLASS_DECLSPEC
double estimateAvgSpacing(std::vector<Point3>& vPoints);

/** @}*/


/** \brief Subscribe to progress updates
 *
 * You can provide your own progress receiver class (e.g. progress bar)
 * deriving from ProgressBase. Whenever the progress state changes its
 * update method ProgressBase::update() will be called.
 */
CLASS_DECLSPEC
void subscribe(ProgressBase* pProgressBase);



/** \private
 */
CLASS_DECLSPEC
void setLic(
        const char* l1,
        const char* l2,
        const char* dt,
        const char* s1,
        const char* s2_
        );


} // NAMESPACE



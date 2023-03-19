#ifndef sphere_H_
#define sphere_H_

#include "Utils/vec.h"

#include <maya/MPoint.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>

class SphereMesh
{
public:
    SphereMesh(const MPoint& start, double r = 0.25);
    ~SphereMesh();

    void getMesh(
        MPointArray& points, 
        MIntArray& faceCounts, 
        MIntArray& faceConnects);
    
    void appendToMesh(
        MPointArray& points, 
        MIntArray& faceCounts, 
        MIntArray& faceConnects);

protected:
    MPoint mStart;
    double r;

    // Creates a sphere from p with radius r
    static void initSphereMesh(vec3 p, double r);
    static MPointArray gPoints;
    static MIntArray gFaceCounts;
    static MIntArray gFaceConnects;
};

#endif
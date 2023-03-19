#include "sphere.h"
#include <maya/MMatrix.h>
#include <math.h>

MPointArray SphereMesh::gPoints;
MIntArray SphereMesh::gFaceCounts;
MIntArray SphereMesh::gFaceConnects;

SphereMesh::SphereMesh(
   const MPoint& start, double _r) : 
    mStart(start), r(_r)
{
    if (gPoints.length() == 0)
    {
        initSphereMesh(vec3(start.x, start.y, start.z), r);
    }
}

SphereMesh::~SphereMesh(){}

void SphereMesh::appendToMesh(
    MPointArray& points, 
    MIntArray& faceCounts, 
    MIntArray& faceConnects)
{
    int startIndex = points.length(); // offset for indexes
    for (int i = 0; i < gPoints.length(); i++)
    {
        points.append(gPoints[i]);
    }
    for (int i = 0; i < gFaceCounts.length(); i++)
    {
        faceCounts.append(gFaceCounts[i]);
    }

    for (int i = 0; i < gFaceConnects.length(); i++)
    {
        faceConnects.append(gFaceConnects[i]+startIndex);
    }
}

void SphereMesh::getMesh(
    MPointArray& points, 
    MIntArray& faceCounts, 
    MIntArray& faceConnects)
{
    points = gPoints;    
    faceCounts = gFaceCounts;
    faceConnects = gFaceConnects;
}

void SphereMesh::initSphereMesh(vec3 p, double r)
{
    int numSlices = 10;
    double angle = M_PI*2/numSlices;

    // Add points
    gPoints.clear();
    gFaceCounts.clear();
    gFaceConnects.clear();

    for (int i = 0; i < numSlices; i++)
    {
        gPoints.append(MPoint(0.8 * r + p[0], p[1] + r * cos(angle * i), p[2] + r * sin(angle * i)));
    }
    for (int i = 0; i < numSlices; i++)
    {
        gPoints.append(MPoint(-0.8 * r + p[0], p[1] + r * cos(angle * i), p[2] + r * sin(angle * i)));
    }
    // endcap 1
    gPoints.append(MPoint(r + p[0], p[1], p[2]));

    // endcap 2
    gPoints.append(MPoint(-r + p[0], p[1], p[2]));

    // Set indices for endcap 1
    for (int i = 0; i < numSlices; i++)
    {
        gFaceCounts.append(3); // append triangle
        gFaceConnects.append(2*numSlices);
        gFaceConnects.append((i+1)%numSlices);
        gFaceConnects.append(i);
    }
    
    // Set indices for endcap 2
    for (int i = numSlices; i < 2*numSlices; i++)
    {
        gFaceCounts.append(3); // append triangle
        gFaceConnects.append(2*numSlices+1);
        gFaceConnects.append(i);
        int next = i+1;
        if (next >= 2*numSlices) next = numSlices;
        gFaceConnects.append(next);
    }

    // Set indices for middle
    for (int i = 0; i < numSlices; i++)
    {
        gFaceCounts.append(4); // append quad
        gFaceConnects.append(i);
        gFaceConnects.append((i+1)%numSlices);
        gFaceConnects.append((i+1)%numSlices+numSlices);
        gFaceConnects.append(i+numSlices);
    }
}

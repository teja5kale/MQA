#pragma once
#include "Point.h"
#include "Writer.h"
#include "Triangulation.h"
#include <vector>
#include <map>

using namespace Geometry;

class ObjWriter : public Writer
{
public:
    void Write(const std::string& filename, const Triangulation& triangulation, QProgressBar*);

private:
    void findAndAddPoint(Point point, std::vector<Point>& pointList, std::map<Point, int>& uniqueValueMap);
    std::string formulateVertex(Triangulation triangulation, Point point);
    std::string formulateVertexNormal(Triangulation triangulation, Point point);
    std::string formulateFace(Triangle triangle, std::map<Point, int>, std::map<Point, int>);
};

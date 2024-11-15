#include "ObjWriter.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "Point.h"

using namespace Geometry;

void ObjWriter::Write(const std::string& filename, const Triangulation& triangulation, QProgressBar* progressBar)
{
    int count = 0;
    std::vector<Point> uniqueVerticesList;
    std::vector<Point> uniqueNormalsList;
    std::map<Point, int> VerticeMap;
    std::map<Point, int> NormalMap;
    std::ofstream outfile(filename);

    for (auto triangle : triangulation.Triangles)
    {
        findAndAddPoint(triangle.P1(), uniqueVerticesList, VerticeMap);
        findAndAddPoint(triangle.P2(), uniqueVerticesList, VerticeMap);
        findAndAddPoint(triangle.P3(), uniqueVerticesList, VerticeMap);
        findAndAddPoint(triangle.Normal(), uniqueNormalsList, NormalMap);
        progressBar->setRange(0, (uniqueVerticesList.size() + uniqueNormalsList.size() + triangulation.Triangles.size()));
    }
    if (outfile.is_open())
    {

        for each (Point pt in uniqueVerticesList)
        {
            outfile << std::fixed << std::setprecision(6) << formulateVertex(triangulation, pt);
            progressBar->setValue(count);
            count++;
        }
        for each (Point normals in uniqueNormalsList)
        {
            outfile << std::fixed << std::setprecision(6) << formulateVertexNormal(triangulation, normals);
            progressBar->setValue(count);
            count++;
        }
        for each (Triangle tri in triangulation.Triangles)
        {
            outfile << formulateFace(tri, VerticeMap, NormalMap) << std::endl;
            progressBar->setValue(count);
            count++;
        }
    }
}

void ObjWriter::findAndAddPoint(Point point, std::vector<Point>& pointList, std::map<Point, int>& uniqueValueMap)
{
    auto pair = uniqueValueMap.find(point);
    if (pair == uniqueValueMap.end())
    {
        pointList.push_back(point);
        uniqueValueMap[point] = pointList.size() - 1;
    }
}

std::string ObjWriter::formulateVertex(Triangulation triangulation, Point point)
{
    double x = triangulation.UniqueNumbers[point.X()];
    double y = triangulation.UniqueNumbers[point.Y()];
    double z = triangulation.UniqueNumbers[point.Z()];

    return "v " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "\n";
}

std::string ObjWriter::formulateVertexNormal(Triangulation triangulation, Point point)
{
    double x = triangulation.UniqueNumbers[point.X()];
    double y = triangulation.UniqueNumbers[point.Y()];
    double z = triangulation.UniqueNumbers[point.Z()];

    return "vn " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "\n";
}

std::string ObjWriter::formulateFace(Triangle triangle, std::map<Point, int> VerticeMap, std::map<Point, int> NormalMap)
{
    // v vt vn
    std::string new1 = std::to_string(VerticeMap[triangle.P1()] + 1) + "//" + std::to_string(NormalMap[triangle.Normal()] + 1)
        + " " + std::to_string(VerticeMap[triangle.P2()] + 1) + "//" + std::to_string(NormalMap[triangle.Normal()] + 1)
        + " " + std::to_string(VerticeMap[triangle.P3()] + 1) + "//" + std::to_string(NormalMap[triangle.Normal()] + 1);
    return "f " + new1;
}

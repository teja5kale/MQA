#include <QString>
#include <QList>
#include <fstream>
#include <iostream>
#include "OBJReader.h"
#include "Triangulation.h"
#include "Point.h"
#include "Triangle.h"
#include <vector>
#include <sstream>
#include <cassert>
#include <string>
#include <map>

#define TOLERANCE 0.0000001

using namespace Geometry;

OBJReader::OBJReader()
{
}

OBJReader::~OBJReader()
{
}
bool OBJReader::operator()(double a, double b) const
{
    return fabs(a - b) > TOLERANCE ? a < b : false;
}
void OBJReader::read(const std::string& fileName, Triangulation& triangulation)
{
    std::map<double, int, OBJReader> uniqueMap;
    double xyz[3];
    double normalXYZ[3];
    std::string fLetter;
    std::string str1;
    std::string str2;
    std::string str3;
    std::vector<Point> vertices;
    std::vector<Point> normals;


    std::ifstream infile(fileName);
    if (infile.is_open())
    {
        std::string line;

        while (getline(infile, line))
        {
            std::stringstream ss(line);
            QString _line = QString::fromStdString(line);
            QStringList linelist = _line.split(" ");
            if (linelist.value(0) == "v")
            {
                xyz[0] = linelist.value(1).toDouble();
                xyz[1] = linelist.value(2).toDouble();
                xyz[2] = linelist.value(3).toDouble();
                helper(xyz, vertices, uniqueMap, triangulation); 
            }

            if (linelist.value(0) == "vn")
            {
                xyz[0] = linelist.value(1).toDouble();
                xyz[1] = linelist.value(2).toDouble();
                xyz[2] = linelist.value(3).toDouble();
                helper(xyz, normals, uniqueMap, triangulation);
            }

            if (linelist.value(0) == "f")
            {
                ss >> fLetter >> str1 >> str2 >> str3;
                QString str_s1 = QString::fromStdString(str1);
                QString str_s2 = QString::fromStdString(str2);
                QString str_s3 = QString::fromStdString(str3);
                QStringList splitList = str_s1.split("/");
                splitList << str_s2.split("/") << str_s3.split("/");
                int normalId = splitList.value(2).toInt() - 1;
                int firstVertexId = splitList.value(0).toInt() - 1;
                int secondVertexId = splitList.value(3).toInt() - 1;
                int thirdVertexId = splitList.value(6).toInt() - 1;

                triangulation.Triangles.push_back(Triangle(normals[normalId], vertices[firstVertexId], vertices[secondVertexId], vertices[thirdVertexId]));
            }
        }
    }
}

void OBJReader::helper(double xyz[3], std::vector<Point>& vertices, std::map<double, int, OBJReader>& uniqueMap, Triangulation& triangulation)
{
    int pt[3];
    for (int i = 0; i < 3; i++)
    {
        auto pair = uniqueMap.find(xyz[i]);
        if (pair == uniqueMap.end())
        {
            triangulation.UniqueNumbers.push_back(xyz[i]);
            uniqueMap[xyz[i]] = triangulation.UniqueNumbers.size() - 1;
            pt[i] = triangulation.UniqueNumbers.size() - 1;

        }
        else
        {
            pt[i] = pair->second;

        }

    }
    vertices.push_back(Point(pt[0], pt[1], pt[2]));
}

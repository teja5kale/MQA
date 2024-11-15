#include "QualityAnalysis.h"
#include "Triangulation.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include "Point.h"
#include "Triangle.h"

#define PI 3.14159265358979323846

QualityAnalysis::QualityAnalysis::QualityAnalysis()
{
}


QualityAnalysis::QualityAnalysis::~QualityAnalysis()
{
}

std::vector<double> QualityAnalysis::QualityAnalysis::convertPointToVector(Geometry::Point point, Geometry::Triangulation triangulation)
{
    double v1 = triangulation.UniqueNumbers[point.X()];
    double v2 = triangulation.UniqueNumbers[point.Y()];
    double v3 = triangulation.UniqueNumbers[point.Z()];
    return std::vector<double>{ v1, v2, v3 };
}

std::vector<double> QualityAnalysis::QualityAnalysis::negativePoint(std::vector<double> point)
{
    return std::vector<double>{ -point[0], -point[1], -point[2]};
}

double QualityAnalysis::QualityAnalysis::clamp(double value, double min, double max)
{
    return (value < min) ? min : (value > max) ? max : value;
}

double QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(std::vector<double> firstPoint, std::vector<double> secondPoint)
{
    // Calculate the distance using Euclidean distance formula
    double dx = firstPoint[0] - secondPoint[0];
    double dy = firstPoint[1] - secondPoint[1];
    double dz = firstPoint[2] - secondPoint[2];

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

double QualityAnalysis::QualityAnalysis::calculateAngleBetweenVectors(std::vector<double> v1, std::vector<double> v2) {
    double dotProduct = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
    double magnitudeV1 = std::sqrt(v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2]);
    double magnitudeV2 = std::sqrt(v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]);
    double cosTheta = dotProduct / (magnitudeV1 * magnitudeV2);
    cosTheta = clamp(cosTheta, -1.0, 1.0);
    return std::acos(cosTheta) * (180.0 / PI);
}

double QualityAnalysis::QualityAnalysis::calculateSingleTriangleArea(Geometry::Triangle& triangle, Geometry::Triangulation triangulation)
{
    Geometry::Point vertex1 = triangle.P1();
    Geometry::Point vertex2 = triangle.P2();
    Geometry::Point vertex3 = triangle.P3();

    std::vector<double> vertex1Double = convertPointToVector(vertex1, triangulation);
    std::vector<double> vertex2Double = convertPointToVector(vertex2, triangulation);
    std::vector<double> vertex3Double = convertPointToVector(vertex3, triangulation);

    double side1 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex1Double, vertex2Double);
    double side2 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex2Double, vertex3Double);
    double side3 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex3Double, vertex1Double);

    double semiPerimeter = (side1 + side2 + side3) / 2;
    double area = std::sqrt(semiPerimeter * (semiPerimeter - side1) * (semiPerimeter - side2) * (semiPerimeter - side3));

    return area;
}

double QualityAnalysis::QualityAnalysis::calculateSingleTriangleAspectRatio(Geometry::Triangle& triangle, Geometry::Triangulation triangulation)
{
    Geometry::Point vertex1 = triangle.P1();
    Geometry::Point vertex2 = triangle.P2();
    Geometry::Point vertex3 = triangle.P3();

    std::vector<double> vertex1Double = convertPointToVector(vertex1, triangulation);
    std::vector<double> vertex2Double = convertPointToVector(vertex2, triangulation);
    std::vector<double> vertex3Double = convertPointToVector(vertex3, triangulation);

    double side1 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex1Double, vertex2Double);
    double side2 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex2Double, vertex3Double);
    double side3 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex3Double, vertex1Double);

    double longestEdge = std::max(std::max(side1, side2), side3);
    double shortestEdge = std::min(std::min(side1, side2), side3);

    double aspectRatio = longestEdge / shortestEdge;
    return aspectRatio;
}

double QualityAnalysis::QualityAnalysis::calculateSingleTriangleInteriorAngle(Geometry::Triangle& triangle, Geometry::Triangulation triangulation) 
{
    Geometry::Point vertex1 = triangle.P1();
    Geometry::Point vertex2 = triangle.P2();
    Geometry::Point vertex3 = triangle.P3();

    std::vector<double> vertex1Double = convertPointToVector(vertex1, triangulation);
    std::vector<double> vertex2Double = convertPointToVector(vertex2, triangulation);
    std::vector<double> vertex3Double = convertPointToVector(vertex3, triangulation);

    std::vector<double> AB{ vertex2Double[0] - vertex1Double[0], vertex2Double[1] - vertex1Double[1], vertex2Double[2] - vertex1Double[2] };
    std::vector<double> BC{ vertex3Double[0] - vertex2Double[0], vertex3Double[1] - vertex2Double[1], vertex3Double[2] - vertex2Double[2] };
    std::vector<double> CA{ vertex1Double[0] - vertex3Double[0], vertex1Double[1] - vertex3Double[1], vertex1Double[2] - vertex3Double[2] };

    double angleA = calculateAngleBetweenVectors(AB, negativePoint(CA));
    double angleB = calculateAngleBetweenVectors(AB, BC);
    double angleC = calculateAngleBetweenVectors(BC, negativePoint(CA));

    double averageAngle = (angleA + angleB + angleC) / 3.0;

    return averageAngle;
}

std::vector<double> QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(std::vector<double> v1, std::vector<double> v2, std::vector<double> v3)
{
    std::vector<double> edge1 = { v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2] };
    std::vector<double> edge2 = { v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2] };

    std::vector<double> normal = { edge1[1] * edge2[2] - edge1[2] * edge2[1], edge1[2] * edge2[0] - edge1[0] * edge2[2], edge1[0] * edge2[1] - edge1[1] * edge2[0] };

    double length = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    normal[0] /= length;
    normal[1] /= length;
    normal[2] /= length;

    return normal;
}

double QualityAnalysis::QualityAnalysis::minX(Geometry::Triangulation triangulation)
{
    double minX = 1000000;
    for (auto vertex : triangulation.Triangles) {
        if (vertex.P1().X() < minX) {
            minX = vertex.P1().X();
        }
		if (vertex.P2().X() < minX) {
			minX = vertex.P2().X();
		}
        if (vertex.P3().X() < minX) {
            minX = vertex.P3().X();
        }
    }
    return minX;
}

double QualityAnalysis::QualityAnalysis::minY(Geometry::Triangulation triangulation)
{
    double minY = 1000000;
    for (auto vertex : triangulation.Triangles) {
        if (vertex.P1().Y() < minY) {
            minY = vertex.P1().Y();
        }
        if (vertex.P2().Y() < minY) {
            minY = vertex.P2().Y();
        }
        if (vertex.P3().Y() < minY) {
            minY = vertex.P3().Y();
        }
    }
    return minY;
}

double QualityAnalysis::QualityAnalysis::minZ(Geometry::Triangulation triangulation)
{
    double minZ = 1000000;
    for (auto vertex : triangulation.Triangles) {
        if (vertex.P1().Z() < minZ) {
            minZ = vertex.P1().Z();
        }
        if (vertex.P2().Z() < minZ) {
            minZ = vertex.P2().Z();
        }
        if (vertex.P3().Z() < minZ) {
            minZ = vertex.P3().Z();
        }
    }
    return minZ;
}

double QualityAnalysis::QualityAnalysis::maxX(Geometry::Triangulation triangulation)
{
    double maxX = -1000000;
    for (auto vertex : triangulation.Triangles) {
        if (vertex.P1().X() > maxX) {
            maxX = vertex.P1().X();
        }
        if (vertex.P2().X() > maxX) {
            maxX = vertex.P2().X();
        }
        if (vertex.P3().X() > maxX) {
            maxX = vertex.P3().X();
        }
    }
    return maxX;
}

double QualityAnalysis::QualityAnalysis::maxY(Geometry::Triangulation triangulation)
{
    double maxY = -1000000;
    for (auto vertex : triangulation.Triangles) {
        if (vertex.P1().Y() > maxY) {
            maxY = vertex.P1().Y();
        }
        if (vertex.P2().Y() > maxY) {
            maxY = vertex.P2().Y();
        }
        if (vertex.P3().Y() > maxY) {
            maxY = vertex.P3().Y();
        }
    }
    return maxY;
}

double QualityAnalysis::QualityAnalysis::maxZ(Geometry::Triangulation triangulation)
{
    double maxZ = -1000000;
    for (auto vertex : triangulation.Triangles) {
        if (vertex.P1().Z() > maxZ) {
            maxZ = vertex.P1().Z();
        }
        if (vertex.P2().Z() > maxZ) {
            maxZ = vertex.P2().Z();
        }
        if (vertex.P3().Z() > maxZ) {
            maxZ = vertex.P3().Z();
        }
    }
    return maxZ;
}

double QualityAnalysis::QualityAnalysis::surfaceArea(Geometry::Triangulation triangulation)
{
	double totalSurfaceArea = 0;
	for (auto triangle : triangulation.Triangles)
	{
        totalSurfaceArea += QualityAnalysis::QualityAnalysis::calculateSingleTriangleArea(triangle, triangulation);
	}
	return totalSurfaceArea;
}

double QualityAnalysis::QualityAnalysis::triangleDensity(Geometry::Triangulation triangulation)
{
    double meshDensity = 0.0;

    double _surfaceArea = QualityAnalysis::QualityAnalysis::surfaceArea(triangulation);

    if (_surfaceArea != 0.0)
    {
        meshDensity = triangulation.Triangles.size() / _surfaceArea;
    }
    return meshDensity;
}

size_t QualityAnalysis::QualityAnalysis::numberOfTriangles(Geometry::Triangulation triangulation)
{
	return triangulation.Triangles.size();
}

size_t QualityAnalysis::QualityAnalysis::numberOfVertices(Geometry::Triangulation triangulation)
{
	std::vector<Geometry::Point> verticesList;
	for (auto triangle : triangulation.Triangles)
	{
		verticesList.push_back(triangle.P1());
		verticesList.push_back(triangle.P2());
		verticesList.push_back(triangle.P3());
	}
	std::set<Geometry::Point> verticesSet(verticesList.begin(), verticesList.end());
	return verticesSet.size();
}

double QualityAnalysis::QualityAnalysis::aspectRatio(Geometry::Triangulation triangulation)
{
	double totalAspectRatio = 0;
	for (auto triangle : triangulation.Triangles)
	{
		totalAspectRatio += QualityAnalysis::QualityAnalysis::calculateSingleTriangleAspectRatio(triangle, triangulation);
	}
	return totalAspectRatio / triangulation.Triangles.size();
}

double QualityAnalysis::QualityAnalysis::orthogonality(Geometry::Triangulation triangulation)
{
	double totalOrthogonality = 0;
	for (auto triangle : triangulation.Triangles)
	{
		totalOrthogonality += QualityAnalysis::QualityAnalysis::calculateSingleTriangleInteriorAngle(triangle, triangulation);
	}
	return totalOrthogonality / triangulation.Triangles.size();
}

double QualityAnalysis::QualityAnalysis::objectLength(Geometry::Triangulation triangulation) 
{
    return maxX(triangulation) - minX(triangulation);
}

double QualityAnalysis::QualityAnalysis::objectBreadth(Geometry::Triangulation triangulation)
{
	return maxY(triangulation) - minY(triangulation);
}

double QualityAnalysis::QualityAnalysis::objectHeight(Geometry::Triangulation triangulation)
{
	return maxZ(triangulation) - minZ(triangulation);
}

std::vector<std::vector<std::vector<double>>> QualityAnalysis::QualityAnalysis::drawBoundingBox(Geometry::Triangulation triangulation)
{
    std::vector<std::vector<std::vector<double>>> boundingBox;

    double _xMin = QualityAnalysis::QualityAnalysis::minX(triangulation);
    double _yMin = QualityAnalysis::QualityAnalysis::minY(triangulation);
    double _zMin = QualityAnalysis::QualityAnalysis::minZ(triangulation);
    double _xMax = QualityAnalysis::QualityAnalysis::maxX(triangulation);
    double _yMax = QualityAnalysis::QualityAnalysis::maxY(triangulation);
    double _zMax = QualityAnalysis::QualityAnalysis::maxZ(triangulation);

    std::vector<double> p1 = { _xMin, _yMin, _zMin };
    std::vector<double> p2 = { _xMax, _yMin, _zMin };
    std::vector<double> p3 = { _xMax, _yMax, _zMin };
    std::vector<double> p4 = { _xMin, _yMax, _zMin };
    std::vector<double> p5 = { _xMin, _yMin, _zMax };
    std::vector<double> p6 = { _xMax, _yMin, _zMax };
    std::vector<double> p7 = { _xMax, _yMax, _zMax };
    std::vector<double> p8 = { _xMin, _yMax, _zMax };

    std::vector<double> n1 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p1, p2, p3);
    std::vector<double> n2 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p1, p4, p3);
    std::vector<double> n3 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p1, p4, p8);
    std::vector<double> n4 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p1, p5, p8);
	std::vector<double> n5 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p1, p2, p6);
	std::vector<double> n6 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p1, p5, p6);
	std::vector<double> n7 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p2, p7, p6);
	std::vector<double> n8 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p2, p7, p3);
	std::vector<double> n9 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p5, p6, p7);
	std::vector<double> n10 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p5, p8, p7);
	std::vector<double> n11 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p3, p4, p8);
	std::vector<double> n12 = QualityAnalysis::QualityAnalysis::calcuateSingleTriangleNormal(p3, p7, p8);

    boundingBox.push_back({ p1, p2, p3, n1 });
	boundingBox.push_back({ p1, p4, p3, n2 });
	boundingBox.push_back({ p1, p4, p8, n3 });
	boundingBox.push_back({ p1, p5, p8, n4 });
	boundingBox.push_back({ p1, p2, p6, n5 });
	boundingBox.push_back({ p1, p5, p6, n6 });
	boundingBox.push_back({ p2, p7, p6, n7 });
	boundingBox.push_back({ p2, p7, p3, n8 });
	boundingBox.push_back({ p5, p6, p7, n9 });
	boundingBox.push_back({ p5, p8, p7, n10 });
	boundingBox.push_back({ p3, p4, p8, n11 });
	boundingBox.push_back({ p3, p7, p8, n12 });

    return boundingBox;
}

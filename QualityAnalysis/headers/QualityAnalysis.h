#pragma once

#include "Triangulation.h"

namespace QualityAnalysis {

	class QualityAnalysis
	{
		std::vector<double> negativePoint(std::vector<double> point);
		std::vector<double> convertPointToVector(Geometry::Point point, Geometry::Triangulation triangulation);
		double clamp(double value, double min, double max);
		double calculateDistanceBetweenPoints(std::vector<double> firstPoint, std::vector<double> secondPoint);
		double calculateSingleTriangleArea(Geometry::Triangle& triangle, Geometry::Triangulation triangulation);
		double calculateAngleBetweenVectors(std::vector<double> v1, std::vector<double> v2);
		double calculateSingleTriangleAspectRatio(Geometry::Triangle& triangle, Geometry::Triangulation triangulation);
		double calculateSingleTriangleInteriorAngle(Geometry::Triangle& triangle, Geometry::Triangulation triangulation);
		std::vector<double> calcuateSingleTriangleNormal(std::vector<double> v1, std::vector<double> v2, std::vector<double> v3);
	public:
		QualityAnalysis();
		~QualityAnalysis();
		double minX(Geometry::Triangulation triangulation);
		double minY(Geometry::Triangulation triangulation);
		double minZ(Geometry::Triangulation triangulation);
		double maxX(Geometry::Triangulation triangulation);
		double maxY(Geometry::Triangulation triangulation);
		double maxZ(Geometry::Triangulation triangulation);
		size_t numberOfTriangles(Geometry::Triangulation triangulation);
		size_t numberOfVertices(Geometry::Triangulation triangulation);
		double surfaceArea(Geometry::Triangulation triangulation);
		double triangleDensity(Geometry::Triangulation triangulation);
		double aspectRatio(Geometry::Triangulation triangulation);
		double orthogonality(Geometry::Triangulation triangulation);
		double objectLength(Geometry::Triangulation triangulation);
		double objectBreadth(Geometry::Triangulation triangulation);
		double objectHeight(Geometry::Triangulation triangulation);
		std::vector<std::vector<std::vector<double>>> drawBoundingBox(Geometry::Triangulation triangulation);
	};
}

#pragma once

#include "Triangulation.h"

class ModifiedTriangulation : public Geometry::Triangulation
{
public:
	double _minX;
	double _maxX;
	double _minY;
	double _maxY;
	double _minZ;
	double _maxZ;
};
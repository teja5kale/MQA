#pragma once

#include <string> 
#include "Triangulation.h"
#include <vector>
#include "Reader.h"

using namespace Geometry;

class OBJReader : public Reader
{
public:
	OBJReader();
	~OBJReader();

	void read(const std::string& fileName, Triangulation& tri);
	void helper(double xyz[3],std::vector<Point>&, std::map<double, int, OBJReader>&, Triangulation& triangulation);
	bool operator()(double a, double b) const;
};

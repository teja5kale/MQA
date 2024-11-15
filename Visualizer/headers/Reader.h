#pragma once
#include<string>
#include "Triangulation.h"

using namespace Geometry;

class Reader
{
	virtual void read(const std::string& fileName, Triangulation& triangulation) = 0;
};

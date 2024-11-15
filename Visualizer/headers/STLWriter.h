#pragma once
#include <string>
#include <vector>
#include "Writer.h"
#include "Triangle.h"
#include "Triangulation.h"

using namespace Geometry;

class STLWriter : public Writer
{
public:
    void Write(const std::string& filename, const Triangulation& triangulation, QProgressBar*);

private:
    std::string formulateText(Triangulation triangulation, Point point);
};
#pragma once
#include "Writer.h"
#include "Triangulation.h"

using namespace Geometry;

class DataWriter : public Writer
{
public:
    void Write(const std::string& filename, const Triangulation& triangulation);

private:
    std::string fomulateText(Triangulation triangulation, Point point);
};

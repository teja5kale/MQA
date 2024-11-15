#include "STLWriter.h"
#include<fstream>
#include<iomanip>

using namespace Geometry;

std::string STLWriter::formulateText(Triangulation triangulation, Point point)
{
    double x = triangulation.UniqueNumbers[point.X()];
    double y = triangulation.UniqueNumbers[point.Y()];
    double z = triangulation.UniqueNumbers[point.Z()];

    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "\n";
}

void STLWriter::Write(const std::string& filename, const Triangulation& triangulation, QProgressBar* progressBar)
{
    int count = 0;
    progressBar->setRange(0, triangulation.Triangles.size());
    std::ofstream outfile(filename);
    if (outfile.is_open())
    {
        outfile << "solid model" << std::endl;
        for (Triangle tri : triangulation.Triangles)
        {
            outfile << std::fixed << std::setprecision(6)
                << "    facet normal " << formulateText(triangulation, tri.Normal());
            outfile << "        outer loop\n";
            outfile << "            vertex " << formulateText(triangulation, tri.P1());
            outfile << "            vertex " << formulateText(triangulation, tri.P2());
            outfile << "            vertex " << formulateText(triangulation, tri.P3());
            outfile << "        endloop\n";
            outfile << "    endfacet\n";
            progressBar->setValue(count);
            count++;
        }
        outfile << "endsolid";
    }
}

#pragma once
#include<string>
#include<vector>
#include <QProgressBar>
#include"Triangulation.h"

using namespace Geometry;

class Writer
{
	virtual void Write(const std::string& filename, const Triangulation& trianglation, QProgressBar*) = 0;
};


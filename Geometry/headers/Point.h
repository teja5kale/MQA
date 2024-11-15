#pragma once

namespace Geometry
{

	class Point
	{
		int mX;
		int mY;
		int mZ;

	public:
		Point(int x, int y, int z);
		~Point();

		int X() const;
		int Y() const;
		int Z() const;

		bool operator<(const Point& other) const;
	};
}
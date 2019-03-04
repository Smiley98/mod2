#pragma once
struct m2AABB3
{
	m2AABB3() {}
	~m2AABB3() {}

	union
	{
		struct {
			float xMin, xMax, yMin, yMax, zMin, zMax;
		};

		struct {
			float values[6];
		};
	};
};
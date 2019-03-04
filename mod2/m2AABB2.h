#pragma once
struct m2AABB2
{
	m2AABB2() {}
	~m2AABB2() {}

	inline float width() { return xMax - xMin; }
	inline float height() { return yMax - yMin; }

	inline float operator[](unsigned int index) { return values[index]; }

	union
	{
		struct {
			float xMin, xMax, yMin, yMax;
		};

		struct {
			float values[4];
		};
	};
};
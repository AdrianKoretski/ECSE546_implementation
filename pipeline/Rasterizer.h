#pragma once

#include "FragmentShader.h"
#include "../scene_structures/PixelBuffer.h"
#include "../Definitions.h"

class Rasterizer
{
public:
	Rasterizer(PixelBuffer* pixel_buffer);
	void render(OBJ obj);
private:
	int m_buffer_height;
	int m_buffer_width;
	void interpolate(Point p0, Point p1, Point p2);
	std::vector<Point> m_interpolated_data;
	FragmentShader* m_fragment_shader;

	bool isContained(v3f p0, v3f p1, v3f p2, v2f p);

	v4f getMinMax(Point p, v4f min_max);
};
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

	void getMinMax(Point p);

	float min_x = 1;
	float max_x = -1;
	float min_y = 1;
	float max_y = -1;
};
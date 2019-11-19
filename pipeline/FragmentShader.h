#pragma once

#include <vector>
#include "../scene_structures/PixelBuffer.h"
#include "../Definitions.h"

class FragmentShader
{
public:
	FragmentShader(PixelBuffer* pixel_buffer);
	void render(std::vector<Point> data);
private:
	PixelBuffer* m_pixel_buffer;
};
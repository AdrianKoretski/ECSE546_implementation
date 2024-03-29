#pragma once

#include "../Definitions.h"
#include "../scene_structures/PixelBuffer.h"
#include "Rasterizer.h"

class VertexShader
{
public:
	VertexShader(PixelBuffer* pixel_buffer);
	void render(Scene scene);
private:
	Rasterizer* m_rasterizer;
};
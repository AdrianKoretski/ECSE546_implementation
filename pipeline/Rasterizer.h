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
	FragmentShader* m_fragment_shader;
};
#include "Rasterizer.h"
#include <vector>

Rasterizer::Rasterizer(PixelBuffer* pixel_buffer)
{
	m_fragment_shader = new FragmentShader(pixel_buffer);
}

void Rasterizer::render(OBJ obj)
{
	std::vector<Point> data;
	for (int i = 0; i < obj.VAO.size(); i++)		// TODO: This should interpolate values and generate new fragments.
		data.push_back(obj.VBO.at(obj.VAO.at(i)));
	m_fragment_shader->render(data);
}
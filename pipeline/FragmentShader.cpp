#include "FragmentShader.h"

FragmentShader::FragmentShader(PixelBuffer* pixel_buffer)
{
	m_pixel_buffer = pixel_buffer;
}

void FragmentShader::render(std::vector<Point> data)
{
	for (int i = 0; i < data.size(); i++)
	{
		int vp_x = data.at(i).position.x * 640;
		int vp_y = data.at(i).position.y * 480;

		m_pixel_buffer->setPixel(vp_x, vp_y, v3f(0, 1, 1));
	}
}
#include "FragmentShader.h"
#include "..//scene_structures/Texture2D.h"

FragmentShader::FragmentShader(PixelBuffer* pixel_buffer)
{
	m_pixel_buffer = pixel_buffer;
}

void FragmentShader::render(std::vector<Point> data)
{
	Texture2D t("asdf");
	for (int i = 0; i < data.size(); i++)
	{
		int vp_x = (data.at(i).position.x) * m_pixel_buffer->getWidth();
		int vp_y = (data.at(i).position.y) * m_pixel_buffer->getHeight();

		m_pixel_buffer->setPixel(vp_x, vp_y, t.sample(data.at(i).texture_coordinates));
	}
}
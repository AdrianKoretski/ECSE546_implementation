#include "FragmentShader.h"
#include "..//scene_structures/Texture2D.h"

FragmentShader::FragmentShader(PixelBuffer* pixel_buffer)
{
	m_pixel_buffer = pixel_buffer;
}

void FragmentShader::render(std::vector<Point> data)
{
	Texture2D t("foxx.bmp");
	for (int i = 0; i < data.size(); i++)
	{
		int vp_x = (data.at(i).position.x * 0.5 + 0.5) * m_pixel_buffer->getWidth();
		int vp_y = (data.at(i).position.y * 0.5 + 0.5) * m_pixel_buffer->getHeight();

		int z_depth = float(m_pixel_buffer->getDepthBufferSize()) / data.at(i).position.w;

		v2f asdf = data.at(i).texture_coordinates;


		m_pixel_buffer->setPixel(vp_x, vp_y, t.sample(data.at(i).texture_coordinates), m_pixel_buffer->getDepthBufferSize() - z_depth);
		//m_pixel_buffer->setPixel(vp_x, vp_y, data.at(i).color, m_pixel_buffer->getDepthBufferSize() - z_depth);
	}
}
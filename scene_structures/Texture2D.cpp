#include "Texture2D.h"

#pragma warning(disable : 4244) 

BMP* Texture2D::m_default;

Texture2D::Texture2D(std::string file_name, bool is_tiled)
{
	if (!m_image.ReadFromFile(file_name.c_str()))
	{
		std::cout << "\aERROR: Could not open texture " + file_name + "." << std::endl;
		m_image.SetSize(128, 128);
		for (int i = 0; i < 128; i++)
			for (int j = 0; j < 128; j++)
				m_image.SetPixel(i, j, RGBApixel{ 0, 0, 0, 255 });
		for (int i = 0; i < 16; i++)
			for (int j = 0; j < 16; j++)
				for (int ki = 0; ki < 4; ki++)
					for (int kj = 0; kj < 4; kj++)
					{
						m_image.SetPixel(i + 16 + ki * 32, j + kj * 32, RGBApixel{ 39, 127, 255, 255 });
						m_image.SetPixel(i + ki * 32, j + 16 + kj * 32, RGBApixel{ 39, 127, 255, 255 });
					}
	}
	loadDefault();
	m_is_tiled = is_tiled;
}

v3f Texture2D::sample(v2f position)
{
	BMP* sampled_image = &m_image;
	if (!m_is_tiled && (position.x < 0 || position.y < 0 || position.x >= 1 || position.y >= 1))
		sampled_image = m_default;
	while (position.x < 0)
		position.x += 1;
	while (position.y < 0)
		position.y += 1;
	while (position.x >= 1)
		position.x -= 1;
	while (position.y >= 1)
		position.y -= 1;

	int x_position = position.x * sampled_image->TellWidth();
	int y_position = position.y * sampled_image->TellHeight();

	auto a = sampled_image->GetPixel(x_position, y_position);
	
	v3f result = v3f(a.Red, a.Green, a.Blue) * (1.f / 255);

	return result;
}

void Texture2D::loadDefault()
{
	if (m_default != NULL)
		return;
	m_default = new BMP();
	m_default->SetSize(128, 128);
	for (int i = 0; i < 128; i++)
		for (int j = 0; j < 128; j++)
			m_default->SetPixel(i, j, RGBApixel{ 0, 0, 0, 255 });
	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 16; j++)
			for (int ki = 0; ki < 4; ki++)
				for (int kj = 0; kj < 4; kj++)
				{
					m_default->SetPixel(i + 16 + ki * 32, j + kj * 32, RGBApixel{ 39, 127, 255, 255 });
					m_default->SetPixel(i + ki * 32, j + 16 + kj * 32, RGBApixel{ 39, 127, 255, 255 });
				}
}

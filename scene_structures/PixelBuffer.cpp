#include "PixelBuffer.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>

#pragma warning(disable : 4996 4244 4838) //_CRT_SECURE_NO_WARNINGS

PixelBuffer::PixelBuffer(unsigned int width, unsigned int height, v3f background_color)
{
	m_data = (v3f*)malloc(width * height * sizeof(v3f));									// Set up the pixel buffer.
	m_z_buffer = (int*)malloc(width * height * sizeof(int));								// Set up the depth buffer.
	m_image.SetSize(width, height);															// Set up the output bmp.
	m_image.SetBitDepth(24);

	m_background = background_color;

	m_width = width;
	m_height = height;

	clearBuffer();
}

void PixelBuffer::clearBuffer()
{
	for (unsigned int i = 0; i < m_width; i++)
		for (unsigned int j = 0; j < m_height; j++)
		{
			m_data[i + j * m_width] = m_background;
			m_z_buffer[i + j * m_width] = m_depth_buffer_size;
		}
}

void PixelBuffer::setPixel(int x, int y, v3f color, int depth)
{
	if (x < 0 || y < 0 || x > int(m_width) || y > int(m_height))
		pixelOutOfBounds(x, y, "set");
	if (x == int(m_width))
		x = m_width - 1;
	if (y == int(m_height))
		y = m_height - 1;

	if (m_z_buffer[x + (m_height - y - 1) * m_width] <= depth)
		return;

	m_data[x + (m_height - y - 1) * m_width] = color;
	m_z_buffer[x + (m_height - y - 1) * m_width] = depth;
}

v3f PixelBuffer::getPixelColor(int x, int y)
{
	if (x < 0 || y < 0 || x > int(m_width) || y > int(m_height))
		pixelOutOfBounds(x, y, "get");
	if (x == int(m_width))
		x = m_width - 1;
	if (y == int(m_height))
		y = m_height - 1;
	v3f color = m_data[x + y * m_width];
	return color;
}

int PixelBuffer::getPixelDepth(int x, int y)
{
	if (x < 0 || y < 0 || x > int(m_width) || y > int(m_height))
		pixelOutOfBounds(x, y, "get");
	if (x == int(m_width))
		x = m_width - 1;
	if (y == int(m_height))
		y = m_height - 1;
	int depth = m_z_buffer[x + y * m_width];
	return depth;
}

void PixelBuffer::saveAs(std::string file_name)
{
	RGBApixel c;
	for (unsigned int i = 0; i < m_width; i++)
		for (unsigned int j = 0; j < m_height; j++)
		{
			c = { 
				ebmpBYTE(m_data[i + j * m_width].z * 255), 
				ebmpBYTE(m_data[i + j * m_width].y * 255), 
				ebmpBYTE(m_data[i + j * m_width].x * 255), 
				1 };
			m_image.SetPixel(i, j, c);
		}
	m_image.WriteToFile((file_name.append(".bmp")).c_str());
	std::cout << "File successfully saved as " << file_name << "." << std::endl;
}

int PixelBuffer::getWidth()
{
	return m_image.TellWidth();
}

int PixelBuffer::getHeight()
{
	return m_image.TellHeight();
}

int PixelBuffer::getDepthBufferSize()
{
	return m_depth_buffer_size;
}

void PixelBuffer::pixelOutOfBounds(int x, int y, std::string get_or_set)					// Error handling.
{
	std::cout << "\aERROR: Could not " << get_or_set << " pixel value";
	if (x < 0 || x >= int(m_width))
		std::cout << ", x coordinate (" << x << ") out of bounds";
	if (y < 0 || y >= int(m_height))
		std::cout << ", y coordinate (" << y << ") out of bounds";
	std::cout << ". Image size is " << m_width << " by " << m_height << ". (width by height)\n" << std::endl;

	std::cout << "Writing file anyway.\n" << std::endl;

	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", timeinfo);
	std::string str(buffer);

	saveAs("crash_" + str);

	std::cout << "Press enter key to exit." << std::endl;

	auto temp = getchar();

	exit(2);
}

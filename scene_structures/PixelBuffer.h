#pragma once

#include <string>
#include "../file_library/EasyBMP.h"
#include "../Definitions.h"

class PixelBuffer
{
public:
	PixelBuffer(unsigned int width, unsigned int height, v3f background_color = v3f(0));

	void clearBuffer();

	void setPixel(int x, int y, v3f color, int depth);

	v3f getPixelColor(int x, int y);
	int getPixelDepth(int x, int y);

	void saveAs(std::string file_name);

	int getWidth();
	int getHeight();

	int getDepthBufferSize();
private:
	v3f* m_data;
	int* m_z_buffer;

	int m_depth_buffer_size = 65536;

	BMP m_image;

	unsigned int m_width;
	unsigned int m_height;

	v3f m_background;

	void pixelOutOfBounds(int x, int y, std::string get_or_set);
};


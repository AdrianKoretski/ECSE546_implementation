#pragma once

#include <string>
#include "EasyBMP.h"
#include "../Definitions.h"

class FileWriter
{
public:
	FileWriter(unsigned int width, unsigned int height, v3f background_color = v3f(0));

	void setPixel(int x, int y, v3f color);

	v3f getPixel(int x, int y);

	void saveAs(std::string file_name);
private:
	v3f* m_data;

	BMP m_image;

	unsigned int m_width;
	unsigned int m_height;

	void pixelOutOfBounds(int x, int y, std::string get_or_set);
};


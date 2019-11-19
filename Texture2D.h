#pragma once

#include <string>

#include "Definitions.h"
#include "file_library/EasyBMP.h"

class Texture2D
{
public:
	Texture2D(std::string file_name, bool is_tiled = false);
	v3f sample(v2f position);
private:
	static void loadDefault();

	static BMP* m_default;
	BMP m_image;
	bool m_is_tiled;
};


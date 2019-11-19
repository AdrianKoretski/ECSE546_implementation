#include <iostream>
#include "file_library/EasyBMP.h"
#include "PixelBuffer.h"
#include "Definitions.h"
#include "Texture2D.h"

int main()
{
	std::cout << "hello rasterization engine" << std::endl;

	PixelBuffer f(640, 480);

	/*for (int i = 0; i < 640; i++)
		for (int j = 0; j < 480; j++)
			f.setPixel(i, j, v3f(float(i) / 640, 1 - float(j) / 480, 0));
	for (int i = 0; i < 640; i++)
		for (int j = 0; j < 480; j++)
			f.setPixel(i, j, v3f(1) - f.getPixel(i, j));*/

	int xs = 640;
	int ys = 480;

	Texture2D t("fox.bmp", true);
	for (int i = 0; i < xs; i++)
		for (int j = 0; j < ys; j++)
			f.setPixel(i, j, t.sample(v2f(i * 2.f/xs, j * 2.f/ys)));


	f.saveAs("test");


}
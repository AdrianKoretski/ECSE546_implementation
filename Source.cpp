#include <iostream>
#include "file_library/FileWriter.h"
#include "Definitions.h"

int main()
{
	std::cout << "hello rasterization engine" << std::endl;


	FileWriter f(640, 480);

	for (int i = 0; i < 640; i++)
		for (int j = 0; j < 480; j++)
			f.setPixel(i, j, v3f(float(i) / 640, 1 - float(j) / 480, 0));
	for (int i = 0; i < 640; i++)
		for (int j = 0; j < 480; j++)
			f.setPixel(i, j, v3f(1) - f.getPixel(i, j));

	f.saveAs("test");
}
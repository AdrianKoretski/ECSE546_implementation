#include <iostream>
#include <vector>
#include "pipeline/VertexShader.h"
#include "file_library/EasyBMP.h"
#include "scene_structures/PixelBuffer.h"
#include "Definitions.h"
#include "scene_structures/Texture2D.h"

std::vector<Point> vertex_buffer_object;

int main()
{
	std::cout << "hello rasterization engine" << std::endl;

	PixelBuffer f(640, 480);

	std::vector<Point> VBO;
	std::vector<int> VAO;
	glm::mat4 transform_matrix(0);

	Point p0;
	Point p1;
	Point p2;
	Point p3;
	Point p4;
	Point p5;
	Point p6;
	Point p7;
	Point p8;
	p0.position = v4f(-1, 1, 0, 1);
	p1.position = v4f(0, 1, 0, 1);
	p2.position = v4f(1, 1, 0, 1);
	p3.position = v4f(-1, 0, 0, 1);
	p4.position = v4f(0, 0, 0, 1);
	p5.position = v4f(1, 0, 0, 1);
	p6.position = v4f(-1, -1, 0, 1);
	p7.position = v4f(0, -1, 0, 1);
	p8.position = v4f(1, -1, 0, 1);


	VBO.push_back(p0);
	VBO.push_back(p1);
	VBO.push_back(p2);
	VBO.push_back(p3);
	VBO.push_back(p4);
	VBO.push_back(p5);
	VBO.push_back(p6);
	VBO.push_back(p7);
	VBO.push_back(p8);

	VAO.push_back(1);
	VAO.push_back(0);
	VAO.push_back(4);
	VAO.push_back(0);
	VAO.push_back(3);
	VAO.push_back(4);

	VAO.push_back(4);
	VAO.push_back(5);
	VAO.push_back(2);
	VAO.push_back(4);
	VAO.push_back(2);
	VAO.push_back(1);

	VAO.push_back(4);
	VAO.push_back(3);
	VAO.push_back(6);
	VAO.push_back(4);
	VAO.push_back(6);
	VAO.push_back(7);

	VAO.push_back(4);
	VAO.push_back(7);
	VAO.push_back(8);
	VAO.push_back(4);
	VAO.push_back(8);
	VAO.push_back(5);


	OBJ first{VBO, VAO, transform_matrix};

	VertexShader vs(&f);

	vs.render(first);

	f.saveAs("test");
}
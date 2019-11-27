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

	PixelBuffer f(640, 480, v3f(0.0,0.15,0.3));
	float theta = 0;

	std::vector<Point> VBO;
	std::vector<int> VAO;
	glm::mat4 transform_matrix(
		cos(theta), 0, -sin(theta), 0, 
		0, 1, 0, 0, 
		sin(theta), 0, cos(theta), 0,
		0, 0, 0, 1);



	Point p0;
	Point p1;
	Point p2;
	Point p3;
	Point p4;
	Point p5;
	Point p6;
	Point p7;
	Point p8;
	p0.position = v4f(-2, 2, -1, 1);
	p0.color = v3f(1, 0, 0);
	p0.texture_coordinates = v3f(0, 0, 0);
	p1.position = v4f(2, 2, -1, 1);
	p1.color = v3f(0, 1, 0);
	p1.texture_coordinates = v3f(1, 0, 0);

	p2.position = v4f(2, 2, 0, 1);
	p2.color = v3f(0, 0, 1);

	p3.position = v4f(-2, -2, 0, 1);
	p3.color = v3f(1, 1, 0);
	p3.texture_coordinates = v3f(0, 1, 0);
	p4.position = v4f(2, -2, 0, 1);
	p4.color = v3f(1, 0, 1);
	p4.texture_coordinates = v3f(1, 1, 0);

	p5.position = v4f(1, 0, 0, 1);
	p5.color = v3f(0, 1, 1);
	p6.position = v4f(-0.5, -0.5, 0, 1);
	p6.color = v3f(1, 1, 1);
	p7.position = v4f(0, -0.5, 0, 1);
	p7.color = v3f(1, 0, 0);
	p8.position = v4f(0.5, -0.5, 0, 1);
	p8.color = v3f(0, 0, 1);


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

	/*VAO.push_back(4);
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
	VAO.push_back(5);*/


	OBJ first{VBO, VAO, transform_matrix};

	VertexShader vs(&f);

	Camera cam;
	cam.position = v3f(0, 0, 2);
	cam.generate_matrix();

	Scene scene;

	scene.perspective_matrix = cam.perspective_matrix * cam.camera_matrix;

	scene.obj.push_back(first);

	vs.render(scene);

	f.saveAs("test");

	/*for (int count = 0; count < 628; count++)
	{
		scene.obj.at(0).transform_matrix = glm::mat4(
			cos(theta), 0, -sin(theta), 0,
			0, 1, 0, 0,
			sin(theta), 0, cos(theta), 0,
			0, 0, 0, 1
		);
		f.clearBuffer();
		vs.render(scene);
		f.saveAs("test" + std::to_string(count));
		theta += 0.01;
	}*/
}
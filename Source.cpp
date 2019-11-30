#include <iostream>
#include <vector>
#include "pipeline/VertexShader.h"
#include "file_library/EasyBMP.h"
#include "scene_structures/PixelBuffer.h"
#include "Definitions.h"
#include "scene_structures/Texture2D.h"

std::vector<Point> vertex_buffer_object;
bool use_texture = false;
bool use_correct_interpolation = true;

int main()
{
	std::cout << "hello rasterization engine" << std::endl;

	PixelBuffer f(1920, 1080, v3f(0.0,0.15,0.3));
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

	float epsilon = -0.000007f;

	p0.position = v4f(-1.5, 2, -3, 1);
	p1.position = v4f(1.5, 2, -3, 1);
	p2.position = v4f(-2.5, 1.5, -2.5 - epsilon, 1);
	p3.position = v4f(2.5, 1.5, -2.5 - epsilon, 1);
	p4.position = v4f(-2.5, -1.5, 0.5 - epsilon, 1);
	p5.position = v4f(2.5, -1.5, 0.5 - epsilon, 1);
	p6.position = v4f(-1.5, -2, 1, 1);
	p7.position = v4f(1.5, -2, 1, 1);

	p0.color = v3f(0, 1, 0);
	p1.color = v3f(0, 1, 0);

	p2.color = v3f(0, 0, 1);
	p3.color = v3f(0, 0, 1);
	p4.color = v3f(0, 0, 1);
	p5.color = v3f(0, 0, 1);

	p6.color = v3f(0, 1, 0);
	p7.color = v3f(0, 1, 0);

	VBO.push_back(p0);
	VBO.push_back(p1);
	VBO.push_back(p2);
	VBO.push_back(p3);
	VBO.push_back(p4);
	VBO.push_back(p5);
	VBO.push_back(p6);
	VBO.push_back(p7);

	VAO.push_back(2);
	VAO.push_back(3);
	VAO.push_back(4);

	VAO.push_back(3);
	VAO.push_back(5);
	VAO.push_back(4);

	VAO.push_back(0);
	VAO.push_back(1);
	VAO.push_back(6);

	VAO.push_back(1);
	VAO.push_back(7);
	VAO.push_back(6);

	OBJ first{VBO, VAO, transform_matrix};

	VertexShader vs(&f);

	Camera cam;
	cam.position = v3f(0, 0, 3);
	cam.generate_matrix();

	Scene scene;

	scene.perspective_matrix = cam.perspective_matrix * cam.camera_matrix;

	first.transform_matrix = glm::mat4(
		cos(0.5), 0, sin(0.5), 0,
		0, 1, 0, 0,
		-sin(0.5), 0, cos(0.5), 0,
		0, 0, 0, 1
	);
	scene.obj.push_back(first);

	vs.render(scene);

	f.saveAs("z_fighting");

	VBO.clear();
	VAO.clear();
	transform_matrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	scene.obj.clear();
	f.clearBuffer();


	p0.position = v4f(-3, 3, -1, 1);
	p1.position = v4f(3, 3, -1, 1);
	p2.position = v4f(-3, -3, 0, 1);
	p3.position = v4f(3, -3, 0, 1);

	p0.texture_coordinates = v4f(0, 1, 0, 1);
	p1.texture_coordinates = v4f(1, 1, 0, 1);
	p2.texture_coordinates = v4f(0, 0, 0, 1);
	p3.texture_coordinates = v4f(1, 0, 0, 1);

	VBO.push_back(p0);
	VBO.push_back(p1);
	VBO.push_back(p2);
	VBO.push_back(p3);

	VAO.push_back(0);
	VAO.push_back(1);
	VAO.push_back(3);

	VAO.push_back(2);
	VAO.push_back(0);
	VAO.push_back(3);

	OBJ second{ VBO, VAO, transform_matrix };
	

	use_texture = true;

	scene.obj.push_back(second);
	vs.render(scene);

	f.saveAs("interpolation_good");

	use_correct_interpolation = false;
	f.clearBuffer();
	vs.render(scene);

	f.saveAs("interpolation_bad");

	VBO.clear();
	VAO.clear();
	transform_matrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	scene.obj.clear();
	f.clearBuffer();

	use_texture = false;
	use_correct_interpolation = true;

	p0.position = v4f(-3, 3, 0, 1);
	p1.position = v4f(-3, 2, 0, 1);
	p2.position = v4f(3, -1, -0.1, 1);

	p3.position = v4f(3, 3, 0, 1);
	p4.position = v4f(3, 2, 0, 1);
	p5.position = v4f(-3, -3, -0.1, 1);

	p6.position = v4f(-2, -3, 0, 1);
	p7.position = v4f(-1, -3, 0, 1);
	p8.position = v4f(-1.5, 3, -0.1, 1);


	p0.color = v3f(1, 0.5, 0.5);
	p1.color = v3f(1, 0.5, 0.5);
	p2.color = v3f(0.5, 0, 0);

	p3.color = v3f(0.5, 1, 0.5);
	p4.color = v3f(0.5, 1, 0.5);
	p5.color = v3f(0, 0.5, 0);

	p6.color = v3f(0.5, 0.5, 1);
	p7.color = v3f(0.5, 0.5, 1);
	p8.color = v3f(0, 0, 0.5);

	VBO.push_back(p0);
	VBO.push_back(p1);
	VBO.push_back(p2);
	VBO.push_back(p3);
	VBO.push_back(p4);
	VBO.push_back(p5);
	VBO.push_back(p6);
	VBO.push_back(p7);
	VBO.push_back(p8);

	VAO.push_back(0);
	VAO.push_back(1);
	VAO.push_back(2);
	VAO.push_back(3);
	VAO.push_back(4);
	VAO.push_back(5);
	VAO.push_back(6);
	VAO.push_back(7);
	VAO.push_back(8);

	OBJ third{ VBO, VAO, transform_matrix };

	scene.obj.push_back(third);
	vs.render(scene);

	f.saveAs("z_buffer_demo");
}
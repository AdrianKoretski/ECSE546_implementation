#pragma once

#include <vector>
#include <iostream>
#include "include/glm/glm.hpp"

#define v4f glm::vec4
#define vec4f glm::vec4
#define v3f glm::vec3
#define vec3f glm::vec3
#define v2f glm::vec2
#define vec2f glm::vec2

//class Texture2d;

struct Point
{
	unsigned int vertex_ID;
	v4f position;
	v3f color;
	v3f normal;
	v2f texture_coordinates;
};

struct OBJ
{
	std::vector<Point> VBO;
	std::vector<int> VAO;
	glm::mat4 transform_matrix;
	unsigned int texture_ID;
};

struct Camera
{
	float aspect_ratio = 1.78f;
	float fov = 2.f;
	v3f position = v3f(0, 0, 2);
	v3f up = v3f(0, 1, 0);
	v3f at = v3f(0, 0, -1);
	bool is_project = false;
	glm::mat4 camera_matrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4 perspective_matrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	void generate_matrix()
	{
		at = glm::normalize(at);
		up = glm::normalize(up);
		v3f side = glm::normalize(glm::cross(at, up));
		camera_matrix = glm::mat4(
			side.x, side.y, side.z, 0,
			up.x, up.y, up.z, 0,
			at.x, at.y, at.z, 0,
			position.x, position.y, position.z, 1
		);

		camera_matrix = glm::inverse(camera_matrix);

		v3f delta;
		delta.y = tan(fov / 2);
		delta.x = delta.y * aspect_ratio;
		delta.z = 1;

		float l = - delta.x;
		float r = + delta.x;
		float b = - delta.y;
		float t = + delta.y;
		float n = 1;
		float f = -1;

		perspective_matrix = glm::mat4(
			2.f / (r - l),		0,					0,					0,
			0,					2.f / (t - b),		0,					0,
			0,					0,					2.f / (n - f),		0,
			0,					0,					0,					1
		);

		perspective_matrix = glm::mat4(
			2 * n / (r - l),	0,					(l + r) / (l - r),	0,
			0,					2 * n / (t - b),	(b + t) / (b - t),	0,
			0,					0,					(f + n) / (n - f),	2*f*n/(f-n),
			0,					0,					1,					0
		);

		perspective_matrix = glm::transpose(perspective_matrix);
	}
};

struct Scene
{
	std::vector<OBJ> obj;
	glm::mat4 perspective_matrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
};

#pragma once

#include <vector>
#include "include/glm/glm.hpp"

#define v4f glm::vec4
#define v3f glm::vec3
#define v2f glm::vec2

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
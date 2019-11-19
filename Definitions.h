#pragma once

#include "include/glm/glm.hpp"

#define v4f glm::vec4
#define v3f glm::vec3
#define v2f glm::vec2

class Texture2D;

struct vertex
{
	v4f position;
	v3f color;
	v3f normal;
	v2f texture;
	Texture2D* texture;
};
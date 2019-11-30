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

struct BarycentricInterpolation
{
	glm::mat4 rotation_matrix;


	v2f pre_w0;
	v2f pre_w1;
	v2f pre_w2;
	float denominator;
	v2f p2;

	float weight_0;
	float weight_1;
	float weight_2;

	void precompute(v4f p0, v4f p1, v4f p2)
	{
		denominator =
			(p1.y - p2.y)
			* (p0.x - p2.x)
			+ (p2.x - p1.x)
			* (p0.y - p2.y);
		pre_w0 = v2f(
			p1.y - p2.y,
			p2.x - p1.x
		);

		pre_w1 = v2f(
			p2.y - p0.y,
			p0.x - p2.x
		);
		this->p2 = p2;



		/*v3f p_0 = v3f(p0.x, p0.y, 1.f / p0.w);
		v3f p_1 = v3f(p1.x, p1.y, 1.f / p1.w);
		v3f p_2 = v3f(p2.x, p2.y, 1.f / p2.w);


		v3f t = glm::normalize(p_1 - p_0);
		v3f n = glm::normalize(glm::cross(t, p_2 - p_0));
		v3f u = glm::normalize(glm::cross(t, n));

		rotation_matrix = glm::mat4{
			t.x, t.y, t.z, 0, 
			u.x, u.y, u.z, 0, 
			n.x, n.y, n.z, 0,
			p_0.x, p_0.y, p_0.z, 1
		};

		rotation_matrix = glm::inverse(rotation_matrix);

		v4f p__0 = v4f(p_0, 1);
		v4f p__1 = v4f(p_1, 1);
		v4f p__2 = v4f(p_2, 1);
		std::cout << p__0.x << " " << p__0.y << " " << p__0.z << " " << p__0.w << std::endl;
		p__0 = rotation_matrix * p__0;
		std::cout << p__0.x << " " << p__0.y << " " << p__0.z << " " << p__0.w << std::endl << std::endl;
		std::cout << p__1.x << " " << p__1.y << " " << p__1.z << " " << p__1.w << std::endl;
		p__1 = rotation_matrix * p__1;
		std::cout << p__1.x << " " << p__1.y << " " << p__1.z << " " << p__1.w << std::endl << std::endl;
		std::cout << p__2.x << " " << p__2.y << " " << p__2.z << " " << p__2.w << std::endl;
		p__2 = rotation_matrix * p__2;
		std::cout << p__2.x << " " << p__2.y << " " << p__2.z << " " << p__2.w << std::endl << std::endl;*/
	}

	void computeWeights(v2f point)
	{
		weight_0 = glm::dot(pre_w0, point - p2) / denominator;
		weight_1 = glm::dot(pre_w1, point - p2) / denominator;
		weight_2 = 1 - weight_1 - weight_0;
	}

	bool isPointValid()
	{
		if (weight_0 < 0 || weight_1 < 0 || weight_2 < 0)
			return false;
		return true;
	}

	void interpolate(Point value_0, Point value_1, Point value_2, Point& point) {
		point.color = value_0.color * weight_0 + value_1.color * weight_1 + value_2.color * weight_2;
		point.normal = value_0.normal * weight_0 + value_1.normal * weight_1 + value_2.normal * weight_2;
		point.texture_coordinates = value_0.texture_coordinates * weight_0 + value_1.texture_coordinates * weight_1 + value_2.texture_coordinates * weight_2;
	}
};
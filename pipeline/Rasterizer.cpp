#include "Rasterizer.h"
#include <vector>

Rasterizer::Rasterizer(PixelBuffer* pixel_buffer)
{
	m_fragment_shader = new FragmentShader(pixel_buffer);
	m_buffer_height = pixel_buffer->getHeight();
	m_buffer_width = pixel_buffer->getWidth();
}

void Rasterizer::render(OBJ obj)
{
	m_interpolated_data.clear();
	for (int i = 0; i < obj.VAO.size(); i += 3)
	{
		interpolate(
			obj.VBO.at(obj.VAO.at(i + 0)),
			obj.VBO.at(obj.VAO.at(i + 1)),
			obj.VBO.at(obj.VAO.at(i + 2))
		);
	}
	m_fragment_shader->render(m_interpolated_data);
}

void Rasterizer::interpolate(Point p0, Point p1, Point p2)
{
	float max_x = -1;
	float min_x = 1;
	float max_y = -1;
	float min_y = 1;

	Point p = p0;				// TODO: fix this jank.

	if (p.position.x > max_x)
		max_x = p.position.x;
	if (p.position.x < min_x)
		min_x = p.position.x;
	if (p.position.y > max_y)
		max_y = p.position.y;
	if (p.position.y < min_y)
		min_y = p.position.y;

	p = p1;

	if (p.position.x > max_x)
		max_x = p.position.x;
	if (p.position.x < min_x)
		min_x = p.position.x;
	if (p.position.y > max_y)
		max_y = p.position.y;
	if (p.position.y < min_y)
		min_y = p.position.y;

	p = p2;

	if (p.position.x > max_x)
		max_x = p.position.x;
	if (p.position.x < min_x)
		min_x = p.position.x;
	if (p.position.y > max_y)
		max_y = p.position.y;
	if (p.position.y < min_y)
		min_y = p.position.y;

	v2f w_0;
	v2f w_1;
	v2f p_0 = v2f(p0.position) * 0.5f + 0.5f;
	v2f p_1 = v2f(p1.position) * 0.5f + 0.5f;
	v2f p_2 = v2f(p2.position) * 0.5f + 0.5f;

	float denominator =
		(p_1.y - p_2.y)
		* (p_0.x - p_2.x)
		+ (p_2.x - p_1.x)
		* (p_0.y - p_2.y);

	w_0 = v2f(
		p_1.y - p_2.y,
		p_2.x - p_1.x
	);

	w_1 = v2f(
		p_2.y - p_0.y,
		p_0.x - p_2.x
	);

	min_x = min_x / 2 + 0.5f;
	max_x = max_x / 2 + 0.5f;
	min_y = min_y / 2 + 0.5f;
	max_y = max_y / 2 + 0.5f;


	float d_w = 1.f / m_buffer_width;
	float d_h = 1.f / m_buffer_height;

	for (float i = float(int(min_x * m_buffer_width)) / m_buffer_width + d_w/2; i <= max_x; i += d_w)	// TODO: This is janky. It might need to change.
	{
		for (float j = float(int(min_y * m_buffer_height)) / m_buffer_height + d_h/2; j <= max_y; j += d_h)
		{
			float w0 = glm::dot(w_0, v2f(i, j) - v2f(p_2)) / denominator;
			float w1 = glm::dot(w_1, v2f(i, j) - v2f(p_2)) / denominator;
			float w2 = 1 - w0 - w1;
			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			//if (isContained(p0.position, p1.position, p2.position, v2f(i * 2 - 1, j * 2 - 1)))
			{
				Point new_point;
				new_point.position = v4f(i, j, 0, 1);
				new_point.color = p0.color * w0 + p1.color * w1 + p2.color * w2;
				new_point.texture_coordinates = p0.texture_coordinates * w0 + p1.texture_coordinates * w1 + p2.texture_coordinates * w2;
				m_interpolated_data.push_back(new_point);
			}
		}
	}
}

bool Rasterizer::isContained(v3f p0, v3f p1, v3f p2, v2f p)		// TODO: optimise.
{
	bool i0 = false;
	bool i1 = false;
	bool i2 = false;

	v3f v(p1.x - p0.x, p1.y - p0.y, 0);
	v3f e(p.x - p0.x, p.y - p0.y, 0);
	if (glm::cross(v, e).z < 0)
		i0 = true;
	v = v3f(p2.x - p1.x, p2.y - p1.y, 0);
	e = v3f(p.x - p1.x, p.y - p1.y, 0);
	if (glm::cross(v, e).z < 0)
		i1 = true;
	v = v3f(p0.x - p2.x, p0.y - p2.y, 0);
	e = v3f(p.x - p2.x, p.y - p2.y, 0);
	if (glm::cross(v, e).z < 0)
		i2 = true;
	if ((i0 && i1 && i2) || !(i0 || i1 || i2))
		return true;
	return false;
}
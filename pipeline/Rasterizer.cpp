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

void Rasterizer::getMinMax(Point p)
{
	if (p.position.x > max_x)
		max_x = p.position.x;
	if (p.position.x < min_x)
		min_x = p.position.x;
	if (p.position.y > max_y)
		max_y = p.position.y;
	if (p.position.y < min_y)
		min_y = p.position.y;
}

void Rasterizer::interpolate(Point p0, Point p1, Point p2)
{
	min_x = 1;
	max_x = -1;
	min_y = 1;
	max_y = -1;

	getMinMax(p0);
	getMinMax(p1);
	getMinMax(p2);

	BarycentricInterpolation barry;
	BarycentricInterpolation harry;				// TODO: fix this. Fails when the triangle is coplainar to the x or y planes.

	harry.precompute(p0.position * (1.f / p0.position.w), p1.position * (1.f / p1.position.w), p2.position * (1.f / p2.position.w));
	barry.precompute(p0.position, p1.position, p2.position);

	float d_w = 2.f / m_buffer_width;
	float d_h = 2.f / m_buffer_height;

	for (int i = min_x * m_buffer_width - 1; i <= max_x * m_buffer_width; i++)
	{
		for (int j = min_y * m_buffer_height - 1; j <= max_y * m_buffer_height; j++)
		{
			barry.computeWeights(v2f((float(i) + 0.5) / m_buffer_width, (float(j) + 0.5) / m_buffer_height));
			if (barry.isPointValid())
			{
				Point new_point;
				new_point.position = v4f(float(i) / m_buffer_width, float(j) / m_buffer_height, 0, 1);
				float w =
						p0.position.w * barry.weight_0 +
						p1.position.w * barry.weight_1 +
						p2.position.w * barry.weight_2;

				new_point.position.w = w;

				harry.computeWeights(v2f((float(i) + 0.5) / m_buffer_width, (float(j) + 0.5) / m_buffer_height) * (1.f / w));

				new_point.color = 
					p0.color * harry.weight_0 + 
					p1.color * harry.weight_1 + 
					p2.color * harry.weight_2;

				new_point.texture_coordinates = 
					p0.texture_coordinates * harry.weight_0 + 
					p1.texture_coordinates * harry.weight_1 + 
					p2.texture_coordinates * harry.weight_2;

				m_interpolated_data.push_back(new_point);
			}
		}
	}
}

bool Rasterizer::isContained(v3f p0, v3f p1, v3f p2, v2f p)		// Old depricated solution.
{
	bool i0 = false;
	bool i1 = false;
	bool i2 = false;

	v3f v(p1.x - p0.x, p1.y - p0.y, 0);
	v3f e(p.x - p0.x, p.y - p0.y, 0);
	if (glm::cross(v, e).z <= 0)
		i0 = true;
	v = v3f(p2.x - p1.x, p2.y - p1.y, 0);
	e = v3f(p.x - p1.x, p.y - p1.y, 0);
	if (glm::cross(v, e).z <= 0)
		i1 = true;
	v = v3f(p0.x - p2.x, p0.y - p2.y, 0);
	e = v3f(p.x - p2.x, p.y - p2.y, 0);
	if (glm::cross(v, e).z <= 0)
		i2 = true;
	if ((i0 && i1 && i2) || !(i0 || i1 || i2))
		return true;
	return false;
}
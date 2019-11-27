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

v4f Rasterizer::getMinMax(Point p, v4f min_max)
{
	// min_max: x = min_x	y = max_x	z = min_y	w = max_y

	if (p.position.x > min_max.y)
		min_max.y = p.position.x;
	if (p.position.x < min_max.x)
		min_max.x = p.position.x;
	if (p.position.y > min_max.w)
		min_max.w = p.position.y;
	if (p.position.y < min_max.z)
		min_max.z = p.position.y;
	return min_max;
}

void Rasterizer::interpolate(Point p0, Point p1, Point p2)
{
	v4f min_max = v4f(1, -1, 1, -1);

	min_max = getMinMax(p0, min_max);
	min_max = getMinMax(p1, min_max);
	min_max = getMinMax(p2, min_max);

	BarycentricInterpolation barry;
	BarycentricInterpolation harry;

	harry.precompute(p0.position * p0.position.w, p1.position * p1.position.w, p2.position * p2.position.w);
	barry.precompute(p0.position, p1.position, p2.position);

	float d_w = 2.f / m_buffer_width;
	float d_h = 2.f / m_buffer_height;

	for (float i = min_max.x + d_w/2; i <= min_max.y; i += d_w)
	{
		for (float j = min_max.z + d_h/2; j <= min_max.w; j += d_h)
		{
			barry.computeWeights(v2f(i, j));
			if (barry.isPointValid())
			{
				Point new_point;
				new_point.position = v4f(i, j, 0, 1);
				float w =
					1.f / (
						1.f / p0.position.w * barry.weight_0 +
						1.f / p1.position.w * barry.weight_1 +
						1.f / p2.position.w * barry.weight_2);

				new_point.position.w = w;

				harry.computeWeights(v2f(i, j) * w);

				new_point.color = 
					p0.color * barry.weight_0 + 
					p1.color * barry.weight_1 + 
					p2.color * barry.weight_2;
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
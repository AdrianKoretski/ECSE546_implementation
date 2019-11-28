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
	/*p0.position.x += 1;
	p0.position.y += 1;
	p1.position.x += 1;
	p1.position.y += 1;
	p2.position.x += 1;
	p2.position.y += 1;*/
	v4f min_max = v4f(1, -1, 1, -1);

	min_max = getMinMax(p0, min_max);
	min_max = getMinMax(p1, min_max);
	min_max = getMinMax(p2, min_max);

	/*min_max.x = int((m_buffer_width / 2) * min_max.x) - 1;
	min_max.x = min_max.x / (m_buffer_width / 2);
	min_max.y = int((m_buffer_width / 2) * min_max.y) + 1;
	min_max.y = min_max.y / (m_buffer_width / 2);

	min_max.z = int((m_buffer_height / 2) * min_max.z) - 1;
	min_max.z = min_max.z / (m_buffer_height / 2);
	min_max.w = int((m_buffer_height / 2) * min_max.w) + 1;
	min_max.w = min_max.w / (m_buffer_height / 2);*/

	//min_max = min_max + v4f(1.f / m_buffer_width, 1.f / m_buffer_width, 1.f / m_buffer_height, 1.f / m_buffer_height);

	BarycentricInterpolation barry;
	BarycentricInterpolation harry;

	harry.precompute(p0.position * p0.position.w, p1.position * p1.position.w, p2.position * p2.position.w);
	barry.precompute(p0.position, p1.position, p2.position);

	float d_w = 2.f / m_buffer_width;
	float d_h = 2.f / m_buffer_height;

	for (int i = min_max.x * m_buffer_width - 1; i <= min_max.y * m_buffer_width; i++)
	{
		for (int j = min_max.z * m_buffer_height - 1; j <= min_max.w * m_buffer_height; j++)
		{
			barry.computeWeights(v2f((float(i) + 0.5) / m_buffer_width, (float(j) + 0.5) / m_buffer_height));
			if (barry.isPointValid()/* || isContained(p0.position, p1.position, p2.position, v2f(i, j))*/)
			{
				Point new_point;
				new_point.position = v4f(float(i) / m_buffer_width, float(j) / m_buffer_height, 0, 1);
				float w =
					1.f / (
						1.f / p0.position.w * barry.weight_0 +
						1.f / p1.position.w * barry.weight_1 +
						1.f / p2.position.w * barry.weight_2);

				new_point.position.w = w;

				harry.computeWeights(v2f((float(i) + 0.5) / m_buffer_width, (float(j) + 0.5) / m_buffer_height) * w);

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
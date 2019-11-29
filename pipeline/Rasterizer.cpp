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
				float w =
						p0.position.w * barry.weight_0 +
						p1.position.w * barry.weight_1 +
						p2.position.w * barry.weight_2;
				new_point.position = v4f(float(i) / m_buffer_width, float(j) / m_buffer_height, 1.f / w, w);

				harry.computeWeights(v2f((float(i) + 0.5) / m_buffer_width, (float(j) + 0.5) / m_buffer_height) * new_point.position.z);

				harry.interpolate(p0, p1, p2, new_point);

				m_interpolated_data.push_back(new_point);
			}
		}
	}
}
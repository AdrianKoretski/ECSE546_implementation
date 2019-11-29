#include "VertexShader.h"

VertexShader::VertexShader(PixelBuffer* pixel_buffer)
{
	m_rasterizer = new Rasterizer(pixel_buffer);
}

void VertexShader::render(Scene scene)		// TODO: This should take the points and transform them according to the transform matrix.
{
	for (int i = 0; i < scene.obj.size(); i++)
	{
		for (int j = 0; j < scene.obj.at(i).VBO.size(); j++)
		{
			scene.obj.at(i).VBO.at(j).position = scene.perspective_matrix * scene.obj.at(i).transform_matrix * scene.obj.at(i).VBO.at(j).position;
			float w = scene.obj.at(i).VBO.at(j).position.w;
			scene.obj.at(i).VBO.at(j).position = scene.obj.at(i).VBO.at(j).position / w;
			scene.obj.at(i).VBO.at(j).position.w = 1.f / w;
		}
		m_rasterizer->render(scene.obj.at(i));
	}
}

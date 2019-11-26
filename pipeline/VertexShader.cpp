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
			std::cout << scene.obj.at(i).VBO.at(j).position.x << " " << scene.obj.at(i).VBO.at(j).position.y << " " << scene.obj.at(i).VBO.at(j).position.z << " " << scene.obj.at(i).VBO.at(j).position.w << std::endl;
			scene.obj.at(i).VBO.at(j).position = scene.perspective_matrix * scene.obj.at(i).transform_matrix * scene.obj.at(i).VBO.at(j).position;
			std::cout << scene.obj.at(i).VBO.at(j).position.x << " " << scene.obj.at(i).VBO.at(j).position.y << " " << scene.obj.at(i).VBO.at(j).position.z << " " << scene.obj.at(i).VBO.at(j).position.w << std::endl;
			float w = scene.obj.at(i).VBO.at(j).position.w;
			scene.obj.at(i).VBO.at(j).position = scene.obj.at(i).VBO.at(j).position / w;
			scene.obj.at(i).VBO.at(j).position.w = w;
			std::cout << scene.obj.at(i).VBO.at(j).position.x << " " << scene.obj.at(i).VBO.at(j).position.y << " " << scene.obj.at(i).VBO.at(j).position.z << " " << scene.obj.at(i).VBO.at(j).position.w << std::endl << std::endl;
		}
		m_rasterizer->render(scene.obj.at(i));
	}
}

#include "VertexShader.h"

VertexShader::VertexShader(PixelBuffer* pixel_buffer)
{
	m_rasterizer = new Rasterizer(pixel_buffer);
}

void VertexShader::render(OBJ obj)		// TODO: This should take the points and transform them according to the transform matrix.
{
	m_rasterizer->render(obj);
}

#include "Renderer.h"

void Renderer::Clear()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // bg
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.Bind();
	ib.Bind();
	shader.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

}

#pragma once

#include <glad/glad.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"


class Renderer
{
public:
	void Clear();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
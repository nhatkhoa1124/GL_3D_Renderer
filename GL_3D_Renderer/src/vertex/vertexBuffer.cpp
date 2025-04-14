#include "vertexBuffer.h"


namespace Vertex {
	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &mVBO);
	}

	VertexBuffer::~VertexBuffer() {}

	const void VertexBuffer::bindVertexBuffer(const std::vector<VertexData>& vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}

	const void VertexBuffer::deleteVertexBuffer()
	{
		glDeleteBuffers(1, &mVBO);
	}

}
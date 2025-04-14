#include "vertexBuffer.h"


namespace Vertex {
	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &mVBO);
	}

	VertexBuffer::~VertexBuffer() {}

	void VertexBuffer::bindVertexBuffer(const std::vector<VertexData>& vertices) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}

	void VertexBuffer::deleteVertexBuffer() const
	{
		glDeleteBuffers(1, &mVBO);
	}

}
#include "vertexArray.h"

namespace Vertex {

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &mVAO);
	}

	VertexArray::~VertexArray() {}

	const void VertexArray::bindVertexArray(GLuint vbo)
	{
		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		GLsizei stride = sizeof(VertexData);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, color));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, texCoord));
		glEnableVertexAttribArray(3);
	}

	const void VertexArray::deleteVertexArray() {
		glDeleteVertexArrays(1, &mVAO);
	}

}
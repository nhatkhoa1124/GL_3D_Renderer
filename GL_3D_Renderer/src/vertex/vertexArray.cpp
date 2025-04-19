#include "vertexArray.h"

namespace Vertex {

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &mVAO);
	}

	VertexArray::~VertexArray() {}

	void VertexArray::bindVertexArray(GLuint vbo) const
	{
		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		GLsizei stride = sizeof(VertexData);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, texCoord));

	}

	void VertexArray::deleteVertexArray() const {
		glDeleteVertexArrays(1, &mVAO);
	}

}
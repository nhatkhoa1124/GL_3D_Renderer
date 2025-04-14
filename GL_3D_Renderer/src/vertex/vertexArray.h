#pragma once

#include "vertex.h"

namespace Vertex {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		const void bindVertexArray(GLuint vbo);
		const void deleteVertexArray();
		inline const GLuint getVertexArrayObject() { return mVAO; };
	private:
		GLuint mVAO;
	};
}
#pragma once

#include "vertex.h"

namespace Vertex {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		void bindVertexArray(GLuint vbo) const;
		void deleteVertexArray() const;
		inline GLuint getVertexArrayObject() const { return mVAO; };
	private:
		GLuint mVAO;
	};
}
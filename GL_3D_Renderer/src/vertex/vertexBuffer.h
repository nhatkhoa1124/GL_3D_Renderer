#pragma once

#include "vertex.h"

namespace Vertex {
	class VertexBuffer {
	public:
		VertexBuffer();
		~VertexBuffer();
		const void bindVertexBuffer(const std::vector<VertexData>& vertices);
		const void deleteVertexBuffer();
		inline const GLuint getVertexBufferObject() { return mVBO; };
	private:
		GLuint mVBO;
	};
}
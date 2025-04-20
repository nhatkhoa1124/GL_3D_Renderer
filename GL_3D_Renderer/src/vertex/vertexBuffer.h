#pragma once

#include "vertex.h"

namespace Model {
	class VertexBuffer {
	public:
		VertexBuffer();
		~VertexBuffer();
		void bindVertexBuffer(const std::vector<VertexData>& vertices)const;
		void deleteVertexBuffer()const;
		inline GLuint getVertexBufferObject() const { return mVBO; };
	private:
		GLuint mVBO;
	};
}
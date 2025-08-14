#pragma once

#include "vertex.h"

namespace Model {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		void bindVertexArray(uint32_t vbo) const;
		void deleteVertexArray() const;
		inline uint32_t getVertexArrayObject() const { return mVAO; };
	private:
		uint32_t mVAO;
	};
}
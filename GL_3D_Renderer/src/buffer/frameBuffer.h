#pragma once

#include <cstdint>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer() = default;
	void bindFrameBuffer();
	void unbindFrameBuffer();
	void deleteFrameBuffer();
	void attachTexture();
	void bindRenderBuffer();
	void unbindRenderBuffer();
private:
	uint32_t mFBO;
	uint32_t mRBO;
};
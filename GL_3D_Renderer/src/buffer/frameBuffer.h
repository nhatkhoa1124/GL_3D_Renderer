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
	void deleteRenderBuffer();
	inline uint32_t getTexture() const { return mTexBuffer; };
private:
	uint32_t mFBO;
	uint32_t mRBO;
	uint32_t mTexBuffer;
};
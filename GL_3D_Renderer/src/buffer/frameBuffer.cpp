#include "frameBuffer.h"

#include "core.h"

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void FrameBuffer::bindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void FrameBuffer::unbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::deleteFrameBuffer()
{
	glDeleteFramebuffers(1, &mFBO);
}

void FrameBuffer::attachTexture()
{
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
}

void FrameBuffer::bindRenderBuffer()
{
	glGenRenderbuffers(1, &mRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO);
}

void FrameBuffer::unbindRenderBuffer()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
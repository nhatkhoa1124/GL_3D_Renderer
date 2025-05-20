#include "frameBuffer.h"

#include "core.h"
#include <iostream>

FrameBuffer::FrameBuffer()
	: mFBO(0), mRBO(0), mTexBuffer(0)
{
	// Generate framebuffer
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	// Create and attach texture
	glGenTextures(1, &mTexBuffer);
	glBindTexture(GL_TEXTURE_2D, mTexBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexBuffer, 0);

	// Create renderbuffer for depth + stencil
	glGenRenderbuffers(1, &mRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO);

	// Check framebuffer completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind
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
	if (mFBO) {
		glDeleteFramebuffers(1, &mFBO);
		mFBO = 0;
	}
	if (mTexBuffer) {
		glDeleteTextures(1, &mTexBuffer);
		mTexBuffer = 0;
	}
}

void FrameBuffer::attachTexture()
{
	glGenTextures(1, &mTexBuffer);
	glBindTexture(GL_TEXTURE_2D, mTexBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexBuffer, 0);
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

void FrameBuffer::deleteRenderBuffer()
{
	if (mRBO) {
		glDeleteRenderbuffers(1, &mRBO);
		mRBO = 0;
	}
}
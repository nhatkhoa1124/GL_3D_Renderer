#pragma once

#include <cstdint>

enum class ShadowType
{
	DIRECTIONAL,
	POINT,
	SPOT
};

class Shadow
{
public:
	Shadow();

	void init();
	void drawDepthMap();
	int getWidth() const { return SHADOW_WIDTH; };
	int getHeight() const { return SHADOW_HEIGHT; };
	uint32_t getFramebuffer() const { return mFBO; };
	uint32_t getDepthmap() const { return mDepthMap; };

private:
	uint32_t mFBO;
	uint32_t mDepthMap;
	const int SHADOW_WIDTH = 1024;
	const int SHADOW_HEIGHT = 1024;
};
#pragma once

#include <cstdint>
#include <vector>
#include <string>

class CubeMap
{
public:
	CubeMap();
	~CubeMap() = default;
	void loadTexture(std::vector<std::string> faces, bool setFlipTexture);
	void drawCube();
	inline uint32_t getTextureId() const { return mTextureId; };
private:
	uint32_t mTextureId;
	uint32_t mCubeVAO, mCubeVBO;
};
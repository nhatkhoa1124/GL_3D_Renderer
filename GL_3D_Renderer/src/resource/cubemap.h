#pragma once

#include <cstdint>
#include <vector>
#include <string>

class CubeMap
{
public:
	CubeMap() = default;
	~CubeMap() = default;
	void loadTexture(std::vector<std::string> faces, bool setFlipTexture);
	inline uint32_t getTextureId() const { return mTextureId; };
private:
	uint32_t mTextureId;
};
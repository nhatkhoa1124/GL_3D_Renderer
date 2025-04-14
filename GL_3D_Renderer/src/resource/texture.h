#pragma once

class Texture {
public:
	Texture();
	~Texture();
	void loadTexture(const char* texPath);
	inline void destroyTexture() const { glDeleteTextures(1, &mTextureId); };
	inline void bindTexture() const { glBindTexture(GL_TEXTURE_2D, mTextureId); };
private:
	GLuint mTextureId;
};
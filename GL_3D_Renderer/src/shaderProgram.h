#pragma once

#include "core.h"

class ShaderProgram {
public:
	ShaderProgram(const std::string& vsPath, const std::string& fsPath);
	~ShaderProgram();
	std::string loadShaderSource(const std::string& shaderPath);
	const void useProgram();
	inline const GLuint getProgramId() { return mShaderId; };
private:
	GLuint mShaderId;
};
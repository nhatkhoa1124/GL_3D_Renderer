#pragma once

class ShaderProgram {
public:
	ShaderProgram(const std::string& vsPath, const std::string& fsPath);
	~ShaderProgram();
	std::string loadShaderSource(const std::string& shaderPath);
	void useProgram();
	inline GLuint getProgramId()  const { return mShaderId; };
private:
	GLuint mShaderId;
};
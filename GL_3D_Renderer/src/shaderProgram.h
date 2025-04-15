#pragma once

#include <iostream>

class ShaderProgram {
public:
	ShaderProgram(const std::string& vsPath, const std::string& fsPath);
	~ShaderProgram();
	std::string loadShaderSource(const std::string& shaderPath);
	void useProgram();
	inline GLuint getProgramId()  const { return mShaderId; };

	inline void setUniformMat4(const glm::mat4& value, const char* uniformName) const
	{
		GLuint location = glGetUniformLocation(mShaderId, uniformName);
		if (location == (GLuint)-1) {
			std::cerr << "WARNING: UNIFORM'" << uniformName << "' NOT FOUND IN SHADER" << std::endl;
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	};
	inline void setUniformVec3(const glm::vec3& value, const char* uniformName) const
	{
		GLuint location = glGetUniformLocation(mShaderId, uniformName);
		if (location == (GLuint)-1) {
			std::cerr << "WARNING: UNIFORM'" << uniformName << "' NOT FOUND IN SHADER" << std::endl;
			return;
		}
		glUniform3fv(location, 1, glm::value_ptr(value));
	};
	inline void setUniformFloat(const float value, const char* uniformName) const
	{
		GLuint location = glGetUniformLocation(mShaderId, uniformName);
		if (location == (GLuint)-1) {
			std::cerr << "WARNING: UNIFORM'" << uniformName << "' NOT FOUND IN SHADER" << std::endl;
			return;
		}
		glUniform1f(location, value);
	};
	inline void setMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
	{
		setUniformMat4(model, "model");
		setUniformMat4(view, "view");
		setUniformMat4(projection, "projection");
	};
private:
	GLuint mShaderId;
};
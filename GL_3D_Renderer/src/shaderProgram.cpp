#include <fstream>
#include <sstream>
#include <iostream>

#include "core.h"
#include "shaderProgram.h"

ShaderProgram::ShaderProgram(const std::string& vsPath, const std::string& fsPath)
{
	std::string vertexSource = loadShaderSource(vsPath);
	std::string fragmentSource = loadShaderSource(fsPath);

	const char* vertexCode = vertexSource.c_str();
	const char* fragmentCode = fragmentSource.c_str();

	int success;
	char infoLog[512];

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, nullptr);
	glCompileShader(vertexShader);
	// Debug vertex shader compilation
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "FAILED COMPILING VERTEX SHADER:\t" << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
	glCompileShader(fragmentShader);
	// Debug fragment shader compilation
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "FAILED COMPILING VERTEX SHADER:\t" << infoLog << std::endl;
	}

	mShaderId = glCreateProgram();
	glAttachShader(mShaderId, vertexShader);
	glAttachShader(mShaderId, fragmentShader);
	glLinkProgram(mShaderId);
	// debug errors
	glGetProgramiv(mShaderId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mShaderId, 512, nullptr, infoLog);
		std::cout << "FAILED LINKING SHADER PROGRAM:\t" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
};

ShaderProgram::~ShaderProgram() {}

std::string ShaderProgram::loadShaderSource(const std::string& shaderPath)
{
	std::ifstream file{ shaderPath };
	std::stringstream buffer;
	if (!file.is_open()) {
		std::cerr << "ERROR OPENING SHADER FILE\n";
		return "";
	}

	buffer << file.rdbuf();
	return buffer.str();
}

void ShaderProgram::useProgram()
{
	glUseProgram(mShaderId);
}
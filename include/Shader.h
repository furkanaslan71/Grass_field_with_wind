#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"

class Shader
{
public:
	GLuint m_Program;

	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
	~Shader();
	void Use();
	void Load(const char* vertexPath, const char* fragmentPath);
	void Load(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

	// Uniform setters
	void SetUniform1i(const std::string& name, int i);
	void SetUniform1f(const std::string& name, float f);
	void SetUniform2i(const std::string& name, int i1, int i2);
	void SetUniform3f(const std::string& name, const glm::vec3& vector);
	void SetUniformMat4fv(const std::string& name, const glm::mat4& matrix);
	void SetUniformBool(const std::string& name, float f);


private:
	void checkCompileErrors(GLuint shader, std::string type);

};
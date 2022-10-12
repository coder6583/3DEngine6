#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>

class Shader
{
	public:
		GLuint Program;
		Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
		Shader();
		void Use();
	private:
		std::string name;
};
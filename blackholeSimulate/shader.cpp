#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>

std::string loadFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::in);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + filename);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

GLuint compileShader(const std::string& source, GLenum type)
{
	// ������ɫ������
	GLuint shader = glCreateShader(type);

	// ������ɫ��Դ����
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	// ������ɫ��
	glCompileShader(shader);

	// �������Ƿ�ɹ�
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// ��ȡ���������־�ĳ���
		GLint length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		// �����ڴ�洢������־
		std::vector<char> log(length);
		// ��ȡ���������־
		glGetShaderInfoLog(shader, length, &length, log.data());
		// ���������־����׼������
		std::cerr << "Shader compilation failed: " << log.data() << std::endl;
		// ɾ����ɫ������
		glDeleteShader(shader);
		// �׳��쳣��ʾ����ʧ��
		throw std::runtime_error("Failed to compile the shader.");
	}
	// ���ر���ɹ�����ɫ������
	return shader;
}

GLuint createShader(const std::string& vertCode, const std::string& fragCode)
{
	std::cout << "compiling " << vertCode << std::endl;
	GLuint vertShader = compileShader(loadFile(vertCode), GL_VERTEX_SHADER);
	std::cout << "compiling " << fragCode << std::endl;
	GLuint fragShader = compileShader(loadFile(fragCode), GL_FRAGMENT_SHADER);
	// �����������
	GLuint program = glCreateProgram();
	// ������ɫ�����󵽳������
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	// ���ӳ������
	glLinkProgram(program);
	// ��������Ƿ�ɹ�
	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		// ��ȡ���Ӵ�����־�ĳ���
		GLint length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		// �����ڴ�洢������־
		std::vector<char> log(length);
		// ��ȡ���Ӵ�����־
		glGetProgramInfoLog(program, length, &length, log.data());
		// ���������־����׼������
		std::cerr << "Program linking failed: " << log.data() << std::endl;
		// ɾ���������
		glDeleteProgram(program);
		// �׳��쳣��ʾ����ʧ��
		throw std::runtime_error("Failed to link the program.");
	}
	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}
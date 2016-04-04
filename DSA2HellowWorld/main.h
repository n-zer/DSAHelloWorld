#ifndef _MAIN
#define _MAIN

#define GLEW_STATIC
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "CameraSingleton.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

//3 for position, 3 for color,
//4 vertices in total
GLfloat vertices[] = {
	0.5f,0.5f,0.0f, 1.0f, 0.0f, 0.0f,
	0.5f,-0.5f,0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f,-0.5f,0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f,0.5f,0.0f, 1.0f, 1.0f, 1.0f
};

//10 squares total, at these locations
glm::vec3 objPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

//we're drawing a square, so we need two triangles using the original vertices
GLuint indices[] = {
	0,1,3,
	1,2,3
};

void key_callback(GLFWwindow* window, int key, int scandcode, int action, int mode);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void doMovement();

GLuint GenerateShaderProgram(const char* vertPath, const char* fragPath);

GLint CheckShader(GLuint shader);

GLint CheckShaderProgram(GLuint program);

std::string ReadFile(const char* filePath);

#endif // !_MAIN
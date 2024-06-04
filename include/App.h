#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "Skybox.h"
#include "Object.h"



struct WindowState
{
	WindowState() {};
	WindowState(int _width, int _height, std::string _title) :width(_width), height(_height), title(_title) {}
	int width;
	int height;
	std::string title;
	float fps;
};


class App
{
public:
	App();
	~App();
	void Init();
	void initPerlin();
	void Run();


	//void Update(float deltaTime);
	void Render();

	//void Restart();

	// Utility functions
	void ProcessInput(float deltaTime);
	void PrepareScene();
	//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	glm::vec2 lastPosition;
	glm::vec2 newPosition;

private:

	GLFWwindow* m_Window;
	WindowState m_WindowState;
	bool mouseInitialized = false;
	std::string path;
	const char* filePath;

	std::shared_ptr<Camera> m_Camera;


	Skybox m_Skybox;
	Object m_Object;
	Object m_ground;
	GLuint perlinNoise = 0;

	float bladeHeight = 0.5;
	float color = 1.0;
	float windStrength = 0.05;

	float exposure;
	int specularOn;
	float kd;
	float ks;
	float kt;
	float km;
	int mode;

};
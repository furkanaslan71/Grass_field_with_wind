#pragma once

#ifndef STB_IMAGE
#define STB_IMAGE
#include <stb_image.h>
#endif

#include <Mesh.h>
#include <Shader.h>
#include <Camera.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 



class Skybox
{
public:
	Skybox();

	~Skybox() {};

	void SetupSkybox(const char* filePath);
	void Render(Camera& camera);

private:
	float* m_data;
	//std::shared_ptr<Cube> m_Cube;
	Mesh m_SkyboxCube;
	Shader m_SkyboxShader;
	unsigned int hdrTexture;
	unsigned int cubeMapTexture;
};
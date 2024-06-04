#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include <memory>


class Object
{
public:
	Object();

	void Render(std::shared_ptr<Camera> camera, float bladeHeight, float color, float wind, float time, int perlinNoise);
	void RenderGround(std::shared_ptr<Camera> camera);
	void Load();
	void LoadGround();
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Mesh> m_Mesh;

};
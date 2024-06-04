#include "Object.h"


Object::Object()
{}

void Object::Render(std::shared_ptr<Camera> camera, float bladeHeight, float color, float wind, float time, int perlinNoise)
{
	m_Shader->Use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 6.0f, -6.0f));
	model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
	glm::vec3 rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	m_Shader->SetUniformMat4fv("model", model);
	m_Shader->SetUniformMat4fv("view", camera->m_ViewMatrix);
	m_Shader->SetUniformMat4fv("projection", camera->m_ProjectionMatrix);
	m_Shader->SetUniform3f("eyePos", camera->m_Position);

	m_Shader->SetUniform1f("bladeHeight", bladeHeight);
	m_Shader->SetUniform1f("color", color);
	m_Shader->SetUniform1i("perlin", perlinNoise);
	m_Shader->SetUniform1f("wind", wind);
	m_Shader->SetUniform1f("time", time);
	m_Mesh->Draw();
}

void Object::RenderGround(std::shared_ptr<Camera> camera)
{
	m_Shader->Use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 6.0f, -6.0f));
	model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
	glm::vec3 rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	m_Shader->SetUniformMat4fv("model", model);
	m_Shader->SetUniformMat4fv("view", camera->m_ViewMatrix);
	m_Shader->SetUniformMat4fv("projection", camera->m_ProjectionMatrix);
	m_Shader->SetUniform3f("eyePos", camera->m_Position);
	m_Mesh->Draw();
}

void Object::Load()
{
	m_Shader = std::make_unique<Shader>("assets/shaders/objectvert.glsl", "assets/shaders/objectfrag.glsl", "assets/shaders/geometry.glsl");
	m_Mesh = std::make_unique<Mesh>("assets/model/surface.obj");
}

void Object::LoadGround()
{
	m_Shader = std::make_unique<Shader>("assets/shaders/groundvert.glsl", "assets/shaders/groundfrag.glsl");
	m_Mesh = std::make_unique<Mesh>("assets/model/surface.obj");
}



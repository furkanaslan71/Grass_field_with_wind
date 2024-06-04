#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#define M_PI       3.14159265358979323846

class Camera
{
public:
	Camera();
	Camera(int screenWidth, int screenHeight);

	void UpdateProjectionMatrix(int width, int height);
	void UpdateViewMatrix(glm::vec3 position, glm::vec3 lookingPoint, glm::vec3 up);
	void ProcessMouseMovement(float xoffset, float yoffset, float deltaTime);
	void processKeyboard(GLFWwindow* window,float deltaTime);

	float yaw = 0.0f;
	float pitch = 0.0f;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::vec3 m_Position;
	glm::vec3 m_Up;
	glm::vec3 m_Front;
private:
	unsigned int m_MoveSpeed;
};
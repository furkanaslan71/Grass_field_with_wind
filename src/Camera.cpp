#include "Camera.h"
#include <string>


Camera::Camera()
{
    // Set up view matrix
    m_MoveSpeed = 3;
    m_Position = glm::vec3(0.61, 10.04, -0.90);
    m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_ViewMatrix = glm::lookAt(m_Position, m_Front, m_Up);

    // Construct projection matrix 100px x 100px as default.
    UpdateProjectionMatrix(100, 100);
}

Camera::Camera(int screenWidth, int screenHeight)
{
    // Set up view matrix
    m_MoveSpeed = 3;
    m_Position = glm::vec3(0.61, 10.04, -0.90);
    m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    // Set up projection matrix
    UpdateViewMatrix(m_Position, m_Front, m_Up);
    UpdateProjectionMatrix(screenWidth, screenHeight);
}

void Camera::UpdateProjectionMatrix(int screenWidth, int screenHeight)
{
    float fovyRad = (float)(90.0 / 180.0) * M_PI;
    m_ProjectionMatrix = glm::perspective(fovyRad, screenWidth / (float)screenHeight, 0.1f, 200.0f);
}

void Camera::UpdateViewMatrix(glm::vec3 position, glm::vec3 lookingPoint, glm::vec3 up)
{
    m_Position = position;
    m_Front = lookingPoint;
    m_Up = up;
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, float deltaTime)
{

    yaw += xoffset * m_MoveSpeed * deltaTime;
    pitch -= yoffset * m_MoveSpeed * deltaTime;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    m_Front = glm::normalize(front + m_Front);

    glm::vec3 right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_Up = glm::normalize(glm::cross(right, m_Front) + m_Up);

    UpdateViewMatrix(m_Position, m_Front, m_Up);

}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        glm::vec3 direction = m_Front;
        /*printf("W: %f, %f, %f\n", m_Front.x, m_Front.y, m_Front.z);*/
        glm::vec3 newPos = m_Position + m_MoveSpeed * deltaTime * direction ;
        UpdateViewMatrix(newPos, m_Front, m_Up);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::cross(m_Front, m_Up);
        /*printf("A: %f, %f, %f\n", direction.x, direction.y, direction.z);*/
        glm::vec3 newPos = m_Position - m_MoveSpeed * deltaTime * direction;
        UpdateViewMatrix(newPos, m_Front, m_Up);

    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        glm::vec3 direction = m_Front;
        glm::vec3 newPos = m_Position - m_MoveSpeed * deltaTime * direction;
        UpdateViewMatrix(newPos, m_Front, m_Up);
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::cross(m_Front, m_Up);
        glm::vec3 newPos = m_Position + m_MoveSpeed * deltaTime * direction;
        UpdateViewMatrix(newPos, m_Front, m_Up);
    }
    /*if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        printf("Position: %f, %f, %f", m_Position.x, m_Position.y, m_Position.z);
    }*/

}

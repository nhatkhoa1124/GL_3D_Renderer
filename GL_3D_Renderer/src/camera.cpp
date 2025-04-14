#include <iostream>

#include "core.h"
#include "camera.h"


Camera::Camera(glm::vec3 startPos) :
	mPos{ startPos },
	worldUp{ glm::vec3(0.0f, 1.0f, 0.0f) },
	mYaw{ -90.0f },
	mPitch{ 0.0f },
	mMoveSpeed{ 4.0f },
	mMouseSens{ 0.05f },
	mUp{ glm::vec3(0.0f, 1.0f, 0.0f) },
	mFront{ glm::vec3(0.0f, 0.0f, -1.0f) },
	mRight{ glm::vec3(1.0f, 0.0f, 0.0f) }
{
	updateCamera();
}

Camera::~Camera() {}

void Camera::processCameraMovement(float xOffset, float yOffset)
{
	xOffset *= mMouseSens;
	yOffset *= mMouseSens;

	mYaw += xOffset;
	mPitch += yOffset;

	if (mPitch > 89.0f) {
		mPitch = 89.0f;
	}
	if (mPitch < -89.0f) {
		mPitch = 89.0f;
	}

	updateCamera();
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
	if (window == nullptr) {
		std::cerr << "PROCESSKEYBOARD::NOWINDOW\n";
	}
	float velocity = mMoveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		mPos += mFront * velocity;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		mPos += mRight * velocity;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		mPos -= mFront * velocity;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		mPos -= mRight * velocity;
	}
}

void Camera::updateCamera()
{
	glm::vec3 look;
	look.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	look.y = sin(glm::radians(mPitch));
	look.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

	mFront = glm::normalize(look);
	mRight = glm::normalize(glm::cross(mFront, worldUp));
	mUp = glm::normalize(glm::cross(mFront, mRight));

}
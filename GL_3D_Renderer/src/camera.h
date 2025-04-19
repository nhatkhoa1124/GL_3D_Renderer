#pragma once

class Camera {
public:
	Camera(glm::vec3 startPos);
	~Camera() = default;
	void processCameraMovement(float xOffset, float yOffset);
	void processKeyboard(GLFWwindow* window, float deltaTime);
	void updateCamera();
	inline glm::vec3 getPos() const { return mPos; };
	inline glm::mat4 getViewMatrix() const { return glm::lookAt(mPos, mPos + mFront, mUp); };
	inline void setPosition(glm::vec3 pos) { mPos = pos; };
private:
	glm::vec3 mPos;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mFront;
	glm::vec3 worldUp;

	float mYaw;
	float mPitch;
	const float mMoveSpeed;
	const float mMouseSens;
};
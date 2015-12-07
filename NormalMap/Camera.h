#pragma once
#include <glm\glm.hpp>

class Camera
{

public:
	glm::vec3 position;
	glm::vec3 viewDirection;
	const glm::vec3 UP;
	glm::vec2 oldMousePosition;
	//glm::vec3 getPosition();
	//static float const MOVEMENT_SPEED;

	Camera();
	//glm::vec3 getPosition();
	void mouseUpdate(const glm::vec2& newMousePosition);
	glm::mat4 getWorldToViewMatrix() const;

	void Camera::moveForward();
	void Camera::moveBackward();
	void Camera::strafeLeft();
	void Camera::strafeRight();
	void Camera::moveUp();
	void Camera::moveDown();

/*	glm::vec3 position;
	glm::vec3 viewDirection;
	const glm::vec3 Up;
	glm::vec2 oldMousePosition;
	static const float MOVEMENT_SPEED;
	glm::vec3 strafeDirection;

public:
	Camera();
	glm::mat4 getWorldToViewMatrix() const;
	void mouseUpdate(const glm::vec2 & newMousePosition);
	void moveForward();
	void strafeLeft();
	void moveBackwrd();
	void strafeRight();
	void moveUp();
	void moveDown();
	glm::vec3 getPosition() const;*/
};


#include "Camera.h"
#include <glm\gtx\transform.hpp>

#include "Camera.h"
#include <glm\gtx\transform.hpp>

const float MOVEMENT_SPEED = 0.1f;

Camera::Camera() : viewDirection(0.0f, 0.0f, -1.0f), UP(0.0f, 1.0f, 0.0f)
{
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	//the if statement is to fix the Camera jumping
	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}

	const float ROTATIONAL_SPEED = 0.5f;
	//horizontal and vertical update
	glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
	glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATIONAL_SPEED, UP) *
		glm::rotate(-mouseDelta.y * ROTATIONAL_SPEED, toRotateAround);

	viewDirection = glm::mat3(rotator) * viewDirection;

	oldMousePosition = newMousePosition;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}

void Camera::moveForward()
{
	position += MOVEMENT_SPEED * viewDirection;
}

void Camera::moveBackward()
{
	position += -MOVEMENT_SPEED * viewDirection;
}

void Camera::strafeLeft()
{
	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position += -MOVEMENT_SPEED * strafeDirection;
}
void Camera::strafeRight()
{
	glm::vec3 strafeDirection = glm::cross(viewDirection, UP);
	position += MOVEMENT_SPEED * strafeDirection;
}
void Camera::moveUp()
{
	position += MOVEMENT_SPEED * UP;
}
void Camera::moveDown()
{
	position += -MOVEMENT_SPEED * UP;
}

/*
const float Camera::MOVEMENT_SPEED = 0.1f;

Camera::Camera() : viewDirection(0.0675045103, -0.435228437, -0.897777438), Up(0.0f, 1.0f, 0.0f),
position(0.304028988, 1.36063778, 5.18686485)
{
}

void Camera::mouseUpdate(const glm::vec2 &newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 10.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	else
	{
		const float ROTATIONAL_SPEED = 0.1f;
		strafeDirection = glm::cross(viewDirection, Up);
		glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATIONAL_SPEED, Up) *
			glm::rotate(-mouseDelta.y * ROTATIONAL_SPEED, strafeDirection);
		
		viewDirection = glm::mat3(rotator) * viewDirection;

		oldMousePosition = newMousePosition;
	}
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, Up);
}

void Camera::moveForward()
{
	position += MOVEMENT_SPEED * viewDirection;
}

void Camera::strafeLeft()
{
	position += -MOVEMENT_SPEED * strafeDirection;
}
void Camera::moveBackwrd()
{
	position += -MOVEMENT_SPEED * viewDirection;
}
void Camera::strafeRight()
{
	position += MOVEMENT_SPEED * strafeDirection;
}
void Camera::moveUp()
{
	position += MOVEMENT_SPEED * Up;
}
void Camera::moveDown()
{
	position += -MOVEMENT_SPEED * Up;
}

glm::vec3 Camera::getPosition() const
{
	return position;
}*/
#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), fov(FOV), mouseSensitivity(SENSITIVITY)
{
	this -> position = position;
	this -> worldUp = up;
	this -> yaw = yaw;
	this -> pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV)
{
	this -> position = glm::vec3(posX, posY, posZ);
	this -> worldUp = glm::vec3(upX, upY, upZ);
	this -> yaw = yaw;
	this -> pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(this -> position, this -> position + this -> front, this -> up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	float lastYPos = position.y;
	if(direction == FORWARD)
	{
		position += front * velocity;
		position.y = lastYPos;
	}
	if(direction == BACKWARD)
	{
		position -= front * velocity;
		position.y = lastYPos;
	}
	if(direction == LEFT)
	{
		position -= right * velocity;
		position.y = lastYPos;
	}
	if(direction == RIGHT)
	{
		position += right * velocity;
		position.y = lastYPos;
	}
	if(direction == UP)
	{
		position.y += velocity;
	}
	if(direction == DOWN/* && position.y  - velocity >= 0.5f*/)
	{
		position.y -= velocity;
	}
}

void Camera::processMouseScroll(float yoffset)
{
	fov -= (float)yoffset;
	if(fov < 1.0f)
		fov = 1.0f;
	if(fov > 45.0f)
		fov = 45.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = -sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this -> front = glm::normalize(front);
	
	this -> right = glm::normalize(glm::cross(this -> front, this -> worldUp));
	this -> up = glm::normalize(glm::cross(this -> right, this -> front));
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;
	
	this -> yaw += xoffset;
	this -> pitch += yoffset;
	if(constrainPitch)
	{
		if(pitch > 89.0f)
			pitch = 89.0f;
		if(pitch < -89.0f)
			pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::changeSpeed(float speed)
{
	this -> movementSpeed = speed; 
}

void Camera::changeSensitivity(float sensitivity)
{
	this -> mouseSensitivity = sensitivity;
}




















#include "Camera.hpp"

Camera::Camera(float _radius) {
	radius = _radius;
}

void Camera::updateCameraPos(float x, float y) {
	float sensitivity = 0.25f;
	float xoffset = x * sensitivity;
	float yoffset = y * sensitivity;

	yaw += xoffset;
	pitch -= yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 target = glm::vec3(0.0f);

	position.x = target.x + radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	position.y = target.y + radius * sin(glm::radians(pitch));
	position.z = target.z + radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	view = glm::lookAt(position, glm::vec3(0.0f), up);
}
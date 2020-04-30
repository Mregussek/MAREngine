
/*
#include "Camera.h"

void Camera::processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) processKeyboard(CameraMovement::FORWARD);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) processKeyboard(CameraMovement::BACKWARD);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) processKeyboard(CameraMovement::LEFT);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) processKeyboard(CameraMovement::RIGHT);
}

void Camera::processInput(GLFWwindow* window, SerialPortMonitor& spm) {
	if (spm.isConnected()) {
		std::cout << "x: " << spm.getX() << std::endl;
		std::cout << "y: " << spm.getY() << std::endl;
		std::cout << "z: " << spm.getZ() << std::endl;
	}

	processInput(window);
}

void Camera::processKeyboard(CameraMovement direction) {
	float velocity = _movementSpeed * _deltaTime;

	if (direction == CameraMovement::FORWARD) _position += _front * velocity;
	if (direction == CameraMovement::BACKWARD) _position -= _front * velocity;
	if (direction == CameraMovement::LEFT) _position -= _right * velocity;
	if (direction == CameraMovement::RIGHT) _position += _right * velocity;
}

void Camera::processMouseMovement(float x_offset, float y_offset, GLboolean constrainPitch) {
	x_offset *= _mouseSensitivity;
	y_offset *= _mouseSensitivity;

	_yaw += x_offset;
	_pitch += y_offset;

	if (constrainPitch) {
		if (_pitch > 89.0f) _pitch = 89.0f;
		if (_pitch < -89.0f) _pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float y_offset) {
	if (_zoom >= 1.0f && _zoom <= 45.0f) _zoom -= y_offset;
	if (_zoom <= 1.0f) _zoom = 1.0f;
	if (_zoom >= 45.0f) _zoom = 45.0f;
}

void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);

	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}
*/
#include "Camera.h"
Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp, float pitch, float yaw, float lastX, float lastY, bool firstMouse) :
	cameraPosition(cameraPosition), cameraFront(cameraFront), cameraUp(cameraUp), pitch(pitch), yaw(yaw), lastX(lastX), lastY(lastY), firstMouse(firstMouse) {}

void Camera::processKeyBoard(cam position, float deltaTime) {
	const float speed = movementSpeed * deltaTime;
	switch (position) {
	case FORWARD:
		cameraPosition += speed * glm::normalize(cameraFront);
		break;
	case BACKWARD:
		cameraPosition -= speed * glm::normalize(cameraFront);
		break;
	case LEFT:
		cameraPosition -= speed * glm::normalize(glm::cross(cameraFront, cameraUp));
		break;
	case RIGHT:
		cameraPosition += speed * glm::normalize(glm::cross(cameraFront, cameraUp));
		break;
	case UP:
		cameraPosition += speed * glm::normalize(cameraUp);
		break;
	case DOWN:
		cameraPosition -= speed * glm::normalize(cameraUp);
		break;
	}
}
glm::mat4 Camera::lookAt(glm::vec3 cameraPos, glm::vec3 lookingAt, glm::vec3 upVector) {
	glm::vec3 cameraDirection = glm::normalize(cameraPos - lookingAt);
	glm::vec3 rightAxis = glm::normalize(glm::cross(upVector, cameraDirection));
	glm::vec3 upAxis = glm::normalize(glm::cross(cameraDirection, rightAxis));

	glm::mat4 left = glm::transpose(glm::mat4{
									   rightAxis.x, rightAxis.y, rightAxis.z, 0,
										upAxis.x, upAxis.y, upAxis.z, 0,
										cameraDirection.x, cameraDirection.y, cameraDirection.z, 0,
										0,0,0,1 });
	glm::mat4 right = glm::transpose(glm::mat4{ 1,0,0,-cameraPos.x,
								0,1,0,-cameraPos.y,
								0,0,1,-cameraPos.z,
								0,0,0,1 });
	glm::mat4 lookingAtMatrix = left * right;
	return lookingAtMatrix;
}

void Camera::cameraSystem(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) return;
	Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));

	bool firstMouse = cam->firstMouse;
	float lastX = cam->lastX;
	float lastY = cam->lastY;
	float yaw = cam->yaw;
	float pitch = cam->pitch;


	if (firstMouse) {
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;
	glm::vec3 direction;
	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cam->setCameraFront(glm::normalize(direction));
	cam->firstMouse = firstMouse;
	cam->lastX = lastX;
	cam->lastY = lastY;
	cam->yaw = yaw;
	cam->pitch = pitch;
}
glm::vec3 Camera::getCameraPosition() {
	return cameraPosition;
}
glm::vec3 Camera::getCameraFront() {
	return cameraFront;

}
glm::vec3 Camera::getCameraUp() {
	return cameraUp;

}
void Camera::setCameraFront(glm::vec3 front) {
	cameraFront = front;
}

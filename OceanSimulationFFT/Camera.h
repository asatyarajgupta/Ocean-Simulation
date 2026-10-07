#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
enum cam {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};
class Camera {
private:
	glm::vec3 cameraPosition;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float pitch;
	float yaw;
	float lastX;
	float lastY;


public:
	bool firstMouse;
	float movementSpeed;
	Camera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp, float pitch, float yaw, float lastX, float lastY, bool firstMouse);
	glm::vec3 getCameraPosition();
	glm::vec3 getCameraFront();
	void setCameraFront(glm::vec3 cameraFront);
	glm::vec3 getCameraUp();
	void processKeyBoard(cam position, float deltaTime);
	glm::mat4 lookAt(glm::vec3 cameraPos, glm::vec3 lookingAt, glm::vec3 upVector);
	static void cameraSystem(GLFWwindow* window, double xpos, double ypos);
};
#endif
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "ShaderClass.h"

class Camera {
public:
	// Stores the position of the camera
	glm::vec3 Position;
	
	// Orients the camera to face the -z direction
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);

	// Sets the up direction
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	// stores the camera matrix
	glm::mat4 camMatrix = glm::mat4(1.0f);

	// Toggles mouse movement on and off
	bool mouseEnabled = true;

	// Toggles between perspective and ortho displays
	bool perspectiveToggle = true;

	// prevents multiple inputs from being read at once
	bool togglePending = false;
	bool pKeyPressed = false;

	// Width and height of the window
	int width;
	int height;

	// Movement speed and pan sensitivity
	float speed = 0.1f;
	float sensitivity = 100.0f;

	Camera(int width, int height, glm::vec3 position);

	void Matrix(Shader& shader, const char* uniform);
	void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Inputs(GLFWwindow* window);
};
#endif // !CAMERA_CLASS_H
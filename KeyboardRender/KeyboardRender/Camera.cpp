#include "Camera.h"

// Constructor
Camera::Camera(int width, int height, glm::vec3 position) {
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

// Creates the view matrix
void Camera::Matrix(Shader& shader, const char* uniform) {

	// Passes transform matrices to shader program
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(camMatrix));
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	// Initialize view and projection matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Sets the position of the camera and makes it look in -z Direction 
	view = glm::lookAt(Position, Position + Orientation, Up);

	// Sets the projection from the camera
	if (perspectiveToggle) {
		projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
	}
	else {
		projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 20.0f);
	}

	camMatrix = projection * view;
}

// Process inputs
void Camera::Inputs(GLFWwindow* window) {

	// Exits the program when escape is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Handles WASD Movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		if (!pKeyPressed) {
			perspectiveToggle = !perspectiveToggle;
			pKeyPressed = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
		pKeyPressed = false;
	}

	// Handle mouse input
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (!togglePending) {
			// Makes Sure that a mouse click is only processed once
			togglePending = true;
			if (mouseEnabled) {
				// Disables mouse input
				// Shows mouse cursor
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				mouseEnabled = false;
			}
			else {
				// Enables mouse input
				// Hides mouse cursor
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				// Sets the cursor to the middle of the screen
				glfwSetCursorPos(window, (width / 2), (height / 2));
				mouseEnabled = true;
			}
		}
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		togglePending = false;
	}

	// Handles mouse movement
	if (mouseEnabled) {

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
}
#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

// Stores data for vertices, can be expanded to include textures later on
struct Vertex {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 normal;
	glm::vec2 texture;
};

class VBO {
public:
	// Id reference of Vertex Buffer Object
	GLuint ID;
	// Constructor that generates an VBO object and links it to indices
	VBO(std::vector <Vertex>& vertices);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif // !VBO_CLASS_H#pragma once

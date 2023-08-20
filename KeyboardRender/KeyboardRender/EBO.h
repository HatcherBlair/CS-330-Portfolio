#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

class EBO {
public:
	// Id reference of Elements Buffer Object
	GLuint ID;
	// Constructor that generates an EBO object and links it to indices
	EBO(std::vector <GLuint>& indices);

	// Binds the EBO
	void Bind();
	// Unbinds the EBO
	void Unbind();
	// Deletes the EBO
	void Delete();
};

#endif // !EBO_CLASS_H


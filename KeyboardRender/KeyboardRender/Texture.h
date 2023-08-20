#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "ShaderClass.h"

class Texture {
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);

	// Assign texture unit to texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Bind texture
	void Bind();
	// Unbind texture
	void Unbind();
	// Delete texture
	void Delete();
};
#endif // !TEXTURE_CLASS_H
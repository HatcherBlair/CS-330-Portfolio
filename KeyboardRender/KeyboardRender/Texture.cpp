#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType) {

	// Assign texture type to texture object
	type = texType;

	// Store width, height, and num color channels
	int widthImg, heightImg, numColCh;

	// Flip image vertically on load so that it renders right side up
	stbi_set_flip_vertically_on_load(true);

	// Load the image
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Generate texture object
	glGenTextures(1, &ID);

	// Assign texture object
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configure the scaling type
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configure the repetition of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Assign image to texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);

	// Create mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Delete the image
	stbi_image_free(bytes);

	// Unbind texture object to prevent modification
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {

	// Get location of uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);

	// Set value of uniform
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}
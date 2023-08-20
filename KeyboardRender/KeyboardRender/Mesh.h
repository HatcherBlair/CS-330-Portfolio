#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh {
public:
	// Stores vertices and indices of objects
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	// VAO
	VAO VAO;

	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
	Mesh(std::vector <Vertex>& vertices, std::vector <Texture>& textures);
	void DrawCube(Shader& shader, Camera& camera);
	void DrawVertices(Shader& shader, Camera& camera);
	void DrawStrips(Shader& shader, Camera& camera);
	void DrawFan(Shader& shader, Camera& camera);

private:
	// Integers for size of color, vertex, and texture elements and stride
	const int numVertexElements = 3;
	const int numColorElements = 4;
	const int numNormalElements = 3;
	const int numTextureElements = 2;
	int stride = sizeof(float) * (numVertexElements + numColorElements + numNormalElements + numTextureElements);
};
#endif // !MESH_CLASS_H


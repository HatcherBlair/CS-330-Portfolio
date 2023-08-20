#include "Torus.h"

// Creates a vertex for a point on the torus
Vertex setCoords(double r, double c, int rSeg, int cSeg, int i, int j) {
	// Define the color, all tori are the same color
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Calculate vertex position
	double x = (c + r * cos(i * TAU / rSeg)) * cos(j * TAU / cSeg);
	double y = (c + r * cos(i * TAU / rSeg)) * sin(j * TAU / cSeg);
	double z = r * sin(i * TAU / rSeg);

	// Calculate the normal
	glm::vec3 normal = glm::normalize(glm::vec3(x - c * cos(j * TAU / cSeg), y - c * sin(j * TAU / cSeg), z));

	// Set x, y, z, color, and UV
	Vertex newVertex = { glm::vec3(x, y, z), // Position
		color,								 // Standard color
		normal,								 // Normal
		glm::vec2((i / (double)rSeg) * 2, (j / (double)cSeg) * 2) }; // UV

	return newVertex;
}

// c is the outer radius 
// r is the inner radius
// rSeg and cSeg are the segments of the inner and outer circles respectively
// hasTexture and hasNormal determines if we need to generate normals and texture uv's
std::vector <Vertex> createTorus(double r, double c, int rSeg, int cSeg, bool hasTexture, bool hasNormal) {

	// Define a vector to store vertex and a vertex to store point data
	std::vector <Vertex> newVertices;
	Vertex newVert;

	for (int i = 0; i < cSeg; i++) {
		// Loop over outer radius
		for (int j = 0; j < rSeg; j++) {
			// Loop over inner radius
			newVert = setCoords(r, c, rSeg, cSeg, i, j);
			newVertices.push_back(newVert);

			newVert = setCoords(r, c, rSeg, cSeg, i + 1, j);
			newVertices.push_back(newVert);

			newVert = setCoords(r, c, rSeg, cSeg, i, j + 1);
			newVertices.push_back(newVert);

			newVert = setCoords(r, c, rSeg, cSeg, i, j + 1);
			newVertices.push_back(newVert);

			newVert = setCoords(r, c, rSeg, cSeg, i + 1, j);
			newVertices.push_back(newVert);

			newVert = setCoords(r, c, rSeg, cSeg, i + 1, j + 1);
			newVertices.push_back(newVert);

		}
	}
	return newVertices;
}

void translateTorus(std::vector <Vertex>& vertices, int i) {
	for (int j = 0; j < vertices.size(); j++) {
		vertices[j].position.z += cableStartingPointZ;
		vertices[j].position.x += cableStartingPointX + (smallDiameter * i);
		vertices[j].position.y += (largeDiameter / 2) + 0.255f;
	}
}

void rotateTorus(std::vector <Vertex>& vertices) {
	for (int j = 0; j < vertices.size(); j++) {
		// Store the original X and Z values
		GLfloat originalX = vertices[j].position.x;
		GLfloat originalZ = vertices[j].position.z;

		// Apply the rotation matrix to X and Z
		vertices[j].position.x = (originalX * cos(rotationAngle)) - (originalZ * sin(rotationAngle));
		vertices[j].position.z = (originalX * sin(rotationAngle)) - (originalZ * cos(rotationAngle));

		// Store the original X and Z normals
		GLfloat originalNormalX = vertices[j].normal.x;
		GLfloat originalNormalZ = vertices[j].normal.z;

		// Apply the rotation matrix to X and Z
		vertices[j].normal.x = (originalNormalX * cos(rotationAngle)) - (originalNormalZ * sin(rotationAngle));
		vertices[j].normal.z = (originalNormalX * sin(rotationAngle)) - (originalNormalZ * cos(rotationAngle));
	}
	
}
#include "Cylinder.h"

std::vector <std::vector <Vertex> > createCylinder(double height, int numSlices) {
	// Define the color, all cylinders are the same color
	//glm::vec4 color = glm::vec4(37.0 / 255.0, 150.0 / 255.0, 190.0 / 255.0, 1.0f);
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Create a Vertex and a vector of Vertex
	Vertex newVertex;
	std::vector <Vertex> newVertices;

	// Create a 2D vector to store the three different arrays of vertices
	std::vector < std::vector <Vertex> > sideTopBottomVertices;

	// Pre-calculate X, Z, sines, and cosines for given number of slices
	std::vector<float> x, z, sines, cosines;
	const float sliceAngleStep = 2.0f * glm::pi<float>() / float(numSlices);
	for (int i = 0; i <= numSlices; i++)
	{
		sines.push_back(sin(sliceAngleStep * i));
		cosines.push_back(cos(sliceAngleStep * i));
		x.push_back(sines[i] * radius);
		z.push_back(cosines[i] * radius);
	}

	// Calculate texture step
	const float sliceTextureStepU = 2.0f / float(numSlices);

	/* Sides of cylinder */
	for (int i = 0; i <= numSlices; i++)
	{
		// Calculate the normal
		glm::vec3 normal = glm::normalize(glm::vec3(x[i], 0, z[i]));
		// Loop around top/bottom circle
		// Top of cylinder
		newVertex = { glm::vec3(x[i], height / 2.0f, z[i]), color, normal, glm::vec2(sliceTextureStepU * i, 1.0f) };
		newVertices.push_back(newVertex);
			
		// Bottom of cylinder
		newVertex = { glm::vec3(x[i], -height / 2.0f, z[i]), color, normal, glm::vec2(sliceTextureStepU * i, 0.0f) };
		newVertices.push_back(newVertex);
	}

	// Add the sides of the cylinder to return variable
	sideTopBottomVertices.push_back(newVertices);
	newVertices.clear();

	/* Top of cylinder */
	// Center point
	newVertex = { glm::vec3(0.0f, height / 2.0f, 0.0f), color, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.5f) };
	newVertices.push_back(newVertex);

	// Outer edge
	for (int i = 0; i <= numSlices; i++)
	{
		newVertex = { glm::vec3(x[i], height / 2.0f, z[i]), color, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f + sines[i] * 0.5f, 0.5f + cosines[i] * 0.5f) };
		newVertices.push_back(newVertex);
	}

	// Add the top of the cylinder to return variable
	sideTopBottomVertices.push_back(newVertices);
	newVertices.clear();

	/* Bottom of cylinder */
	// Center point
	newVertex = { glm::vec3(0.0f, -height / 2.0f, 0.0f), color, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.5f, 0.5f) };
	newVertices.push_back(newVertex);

	// Outer edge
	for (int i = 0; i <= numSlices; i++)
	{
		newVertex = { glm::vec3(x[i], height / 2.0f, z[i]), color, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.5f + sines[i] * 0.5f, 0.5f - cosines[i] * 0.5f) };
		newVertices.push_back(newVertex);
	}

	// Add the top of the cylinder to return variable
	sideTopBottomVertices.push_back(newVertices);

	return sideTopBottomVertices;
}

// Translates a cylinder into the proper location in the world
void translateCylinder(std::vector < std::vector <Vertex> >& vertices, int i) {
	if (i == 1) {
		// First cylinder
		for (int k = 0; k < 3; k++) {
			// k -> sides, top, bottom
			for (int j = 0; j < vertices[k].size(); j++) {
				vertices[k][j].position += firstCylinderPosition;
			}
		}
		
	}
	else if (i == 2) {
		// Second cylinder
		for (int k = 0; k < 3; k++) {
			// k -> sides, top, bottom
			for (int j = 0; j < vertices[k].size(); j++) {
				vertices[k][j].position += secondCylinderPosition;
			}
		}
		
	}
	
}

// Rotates a cylinder by 90 degrees around the X axis
void rotateCylinder(std::vector < std::vector <Vertex> >& vertices) {

	for (int k = 0; k < 3; k++) {
		// k -> sides, top, bottom
		for (int j = 0; j < vertices[k].size(); j++) {
			// Store the original Y and Z values
			GLfloat originalY = vertices[k][j].position.y;
			GLfloat originalZ = vertices[k][j].position.z;

			// Apply the rotation matrix to Y and Z
			vertices[k][j].position.y = (originalY * cos(rotationAngle)) - (originalZ * sin(rotationAngle));
			vertices[k][j].position.z = (originalY * sin(rotationAngle)) - (originalZ * cos(rotationAngle));

			// Rotate the normals too
			GLfloat originalNormalY = vertices[k][j].normal.y;
			GLfloat originalNormalZ = vertices[k][j].normal.z;

			vertices[k][j].normal.y = (originalNormalY * cos(rotationAngle)) - (originalNormalZ * sin(rotationAngle));
			vertices[k][j].normal.z = (originalNormalY * sin(rotationAngle)) - (originalNormalZ * cos(rotationAngle));
		}
	}
}
#ifndef KEYBOARD_CABLE_H
#define KEYBOARD_CABLE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "VBO.h"

// Constants for all tori
const double PI = 3.1415926535897932384626433832795;
const double TAU = 2 * PI;
const GLfloat rotationAngle = glm::radians(90.0f);
const GLfloat cableStartingPointZ = 10.2f;
const GLfloat cableStartingPointX = 12.8f;
const double smallDiameter = 0.52;
const double largeDiameter = 2.05;

// Constants for cylinders
const double radius = smallDiameter / 2.0;
const glm::vec3 firstCylinderPosition = glm::vec3(cableStartingPointX, radius, cableStartingPointZ - 2.3);
const GLfloat secondCylinderStartingPointX = cableStartingPointX + (24 * smallDiameter);
const glm::vec3 secondCylinderPosition = glm::vec3(secondCylinderStartingPointX, radius, cableStartingPointZ * 1.5);

#endif // !KEYBOARD_CABLE_H

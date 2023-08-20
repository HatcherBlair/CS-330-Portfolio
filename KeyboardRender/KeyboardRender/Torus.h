#ifndef TORUS_H
#define TORUS_H

#include "KeyboardCable.h"

// Sets the coordinates for one point on the torus
Vertex setCoords(double r, double c, int rSeg, int cSeg, int i, int j);

// Creates a torus object from specified dimensions
std::vector <Vertex> createTorus(double r, double c, int rSeg, int cSeg, bool hasTexture, bool hasNormal);

// Translates a torus into the proper location in the world
void translateTorus(std::vector <Vertex>& vertices, int i);

// Rotates a torus by 90 degrees around the y axis
void rotateTorus(std::vector <Vertex>& vertices);

#endif // !TORUS_H

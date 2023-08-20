#ifndef CYLINDER_H
#define CYLINDER_H

#include "KeyboardCable.h"

// Creates a cylinder with the specified height, radius is constant
// Returns a 2D vector where the first vector is the side of the cylinder, the second is the top, and the third is the bottom
std::vector < std::vector <Vertex> > createCylinder(double height, int numSlices);

// Translates a cylinder into the proper location in the world
void translateCylinder(std::vector < std::vector <Vertex> >& vertices, int i);

// Rotates a cylinder by 90 degrees around the X or Z axis, depending on i
void rotateCylinder(std::vector < std::vector <Vertex> >& vertices);

#endif // !CYLINDER_H

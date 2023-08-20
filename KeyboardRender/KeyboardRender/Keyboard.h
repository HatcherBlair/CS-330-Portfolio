#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Mesh.h"

// Returns a mesh containing the base of the keyboard
Mesh CreateKeyboardBase();
// Returns a mesh containing the light for the keyboard
Mesh CreateKeyboardLight();
// Returns a vector of mesh containing all of the keys for the keyboard
std::vector <Mesh> CreateKeyCaps();
// Creates one row of KeyCaps
std::vector < std::vector <Vertex> > CreateKeyCapRow(Vertex* vertices, int numKeys, int numVertices, GLfloat* scale);
// Rotates keycaps to be parallel with the top of the base
Vertex RotateKeyCap(Vertex vertex);

#endif // !KEYBOARD_H

#include "Keyboard.h"

// KeyCap Width, Height, Depth, offset between keys, and starting point for rows of keys
GLfloat keyCapWidth = 1.8f;
GLfloat keyCapDepth = 1.8f;
const GLfloat offsetXZ = 0.05f;
const GLfloat offsetY = 0.05f;

// Base height front to back (2.5, 6) in mm
// Base dimensions (295, 108) in mm
const GLfloat baseWidth = 29.5f;
const GLfloat baseDepth = 10.8f;
const GLfloat keyStartingPointX = (0.0f - (baseWidth / 2) + offsetXZ) + 2.0f;
const GLfloat keyStartingPointZ = ((baseDepth / 2) + offsetXZ) - 2.0f;

// Rotation angle for the keycaps, they are all parallel with the top of the keyboard base
// This means that the angle they are at is arctan(DeltaY / DeltaZ)
// 0.35f is the difference in height (6 - 2.5) converted to cm.
const float keyCapAngle = atan(0.35f / baseDepth);

// Global accumulatedDepth to be used across several function calls
GLfloat accumulatedDepth = keyStartingPointZ;

// Global accumulatedHeight to be used across several function calls
// Starts at 0.6f + offsetY and goes to 0.25f + offsetY over the five rows of the keyboard
// Add in a delta to make them above the plane and a little extra to prevent Z-fighting
GLfloat accumulatedHeight = 1.0f + 0.2125f + 0.0005f;

// Standard number of vertices for different shapes
const GLuint NUM_VERTICES_CUBE = 24;
const GLuint NUM_VERTICES_PLANE = 4;

// Array containing the size of the keys in the keyboard, one array for each row of the keyboard(Top to bottom
GLfloat KeyCapSizesRow0[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.15f }; // Backspace = 2.15u
GLfloat KeyCapSizesRow1[] = { 1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.5f }; // TAB 1.5u | \| = 1.5u
GLfloat KeyCapSizesRow2[] = { 1.8f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.3f }; // CAPS 1.8u |  ENTER 2.3u
GLfloat KeyCapSizesRow3[] = { 2.3f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.8f }; // LSHIFT 2.3u | RSHIFT 2.8u (Maybe 51/18u)
GLfloat KeyCapSizesRow4[] = { 1.25f, 1.25f, 1.25f, 6.5f, 1.25f, 1.25f, 1.25f, 1.25f }; // CTRL, WINDOWS, ALT, FN = 1.25u | Space = 6.5u (Might change to 6.25u (Standard) depending on spacing in render)

/* To keep all these objects at a reasonable size we are scaling them down by a factor of 10, ie 18 -> 1.8 */
// Key Dimenstions in mm (1Unit keys)
// Top of key (13.25, 15) (x (Right-Top), z (Back-Top))
// Bottom of key (18,18) (x (Right-Bottom), z (Back-Bottom))
// Row 0  Height front to back (11, 11.5)
// Row 1  Height front to back (9.5, 9.25)
// Row 2  Height front to back (9.3, 8.3)
// Row 3  Height front to back (10.3, 8.4)
// Row 4  Height front to back (9.9, 8)

/* KeyCap Vertex Arrays */

/* Row0 Vertex Array*/
// Row 0  Height front to back (11, 11.5) (y (Front-Top), (Back-Top))
// To center object around origin dimensions must be halved and some inverted
// Updated x-dimensions 1.325 -> 0.6625, 0.2375 -> -0.6625, 1.8 -> 0.9, 0.0 -> -0.9
// Updated z-dimensions 1.5 -> 0.75, 1.8 -> 0.9, 0.0 -> -0.9, 0.5 -> 0.75 
/* Y is a little more complicated because the y is sloped, to find the offset to center the KeyCap at the y-axis we need
*  to find the offset at the midpoint of the keycap.
*
*  y @ mid-point of top xz plane = ((1.15 - 1.1) / 2) + 1.1 = 1.125
*  Therefore y offset = 1.125/2 = 0.5625 and y = -0.5625 is the bottom of the KeyCap
*  Updated y-dimensions 1.1 - 0.5625 = 0.5375, 1.15 - 0.5625 = 0.5875, 0.0 -> -0.5625
*/
Vertex keyCapVerticesRow0[] = {
    // Positions                                    // Colors (r,g,b,a)                 // Normals                      // Texture coords
    // Front Face
    Vertex{glm::vec3(0.6625f, 0.5875f, -0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      0
    Vertex{glm::vec3(0.9f, -0.5625f, -0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Front   1
    Vertex{glm::vec3(-0.9f, -0.5625f, -0.9f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Front    2
    Vertex{glm::vec3(-0.6625f, 0.5875f, -0.75f),    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Top-Front       3

    // Left Face
    Vertex{glm::vec3(-0.9f, -0.5625f, -0.9f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}, // Left-Bottom-Front    4
    Vertex{glm::vec3(-0.6625f, 0.5875f, -0.75f),    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 1.0f)}, // Left-Top-Front       5
    Vertex{glm::vec3(-0.6625f, 0.5375f, 0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Top-Back        6
    Vertex{glm::vec3(-0.9f, -0.5625f, 0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     7

    // Back Face
    Vertex{glm::vec3(0.9f, -0.5625f, 0.9f),         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Back    8
    Vertex{glm::vec3(0.6625f, 0.5375f, 0.75f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Back       9
    Vertex{glm::vec3(-0.6625f, 0.5375f, 0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 1.0f)}, // Left-Top-Back        10
    Vertex{glm::vec3(-0.9f, -0.5625f, 0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     11

    // Right Face
    Vertex{glm::vec3(0.6625f, 0.5875f, -0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      12
    Vertex{glm::vec3(0.9f, -0.5625f, -0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Front   13
    Vertex{glm::vec3(0.9f, -0.5625f, 0.9f),         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 0.0f)}, // Right-Bottom-Back    14
    Vertex{glm::vec3(0.6625f, 0.5375f, 0.75f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, // Right-Top-Back       15

    // Bottom Face
    Vertex{glm::vec3(0.9f, -0.5625f, -0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Bottom-Front   16
    Vertex{glm::vec3(-0.9f, -0.5625f, -0.9f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Bottom-Front    17
    Vertex{glm::vec3(0.9f, -0.5625f, 0.9f),         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Back    18
    Vertex{glm::vec3(-0.9f, -0.5625f, 0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 0.0f)},  // Left-Bottom-Back    19

    // Top Face
    Vertex{glm::vec3(0.6625f, 0.5875f, -0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      20
    Vertex{glm::vec3(-0.6625f, 0.5875f, -0.75f),    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, // Left-Top-Front       21
    Vertex{glm::vec3(0.6625f, 0.5375f, 0.75f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Top-Back       22
    Vertex{glm::vec3(-0.6625f, 0.5375f, 0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f)}, // Left-Top-Back        23

};

/* Row1 Vertex Array*/
// Row 1  Height front to back (9.5, 9.25) (y (Front-Top), (Back-Top))
// To center object around origin dimensions must be halved and some inverted
/* Y is a little more complicated because the y is sloped, to find the offset to center the KeyCap at the y-axis we need
*  to find the offset at the midpoint of the keycap.
*
*  y @ mid-point of top xz plane = ((0.95 - 0.925) / 2) + 0.925 = 0.9375
*  Therefore y offset = 0.9375/2 = 0.46875 and y = -0.46875 is the bottom of the KeyCap
*/
// X and Z stay the same for all rows, only Y changes
Vertex keyCapVerticesRow1[] = {
    // Positions                                    // Colors (r,g,b,a)                 // Normals                      // Texture coords
    // Front Face
    Vertex{glm::vec3(0.6625f, 0.45625f, -0.75f),    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f) },    // Right-Top-Front      0
    Vertex{glm::vec3(0.9f, -0.46875f, -0.9f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 0.0f) },    // Right-Bottom-Front   1
    Vertex{glm::vec3(-0.9f, -0.46875f, -0.9f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 0.0f) },    // Left-Bottom-Front    2
    Vertex{glm::vec3(-0.6625f, 0.45625f, -0.75f),   glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 1.0f) },    // Left-Top-Front       3

    // Left Face
    Vertex{glm::vec3(-0.9f, -0.46875f, -0.9f),      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 0.0f)},     // Left-Bottom-Front    4
    Vertex{glm::vec3(-0.6625f, 0.45625f, -0.75f),   glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 1.0f)},     // Left-Top-Front       5
    Vertex{glm::vec3(-0.6625f, 0.48125f, 0.75f),    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 1.0f)},     // Left-Top-Back        6
    Vertex{glm::vec3(-0.9f, -0.46875f, 0.9f),       glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 0.0f)},     // Left-Bottom-Back     7

    // Back Face
    Vertex{glm::vec3(0.9f, -0.46875f, 0.9f),        glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f)},     // Right-Bottom-Back    8
    Vertex{glm::vec3(0.6625f, 0.48125f, 0.75f),     glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 1.0f)},     // Right-Top-Back       9
    Vertex{glm::vec3(-0.6625f, 0.48125f, 0.75f),    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 1.0f)},     // Left-Top-Back        10
    Vertex{glm::vec3(-0.9f, -0.46875f, 0.9f),       glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)},     // Left-Bottom-Back     11

    // Right Face
    Vertex{glm::vec3(0.6625f, 0.45625f, -0.75f),    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 1.0f)},     // Right-Top-Front      12
    Vertex{glm::vec3(0.9f, -0.46875f, -0.9f),       glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 0.0f)},     // Right-Bottom-Front   13
    Vertex{glm::vec3(0.9f, -0.46875f, 0.9f),        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 0.0f)},     // Right-Bottom-Back    14
    Vertex{glm::vec3(0.6625f, 0.48125f, 0.75f),     glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 1.0f)},     // Right-Top-Back       15

    // Bottom Face
    Vertex{glm::vec3(0.9f, -0.46875f, -0.9f),       glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 1.0f)},     // Right-Bottom-Front   16
    Vertex{glm::vec3(-0.9f, -0.46875f, -0.9f),      glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 1.0f)},     // Left-Bottom-Front    17
    Vertex{glm::vec3(0.9f, -0.46875f, 0.9f),        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 0.0f)},     // Right-Bottom-Back    18
    Vertex{glm::vec3(-0.9f, -0.46875f, 0.9f),       glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 0.0f)},     // Left-Bottom-Back     19

    // Top Face
    Vertex{glm::vec3(0.6625f, 0.45625f, -0.75f),    glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 1.0f)},     // Right-Top-Front      20
    Vertex{glm::vec3(-0.6625f, 0.45625f, -0.75f),   glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 1.0f)},     // Left-Top-Front       21
    Vertex{glm::vec3(0.6625f, 0.48125f, 0.75f),     glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 0.0f)},     // Right-Top-Back       22
    Vertex{glm::vec3(-0.6625f, 0.48125f, 0.75f),    glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f)},     // Left-Top-Back        23
};

/* Row2 Vertex Array*/
// Row 2  Height front to back (9.3, 8.3) (y (Front-Top), (Back-Top))
// To center object around origin dimensions must be halved and some inverted
/* Y is a little more complicated because the y is sloped, to find the offset to center the KeyCap at the y-axis we need
*  to find the offset at the midpoint of the keycap.
*
*  y @ mid-point of top xz plane = ((0.93 - 0.83) / 2) + 0.83 = 0.88
*  Therefore y offset = 0.88/2 = 0.44 and y = -0.44 is the bottom of the KeyCap
*/
// X and Z stay the same for all rows, only Y changes
Vertex keyCapVerticesRow2[] = {
    // Positions                                    // Colors (r,g,b,a)                 // Normals                      // Texture coords
    // Front Face
    Vertex{glm::vec3(0.6625f, 0.39f, -0.75f),       glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      0
    Vertex{glm::vec3(0.9f, -0.44f, -0.9f),          glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Front   1
    Vertex{glm::vec3(-0.9f, -0.44f, -0.9f),         glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Front    2
    Vertex{glm::vec3(-0.6625f, 0.39f, -0.75f),      glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Top-Front       3

    // Left Face
    Vertex{glm::vec3(-0.9f, -0.44f, -0.9f),         glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}, // Left-Bottom-Front    4
    Vertex{glm::vec3(-0.6625f, 0.39f, -0.75f),      glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 1.0f)}, // Left-Top-Front       5
    Vertex{glm::vec3(-0.6625f, 0.49f, 0.75f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Top-Back        6
    Vertex{glm::vec3(-0.9f, -0.44f, 0.9f),          glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     7

    // Back Face
    Vertex{glm::vec3(0.9f, -0.44f, 0.9f),           glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Back    8
    Vertex{glm::vec3(0.6625f, 0.49f, 0.75f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Back       9
    Vertex{glm::vec3(-0.6625f, 0.49f, 0.75f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 1.0f)}, // Left-Top-Back        10
    Vertex{glm::vec3(-0.9f, -0.44f, 0.9f),          glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     11

    // Right Face
    Vertex{glm::vec3(0.6625f, 0.39f, -0.75f),       glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      12
    Vertex{glm::vec3(0.9f, -0.44f, -0.9f),          glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Front   13
    Vertex{glm::vec3(0.9f, -0.44f, 0.9f),           glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 0.0f)}, // Right-Bottom-Back    14
    Vertex{glm::vec3(0.6625f, 0.49f, 0.75f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, // Right-Top-Back       15

    // Bottom Face
    Vertex{glm::vec3(0.9f, -0.44f, -0.9f),          glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Bottom-Front   16
    Vertex{glm::vec3(-0.9f, -0.44f, -0.9f),         glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Bottom-Front    17
    Vertex{glm::vec3(0.9f, -0.44f, 0.9f),           glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Back    18
    Vertex{glm::vec3(-0.9f, -0.44f, 0.9f),          glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     19

    // Top Face
    Vertex{glm::vec3(0.6625f, 0.39f, -0.75f),       glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      20
    Vertex{glm::vec3(-0.6625f, 0.39f, -0.75f),      glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, // Left-Top-Front       21
    Vertex{glm::vec3(0.6625f, 0.49f, 0.75f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Top-Back       22
    Vertex{glm::vec3(-0.6625f, 0.49f, 0.75f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f)}, // Left-Top-Back        23
};

/* Row3 Vertex Array*/
// Row 3  Height front to back (10.3, 8.4) (y (Front-Top), (Back-Top))
// To center object around origin dimensions must be halved and some inverted
/* Y is a little more complicated because the y is sloped, to find the offset to center the KeyCap at the y-axis we need
*  to find the offset at the midpoint of the keycap.
*
*  y @ mid-point of top xz plane = ((1.03 - 0.84) / 2) + 0.84 = 0.935
*  Therefore y offset = 0.935/2 = 0.4675 and y = -0.4675 is the bottom of the KeyCap
*/
// X and Z stay the same for all rows, only Y changes
Vertex keyCapVerticesRow3[] = {
    // Positions                                    // Colors (r,g,b,a)                 // Normals                      // Texture coords
    // Front Face
    Vertex{glm::vec3(0.6625f, 0.3725f, -0.75f),     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      0
    Vertex{glm::vec3(0.9f, -0.4675f, -0.9f),        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Front   1
    Vertex{glm::vec3(-0.9f, -0.4675f, -0.9f),       glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Front    2
    Vertex{glm::vec3(-0.6625f, 0.3725f, -0.75f),    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Top-Front       3

    // Left Face
    Vertex{glm::vec3(-0.9f, -0.4675f, -0.9f),       glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}, // Left-Bottom-Front    4
    Vertex{glm::vec3(-0.6625f, 0.3725f, -0.75f),    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 1.0f)}, // Left-Top-Front       5
    Vertex{glm::vec3(-0.6625f, 0.5625f, 0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Top-Back        6
    Vertex{glm::vec3(-0.9f, -0.4675f, 0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     7

    // Back Face
    Vertex{glm::vec3(0.9f, -0.4675f, 0.9f),         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Back    8
    Vertex{glm::vec3(0.6625f, 0.5625f, 0.75f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Back       9
    Vertex{glm::vec3(-0.6625f, 0.5625f, 0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 1.0f)}, // Left-Top-Back        10
    Vertex{glm::vec3(-0.9f, -0.4675f, 0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     11

    // Right Face
    Vertex{glm::vec3(0.6625f, 0.3725f, -0.75f),     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      12
    Vertex{glm::vec3(0.9f, -0.4675f, -0.9f),        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Front   13
    Vertex{glm::vec3(0.9f, -0.4675f, 0.9f),         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 0.0f)}, // Right-Bottom-Back    14
    Vertex{glm::vec3(0.6625f, 0.5625f, 0.75f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, // Right-Top-Back       15

    // Bottom Face
    Vertex{glm::vec3(0.9f, -0.4675f, -0.9f),        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Bottom-Front   16
    Vertex{glm::vec3(-0.9f, -0.4675f, -0.9f),       glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Bottom-Front    17
    Vertex{glm::vec3(0.9f, -0.4675f, 0.9f),         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Back    18
    Vertex{glm::vec3(-0.9f, -0.4675f, 0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     19

    // Top Face
    Vertex{glm::vec3(0.6625f, 0.3725f, -0.75f),     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      20
    Vertex{glm::vec3(-0.6625f, 0.3725f, -0.75f),    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, // Left-Top-Front       21
    Vertex{glm::vec3(0.6625f, 0.5625f, 0.75f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Top-Back       22
    Vertex{glm::vec3(-0.6625f, 0.5625f, 0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f)}, // Left-Top-Back        23
};

/* Row4 Vertex Array*/
// Row 4  Height front to back (9.9, 8) (y (Front-Top), (Back-Top))
// To center object around origin dimensions must be halved and some inverted
/* Y is a little more complicated because the y is sloped, to find the offset to center the KeyCap at the y-axis we need
*  to find the offset at the midpoint of the keycap.
*
*  y @ mid-point of top xz plane = ((0.99 - 0.8) / 2) + 0.8 = 0.895
*  Therefore y offset = 0.895/2 = 0.4475 and y = -0.4475 is the bottom of the KeyCap
*/
// X and Z stay the same for all rows, only Y changes
Vertex keyCapVerticesRow4[] = {
    // Positions                                    // Colors (r,g,b,a)                 // Normals                      // Texture coords
    // Front Face
    Vertex{glm::vec3(0.6625f, 0.3525f, -0.75f),     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      0
    Vertex{glm::vec3(0.9f, -0.4475f, -0.9f),        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Front   1
    Vertex{glm::vec3(-0.9f, -0.4475f, -0.9f),       glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Front    2
    Vertex{glm::vec3(-0.6625f, 0.3525f, -0.75f),    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Top-Front       3

    // Left Face
    Vertex{glm::vec3(-0.9f, -0.4475f, -0.9f),       glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}, // Left-Bottom-Front    4
    Vertex{glm::vec3(-0.6625f, 0.3525f, -0.75f),    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 1.0f)}, // Left-Top-Front       5
    Vertex{glm::vec3(-0.6625f, 0.5425f, 0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Top-Back        6
    Vertex{glm::vec3(-0.9f, -0.4475f, 0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     7

    // Back Face
    Vertex{glm::vec3(0.9f, -0.4475f, 0.9f),         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Back    8
    Vertex{glm::vec3(0.6625f, 0.5425f, 0.75f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Back       9
    Vertex{glm::vec3(-0.6625f, 0.5425f, 0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 1.0f)}, // Left-Top-Back        10
    Vertex{glm::vec3(-0.9f, -0.4475f, 0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     11

    // Right Face
    Vertex{glm::vec3(0.6625f, 0.3525f, -0.75f),     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      12
    Vertex{glm::vec3(0.9f, -0.4475f, -0.9f),        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Front   13
    Vertex{glm::vec3(0.9f, -0.4475f, 0.9f),         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 0.0f)}, // Right-Bottom-Back    14
    Vertex{glm::vec3(0.6625f, 0.5425f, 0.75f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, // Right-Top-Back       15

    // Bottom Face
    Vertex{glm::vec3(0.9f, -0.4475f, -0.9f),        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Bottom-Front   16
    Vertex{glm::vec3(-0.9f, -0.4475f, -0.9f),       glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Bottom-Front    17
    Vertex{glm::vec3(0.9f, -0.4475f, 0.9f),         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Back    18
    Vertex{glm::vec3(-0.9f, -0.4475f, 0.9f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Back     19

    // Top Face
    Vertex{glm::vec3(0.6625f, 0.3525f, -0.75f),     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      20
    Vertex{glm::vec3(-0.6625f, 0.3525f, -0.75f),    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, // Left-Top-Front       21
    Vertex{glm::vec3(0.6625f, 0.5425f, 0.75f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Top-Back       22
    Vertex{glm::vec3(-0.6625f, 0.5425f, 0.75f),     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f)}, // Left-Top-Back        23
};

/* Keyboard base vertex array */
/* To keep all these objects at a reasonable size we are scaling them down by a factor of 10, ie 18 -> 1.8 */
// Base height front to back (2.5, 6) in mm
// Base dimensions (295, 108) in mm (x, (Right), z (Back))
// To center object around origin dimensions must be halved and some inverted
/* Y is a little more complicated because the y is sloped, to find the offset to center the Keyboard Base at the y-axis we need
*  to find the offset at the midpoint of the Keyboard Base.
*
*  y @ mid-point of top xz plane = ((0.6 - 0.25) / 2) + 0.25 = 0.425
*  Therefore y offset = 0.425/2 = 0.2125 and y = -0.2125 is the bottom of the Keyboard Base
*/

Vertex keyboardBaseVertices[] = {
    // Positions                                    // Colors (r,g,b,a)                 // Normals                      // Texture coords
    // Front Face
    Vertex{glm::vec3(14.75f, 0.0375f, -5.4f),       glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      0
    Vertex{glm::vec3(14.75f, -0.2125f, -5.4f),      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Front   1
    Vertex{glm::vec3(-14.75f, -0.2125f, -5.4f),     glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 0.0f)}, // Left-Bottom-Front    2
    Vertex{glm::vec3(-14.75f, 0.0375f, -5.4f),      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Top-Front       3

    // Left Face
    Vertex{glm::vec3(-14.75f, -0.2125f, -5.4f),     glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}, // Left-Bottom-Front    4
    Vertex{glm::vec3(-14.75f, 0.0375f, -5.4f),      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(1.0f, 1.0f)}, // Left-Top-Front       5
    Vertex{glm::vec3(-14.75f, 0.3875f, 5.4f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Top-Back        6
    Vertex{glm::vec3(-14.75f, -0.2125f, 5.4f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f),   glm::vec2(0.0f, 0.0f)},  // Left-Bottom-Back    7

    // Back Face
    Vertex{glm::vec3(14.75f, -0.2125f, 5.4f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Back    8
    Vertex{glm::vec3(14.75f, 0.3875f, 5.4f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Back       9
    Vertex{glm::vec3(-14.75f, 0.3875f, 5.4f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 1.0f)}, // Left-Top-Back        10
    Vertex{glm::vec3(-14.75f, -0.2125f, 5.4f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)},  // Left-Bottom-Back    11

    // Right Face
    Vertex{glm::vec3(14.75f, 0.0375f, -5.4f),       glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      12
    Vertex{glm::vec3(14.75f, -0.2125f, -5.4f),      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Front   13
    Vertex{glm::vec3(14.75f, -0.2125f, 5.4f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 0.0f)}, // Right-Bottom-Back    14
    Vertex{glm::vec3(14.75f, 0.3875f, 5.4f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, // Right-Top-Back       15

    // Bottom Face
    Vertex{glm::vec3(14.75f, -0.2125f, -5.4f),      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Bottom-Front   16
    Vertex{glm::vec3(-14.75f, -0.2125f, -5.4f),     glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Bottom-Front    17
    Vertex{glm::vec3(14.75f, -0.2125f, 5.4f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(1.0f, 0.0f)}, // Right-Bottom-Back    18
    Vertex{glm::vec3(-14.75f, -0.2125f, 5.4f),      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec2(0.0f, 0.0f)},  // Left-Bottom-Back    19

    // Top Face
    Vertex{glm::vec3(14.75f, 0.0375f, -5.4f),       glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 1.0f)}, // Right-Top-Front      20
    Vertex{glm::vec3(-14.75f, 0.0375f, -5.4f),      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 1.0f)}, // Left-Top-Front       21
    Vertex{glm::vec3(14.75f, 0.3875f, 5.4f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Top-Back       22
    Vertex{glm::vec3(-14.75f, 0.3875f, 5.4f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec2(0.0f, 0.0f)}, // Left-Top-Back        23

};

// Vertices and Indices for the lighting on the keyboard
Vertex keyboardLightVertices[] = {
    // Positions                                    // Colors (r,g,b,a)                 // Normals                      // Texture coords
    Vertex{glm::vec3(14.5f, 0.3875f, -5.0f),       glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(1.0f, 1.0f)}, // Right-Front      0
    Vertex{glm::vec3(-14.5f, 0.3875f, -5.0f),      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, -1.0f),   glm::vec2(0.0f, 1.0f)}, // Left-Front       1
    Vertex{glm::vec3(14.5f, 0.0375f, 5.0f),        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(1.0f, 0.0f)}, // Right-Back       2
    Vertex{glm::vec3(-14.5f, 0.0375f, 5.0f),       glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec2(0.0f, 0.0f)}, // Left-Back        3
};

GLuint keyboardLightIndices[] = {
    3, 1, 0,
    0, 2, 3,
};

/* Cube Index Array */
// Because All KeyCaps and the base are a cube, we can use the same index array for all of them
GLuint cubeIndices[] = {
    // Front
    2, 1, 0,
    2, 3, 0,

    // Left
    4, 5, 6,
    4, 7, 6,

    // Back
    8, 9, 10,
    8, 11, 10,

    // Right
    12, 13, 14,
    12, 15, 14,

    // Bottom
    16, 17, 18,
    18, 19, 17,

    // Top
    20, 21, 23,
    23, 22, 20
};

// Returns a mesh containing the base of the keyboard
Mesh CreateKeyboardBase() {
    // Move the base of the keyboard up in the y direction by 0.2125 (plus a little extra to prevent Z-fighting)
    for (int i = 0; i < NUM_VERTICES_CUBE; i++) {
        keyboardBaseVertices[i].position.y += 0.2125f + 0.0005f;
    }

    // Create a vector containing the vertices for the Keyboard Base
    std::vector <Vertex> baseVerts(keyboardBaseVertices, keyboardBaseVertices + sizeof(keyboardBaseVertices) / sizeof(Vertex));

    // Create a vector containing the indices for the base
    std::vector <GLuint> ind(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));

    // Create a Texture for the base
    Texture keyCapTextures[] = {
        Texture("blackTexture.png", "tex", 0, GL_RGB, GL_UNSIGNED_BYTE),
    };
    std::vector <Texture> tex(keyCapTextures, keyCapTextures + sizeof(keyCapTextures) / sizeof(Texture));

    // Create the mesh
    Mesh baseMesh(baseVerts, ind, tex);
    return baseMesh;
}

Mesh CreateKeyboardLight() {

    // Create a vector containing the vertices for the keyboard light
    std::vector <Vertex> keyboardLightVerts(keyboardLightVertices, keyboardLightVertices + sizeof(keyboardLightVertices) / sizeof(Vertex));

    // Create a vector containing the indices for the keyboard light
    std::vector <GLuint> ind(keyboardLightIndices, keyboardLightIndices + sizeof(keyboardLightIndices) / sizeof(GLuint));

    // Create a Texture for the light
    Texture lightTextures[] = {
        Texture("WhiteMesh.png", "tex", 0, GL_RGBA, GL_UNSIGNED_BYTE),
    };
    std::vector <Texture> tex(lightTextures, lightTextures + sizeof(lightTextures) / sizeof(Texture));

    // Create the mesh
    Mesh lightMesh(keyboardLightVerts, ind, tex);
    return lightMesh;
}

// Returns a vector of mesh containing all of the keys for the keyboard
std::vector <Mesh> CreateKeyCaps() {
    /* Creates a vector containing the vertices of the keycaps*/
    // Row0
    std::vector < std::vector <Vertex> > verts = CreateKeyCapRow(keyCapVerticesRow0, sizeof(KeyCapSizesRow0) / sizeof(KeyCapSizesRow0[0]), NUM_VERTICES_CUBE, KeyCapSizesRow0);

    // Row1
    std::vector < std::vector <Vertex> > vertsTemp = CreateKeyCapRow(keyCapVerticesRow1, sizeof(KeyCapSizesRow1) / sizeof(KeyCapSizesRow1[0]), NUM_VERTICES_CUBE, KeyCapSizesRow1); // Temporary, used to call the insert function
    verts.insert(verts.end(), vertsTemp.begin(), vertsTemp.end());

    // Row2
    vertsTemp = CreateKeyCapRow(keyCapVerticesRow2, sizeof(KeyCapSizesRow2) / sizeof(KeyCapSizesRow2[0]), NUM_VERTICES_CUBE, KeyCapSizesRow2); // Temporary, used to call the insert function
    verts.insert(verts.end(), vertsTemp.begin(), vertsTemp.end());

    // Row3
    vertsTemp = CreateKeyCapRow(keyCapVerticesRow3, sizeof(KeyCapSizesRow3) / sizeof(KeyCapSizesRow3[0]), NUM_VERTICES_CUBE, KeyCapSizesRow3); // Temporary, used to call the insert function
    verts.insert(verts.end(), vertsTemp.begin(), vertsTemp.end());

    // Row4
    vertsTemp = CreateKeyCapRow(keyCapVerticesRow4, sizeof(KeyCapSizesRow4) / sizeof(KeyCapSizesRow4[0]), NUM_VERTICES_CUBE, KeyCapSizesRow4); // Temporary, used to call the insert function
    verts.insert(verts.end(), vertsTemp.begin(), vertsTemp.end());

    // Create a vector containing the indices for the KeyCaps
    std::vector <GLuint> ind(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));

    // Create a Texture for the KeyCaps
    Texture keyCapTextures[] = {
        Texture("blackTexture.png", "tex", 0, GL_RGB, GL_UNSIGNED_BYTE),
    };
    std::vector <Texture> tex(keyCapTextures, keyCapTextures + sizeof(keyCapTextures) / sizeof(Texture));

    // Create a vector of type Mesh to store all the mesh's for the keyCaps
    std::vector <Mesh> keyCaps;
    for (int i = 0; i < verts.size(); i++) {
        Mesh newMesh(verts[i], ind, tex);
        keyCaps.push_back(newMesh);
    }

    return keyCaps;
}

// Creates the vertices for an entire row of keyCaps
/* Eventually x and z translations will be places into a vec3 but for testing purposes they are separate for now
*  There are still some issues with creating the rows, the ends of the row do not line up perfectly, there needs to be some tinkering to fix that
*  Possible solution is to calculate the width of the total row and make sure that the entire with is used
*  Could look like key width = ((totalWidth/ numKeys) - offset * ((2* numkeys) - 2)))
*  That would make all keys the same size, needs to be tweaked to include the scaling factor of each key individually
*  Maybe the total width is based off of the number of units per row (the same units that the scaling factor is in)
*/
std::vector < std::vector <Vertex> > CreateKeyCapRow(Vertex* vertices, int numKeys, int numVertices, GLfloat* scale) {

    // Create a vector to store translated and scaled vertices
    std::vector<Vertex> newVertices;

    // Create a vector to store vectors of new vertices
    std::vector < std::vector <Vertex> > newVectors;

    // Create a vec3 used to translate object in x direction
    GLfloat translateX = keyStartingPointX + offsetXZ;
    GLfloat accumulatedWidth = 0.0f;

    // Create a vertex used to store the new keyCap data
    Vertex newVertex;

    // Iterate over the number of keys in the row
    for (int i = 0; i < numKeys; i++) {

        // Clear newVertices for the start of a new key
        newVertices.clear();

        // Iterate over the number of vertices per key
        for (int j = 0; j < numVertices; j++) {

            // Apply translation and scale to newVertex
            newVertex = vertices[j];
            newVertex.position.x *= scale[numKeys - 1 - i];

            newVertex = RotateKeyCap(newVertex);

            newVertex.position.x += keyStartingPointX + accumulatedWidth + ((keyCapWidth * (scale[numKeys - 1 - i] - 1)) / 2.0f);


            newVertex.position.z += accumulatedDepth;
            newVertex.position.y += accumulatedHeight;

            // Add newVertex to newVertices Vector
            newVertices.push_back(newVertex);
        }
        // Add new cube to newVectors Vector
        //newVectors.insert(newVectors.begin(), newVertices);
        newVectors.push_back(newVertices);

        // Adjust the accumulated width of drawn objects
        accumulatedWidth += (keyCapWidth * scale[numKeys - 1 - i]) + offsetXZ;
    }

    // Z offset is global because it is used across several function calls
    // Update after all keys for the row have been rendered to get it ready for the next row of keys
    accumulatedDepth -= (keyCapDepth + offsetXZ);

    // Y offset is global because it is used across several function calls
    // Update after all keys for the row have been rendered to get it ready for the next row of keys
    accumulatedHeight -= offsetY;
    return newVectors;
}

/* Rotates one vertex in a keycap around the x axis*/
// Because the objects are translated and set in the world before being drawn we can't use glm::rotate
// and the rotation needs to be applied before the translation.
// To accomplish this we are applying the rotation matrix to every point in the each keyCap
// Translation matrix for x :
// | 1 0           0          |
// | 0 cos(theta) -sin(theta) |
// | 0 sin(theta)  cos(theta) |
/* WARNING: DO NOT ROTATE AN OBJECT 45 DEGREES, IT COMPRESSED THE OBJECT AND IT WILL NOT RENDER PROPERLY*/
Vertex RotateKeyCap(Vertex vertex) {
    // Store the original Y and Z values
    GLfloat originalY = vertex.position.y;
    GLfloat originalZ = vertex.position.z;

    // Apply the rotation matrix to Y and Z
    vertex.position.y = (originalY * cos(keyCapAngle)) - (originalZ * sin(keyCapAngle));
    vertex.position.z = (originalY * sin(keyCapAngle)) - (originalZ * cos(keyCapAngle));

    // Store the original Y and Z normal values
    GLfloat originalNormalY = vertex.normal.y;
    GLfloat originalNormalZ = vertex.normal.z;

    // Apply the rotation matrix to Y and Z normals
    vertex.normal.y = (originalNormalY * cos(keyCapAngle)) - (originalNormalZ * sin(keyCapAngle));
    vertex.normal.z = (originalNormalY * sin(keyCapAngle)) - (originalNormalZ * cos(keyCapAngle));

    return vertex;
}
#version 440 core
layout (location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout (location = 1) in vec4 color;    // Color data from Vertex Attrib Pointer 1
layout (location = 2) in vec3 normal;   // Normal data from Vertex Attrib Pointer 2
layout (location = 3) in vec2 texture;  // Texture data from Vertex Attrib Pointer 3

out vec4 vertexColor; // variable to transfer color data to the fragment shader

out vec2 texCoord;   // Outputs the coordinates for the texture to the fragment shader

out vec3 crntPos; // Outputs current position for the fragment shader

out vec3 Normal; // Outputs the normal for the fragment shader

//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    // Current position
    crntPos = vec3(model * vec4(position, 1.0f));
    // Normal
    Normal = normal;
    // Color
    vertexColor = color;
    // Texture location
    texCoord = texture;

    // Position of vertices
    gl_Position = camMatrix * vec4(crntPos, 1.0f);
}
#include "Mesh.h"


// Constructor for objects with indices
Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures) {
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;

    // Generate and bind the VAO's
    VAO.Bind();

    // Generate and bind the VBO's
    VBO VBO(vertices);

    // Generate and bind the EBO
    EBO EBO(indices);

    // Link VBO's Coords, Colors, normals, and Textures to VAO's
    VAO.LinkAttrib(VBO, 0, numVertexElements,  GL_FLOAT, stride, (void*)0);
    VAO.LinkAttrib(VBO, 1, numColorElements,   GL_FLOAT, stride, (void*)(numVertexElements * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, numNormalElements,  GL_FLOAT, stride, (void*)((numVertexElements + numColorElements) * sizeof(float)));
    VAO.LinkAttrib(VBO, 3, numTextureElements, GL_FLOAT, stride, (void*)((numVertexElements + numColorElements + numNormalElements) * sizeof(float)));


    // Unbind all to prevent modification
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

// Constructor for objects without indices
Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <Texture>& textures) {
    Mesh::vertices = vertices;
    Mesh::textures = textures;

    // Generate and bind the VAO's
    VAO.Bind();

    // Generate and bind the VBO's
    VBO VBO(vertices);

    // Link VBO's Coords, Colors, and Textures to VAO's
    VAO.LinkAttrib(VBO, 0, numVertexElements, GL_FLOAT, stride, (void*)0);
    VAO.LinkAttrib(VBO, 1, numColorElements, GL_FLOAT, stride, (void*)(numVertexElements * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, numNormalElements, GL_FLOAT, stride, (void*)((numVertexElements + numColorElements) * sizeof(float)));
    VAO.LinkAttrib(VBO, 3, numTextureElements, GL_FLOAT, stride, (void*)((numVertexElements + numColorElements + numNormalElements) * sizeof(float)));

    // Unbind all to prevent modification
    VAO.Unbind();
    VBO.Unbind();
}

// Draws elements using indices
void Mesh::DrawCube(Shader& shader, Camera& camera) {
    shader.Activate();
    VAO.Bind();

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        textures[i].texUnit(shader, "tex0", i);
        textures[i].Bind();
    }
    // Take care of the camera Matrix
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    // Draw the mesh
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

// Draws elements using vertices
void Mesh::DrawVertices(Shader& shader, Camera& camera) {
    shader.Activate();
    VAO.Bind();

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        textures[i].texUnit(shader, "tex0", i);
        textures[i].Bind();
    }
    // Take care of the camera Matrix
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");
    
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);
}

// Draws triangle strips
void Mesh::DrawStrips(Shader& shader, Camera& camera) {
    shader.Activate();
    VAO.Bind();

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        textures[i].texUnit(shader, "tex0", i);
        textures[i].Bind();
    }
    // Take care of the camera Matrix
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}

// Draws traingle fans
void Mesh::DrawFan(Shader& shader, Camera& camera) {
    shader.Activate();
    VAO.Bind();

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        textures[i].texUnit(shader, "tex0", i);
        textures[i].Bind();
    }
    // Take care of the camera Matrix
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
}
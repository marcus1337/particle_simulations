#ifndef UTILS3_H
#define UTILS3_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;
};

struct MeshVAO {
    GLuint vao;
    GLuint vertexVBO;
    GLuint normalVBO;
    GLuint indexVBO;
    int numVertices;
    int numIndices;
};

struct Context {
    GLuint program;
    Mesh mesh;
    GLuint defaultVAO;
    GLuint cubemap;
    MeshVAO meshVAO;
};

enum AttributeLocation {
    POSITION = 0,
    NORMAL = 1
};

namespace {

    void createMeshVAO(Context &ctx, const Mesh &mesh, MeshVAO *meshVAO) {
        // Generates and populates a VBO for the vertices
        glGenBuffers(1, &(meshVAO->vertexVBO));
        glBindBuffer(GL_ARRAY_BUFFER, meshVAO->vertexVBO);
        auto verticesNBytes = mesh.vertices.size() * sizeof(mesh.vertices[0]);
        glBufferData(GL_ARRAY_BUFFER, verticesNBytes, mesh.vertices.data(), GL_STATIC_DRAW);

        // Generates and populates a VBO for the vertex normals
        glGenBuffers(1, &(meshVAO->normalVBO));
        glBindBuffer(GL_ARRAY_BUFFER, meshVAO->normalVBO);
        auto normalsNBytes = mesh.normals.size() * sizeof(mesh.normals[0]);
        glBufferData(GL_ARRAY_BUFFER, normalsNBytes, mesh.normals.data(), GL_STATIC_DRAW);

        // Generates and populates a VBO for the element indices
        glGenBuffers(1, &(meshVAO->indexVBO));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVAO->indexVBO);
        auto indicesNBytes = mesh.indices.size() * sizeof(mesh.indices[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNBytes, mesh.indices.data(), GL_STATIC_DRAW);

        // Creates a vertex array object (VAO) for drawing the mesh
        glGenVertexArrays(1, &(meshVAO->vao));
        glBindVertexArray(meshVAO->vao);
        glBindBuffer(GL_ARRAY_BUFFER, meshVAO->vertexVBO);
        glEnableVertexAttribArray(POSITION);
        glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, meshVAO->normalVBO);
        glEnableVertexAttribArray(NORMAL);
        glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVAO->indexVBO);
        glBindVertexArray(ctx.defaultVAO); // unbinds the VAO

        // Additional information required by draw calls
        meshVAO->numVertices = mesh.vertices.size();
        meshVAO->numIndices = mesh.indices.size();
    }
};

#endif
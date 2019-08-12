#ifndef PARTICLESHADER_H
#define PARTICLESHADER_H


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>


struct ShaderData {
    int ID = 0;
    float radius = 1.f;
    glm::vec4 color = glm::vec4(0, 0, 0, 0);

    ///////
    GLuint shaderProgram = -1;
    GLuint vao = -1;
    GLuint vertexVBO = -1;
    GLuint colorVBO = -1;
    GLuint radiusVBO = -1;
    GLuint indexVBO = -1;
    int numVertices = -1;
    /////

    ShaderData() {

    }

    ~ShaderData() {
        //  glDeleteVertexArrays(1, &vao);
        //  glDeleteBuffers(1, &vertexVBO);
       //   glDeleteBuffers(1, &colorVBO);
       //   glDeleteBuffers(1, &radiusVBO);
    }

    void cleanup() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vertexVBO);
        glDeleteBuffers(1, &colorVBO);
        glDeleteBuffers(1, &radiusVBO);
    }

};

class ParticleShader {
public:

    static constexpr int FOX = 1;
    std::vector<ShaderData> shaders;
    int getShaderIndex(int ID_, float radius_, glm::vec4 color_, std::vector<glm::vec3>& particles_);
    std::string path3D = "C:\\Users\\Marcus\\source\\repos\\coolgame\\Debug\\3d_models\\";

    ~ParticleShader() {
        for (auto& sh : shaders)
            sh.cleanup();
    }

};

#endif
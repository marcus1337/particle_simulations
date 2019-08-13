#ifndef FLOOR_H
#define FLOOR_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

class Floor {


public:

    GLuint floorProgram = -1;
    GLuint textNum = -1;

    Floor();

    void init();

    GLuint vao, vbo;

    void cleanup();
    void draw(glm::mat4& VP, glm::mat4& P, glm::vec3 cameraPos_);

    glm::vec3 position = glm::vec3(0, 0, 0);
};


#endif
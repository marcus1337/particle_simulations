#include "Floor.h"

#include "MiscCode/utils.h"
#include <iostream>
#include "MiscCode/utils3.h"

using namespace std;

Floor::Floor() {

}

void Floor::init() {
    floorProgram = loadShaderProgram("floor.vert", "floor.frag");
    textNum = load2DTexture("vectorgrid.png", true);

    const float numReps = 5.0f;
    const float floorSize = 50.f;
    ////////////////VAO SETUP
    GLfloat vertices[] = { // format = x, y, z, u, v

    -floorSize, -1.0f, -floorSize,  0.0f, 0.0f,     floorSize, -1.0f, -floorSize, numReps, 0.0f,
     floorSize,  -1.0f, floorSize, numReps, numReps,    -floorSize,  -1.0f, floorSize, 0.0f, numReps
    };
    glGenVertexArrays(1, &vao); // vao saves state of array buffer, element array, etc
    glGenBuffers(1, &vbo); // vbo stores vertex data
    GLint curr_vao; // original state
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &curr_vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(curr_vao);
    /////////////////////////////////////////

}

void Floor::cleanup() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Floor::draw(glm::mat4& VP, glm::mat4& P, glm::vec3 cameraPos_) {
   // glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), mscale);
   // glm::mat4 rotateMat = glm::toMat4(orientation);
    glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 modModel = translateMat;

    glm::mat4 MVP = VP * modModel;

    glUseProgram(floorProgram);
    glUniformMatrix4fv(glGetUniformLocation(floorProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textNum);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //glDrawElements(GL_TRIANGLES, shader.ctx.meshVAO.numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
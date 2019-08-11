#include "Shape.h"

Shape::Shape(ShaderShape& shader_) : shader(shader_), model(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)),
mscale(1.f,1.f,1.f), position(0,0,0), orientation(1.f,0,0,0) {

}


void Shape::draw(glm::mat4& VP) {

    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), mscale);
    glm::mat4 rotateMat = glm::toMat4(orientation);
    glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 modModel = scaleMat * translateMat * rotateMat;

    glm::mat4 MVP = VP * modModel;

    glUseProgram(shader.shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glBindVertexArray(shader.ctx.meshVAO.vao);
    glDrawElements(GL_TRIANGLES, shader.ctx.meshVAO.numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(shader.ctx.defaultVAO);
}
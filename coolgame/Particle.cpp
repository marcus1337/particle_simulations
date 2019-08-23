#include "Particle.h"
#include <iostream>

using namespace std;

void Particle::printStuff() {}

bool Particle::checkCollision(glm::vec3 pos1, glm::vec3 pos2, float radius1, float radius2) {
    bool result = false;
    float tmpDist = glm::distance(pos1, pos2);
    tmpDist < ((radius1 + radius2)) ? result = true : result = false;
    return result;
}

bool Particle::checkCollision(Particle& otherParticle) {
    bool result = false;
    float tmpDist = glm::distance(position, otherParticle.position);
    tmpDist < ((radius+otherParticle.radius)) ? result = true : result = false;
    return result;
}

void Particle::setTrans(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

void Particle::addTrans(float x, float y, float z) {
    position.x += x;
    position.y += y;
    position.z += z;
}

void Particle::draw(glm::mat4& VP) {

    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaleVal, scaleVal, scaleVal));
    //glm::mat4 rotateMat = glm::toMat4(orientation);
    glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 modModel = translateMat * scaleMat;
    glm::mat4 MVP = VP * modModel;

    if (isColliding) {
        drawShader(hitShader, MVP);
    }
    else {
        drawShader(shader, MVP);
    }
}

void Particle::draw2(glm::mat4& VP, glm::mat4& P, glm::mat4& V) {
    glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 modModel = translateMat;
    glm::mat4 MVP = VP * modModel;
    glm::mat4 MV = V * modModel;

    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    glUseProgram(shader->shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shader->shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->shaderProgram, "MV"), 1, GL_FALSE, &MV[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(shader->shaderProgram, "P"), 1, GL_FALSE, &P[0][0]);

    glUniform1f(glGetUniformLocation(shader->shaderProgram, "radius"), radius);
    glUniform1f(glGetUniformLocation(shader->shaderProgram, "v_alpha"), alpha);

    glm::vec2 screenSize(*screenWidth,*screenHeight);
    glUniform2fv(glGetUniformLocation(shader->shaderProgram, "u_screenSize"), 1, glm::value_ptr(screenSize));

    glm::vec3 tmpPos(0.0f, 0.0f, 0.0f);
    glm::vec3 tmpColor(1, 1, 0);
    if (isColliding)
        tmpColor.y = 0;

    glUniform3fv(glGetUniformLocation(shader->shaderProgram, "position"),1,glm::value_ptr(tmpPos));
    glUniform3fv(glGetUniformLocation(shader->shaderProgram, "v_color"), 1, glm::value_ptr(tmpColor));

    glUniform3fv(glGetUniformLocation(shader->shaderProgram, "global_position"), 1, glm::value_ptr(position));
    glUniform3fv(glGetUniformLocation(shader->shaderProgram, "camera_position"), 1, glm::value_ptr(*cameraPos));

    glBindVertexArray(shader->ctx.meshVAO.vao);
    glDrawArrays(GL_POINTS, 0, 1);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

std::string Particle::getCoordStr() {
    return "";
}

void Particle::drawShader(ShaderShape* tmpShape_, glm::mat4& MVP) {
    glUseProgram(tmpShape_->shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(tmpShape_->shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glBindVertexArray(tmpShape_->ctx.meshVAO.vao);
    glDrawElements(GL_TRIANGLES, tmpShape_->ctx.meshVAO.numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(tmpShape_->ctx.defaultVAO);
}
#include "MiscCode/utils.h"

#include "SkyBox.h"
#include <iostream>

#include "MiscCode/utils3.h"

SkyBox::SkyBox() {
}

void SkyBox::init(MyWindow* mywindow_) {
    mywindow = mywindow_;
    skyProgram = loadShaderProgram("skybox.vert", "skybox.frag");
    std::string totpath = myLoc::project_location + pathCubeMap;

    skyText = loadCubemap(totpath);

    glGenBuffers(1, &(vertexVBO));
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    auto verticesNBytes = numSkyVerts * sizeof(skyboxVertices[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesNBytes, &skyboxVertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glEnableVertexAttribArray(POSITION);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void SkyBox::draw(glm::mat4& VP) {

    float scaleVal = 20.f;
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaleVal, scaleVal, scaleVal));
    glm::mat4 rotateMat = glm::mat4(1.0f);
    glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), mywindow->camera.Position);
    glm::mat4 modModel = translateMat *scaleMat;

    glm::mat4 MVP = VP * modModel;

    glDepthMask(GL_FALSE);
    glUseProgram(skyProgram);

    glUniformMatrix4fv(glGetUniformLocation(skyProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glBindVertexArray(vao);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyText);
    glUniform1i(glGetUniformLocation(skyProgram, "skybox"), 0);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void SkyBox::cleanup() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertexVBO);
}
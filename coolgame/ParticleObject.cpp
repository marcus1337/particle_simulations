#include "ParticleObject.h"
#include "ParticleCreator.h"

using namespace std;



ParticleObject::ParticleObject(){
}

void ParticleObject::init2(float partRadius_, float scaleVal_, glm::vec4 color_, ShaderShape* shaderShape_, ParticleShader* particleShadCreator_) {
    partRadius = partRadius_;
    shaderShape = shaderShape_;
    color = color_;
    particleShadCreator = particleShadCreator_;

    ParticleCreator pcreator(shaderShape->ctx.mesh.vertices, partRadius/2);
    auto partPositions = pcreator.createParticles();

    cout << "TEST " << partPositions.size() << endl;

    for (int i = 0; i < partPositions.size(); i++) {
        partPositions[i] *= scaleVal_;
    }
    partRadius /= 2;
    partRadius *= scaleVal_;
    partRadius /= 2;

    partShadIndex = particleShadCreator->getShaderIndex(ParticleShader::FOX, partRadius, color, partPositions);

    for (auto p : partPositions) {
        Particle particle(particleShader, p, partRadius, 1.0f);
        particles.push_back(particle);
    }

   // cout << particleShadCreator_->shaders[partShadIndex].vao << endl;
}

void ParticleObject::init(float partDiameter_, ShaderShape* shaderShape_, ShaderShape* particleShader_, ShaderShape* particleShader2_) {
    partDiameter = partDiameter_;
    shaderShape = shaderShape_;
    particleShader = particleShader_;

    ParticleCreator pcreator(shaderShape->ctx.mesh.vertices, partDiameter /2.0f);
    auto partPositions = pcreator.createParticles();

    cout << "points : " << partPositions.size() << endl;
   /* auto a = partPositions[0];
    float tmp = 999999;
    for (int i = 1; i < partPositions.size(); i++) {
        float dis = glm::distance(a*0.01f, partPositions[i]*0.01f);
        if (dis < tmp)
            tmp = dis;
    }
    cout << "DIST MIN: " << tmp << endl;*/

    for (auto p : partPositions) {
        p *= 0.01;
        Particle particle(particleShader, p, partDiameter/2.0f, 0.01*(partDiameter /2.0f));
        particle.hitShader = particleShader2_;
      //  Particle particle(particleShader, p, 2, 1);
        particles.push_back(particle);
    }
}


void ParticleObject::draw(glm::mat4& VP) {
    for (auto& particle : particles) {
        particle.draw(VP);
    }
}

void ParticleObject::draw2(glm::mat4& VP, glm::mat4& P) {
    //glm::mat4 rotateMat = glm::toMat4(orientation);
    glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 modModel = translateMat;
    glm::mat4 MVP = VP * modModel;

    GLuint prog = particleShadCreator->shaders[partShadIndex].shaderProgram;
    GLuint vao = particleShadCreator->shaders[partShadIndex].vao;

    glUseProgram(prog);

    glUniformMatrix4fv(glGetUniformLocation(prog, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    float P11 = P[1][1];
    glm::vec2 screenSize(*screenWidth, *screenHeight);
    glUniform1f(glGetUniformLocation(prog, "P11"), P11);
    glUniform2fv(glGetUniformLocation(prog, "u_screenSize"), 1, glm::value_ptr(screenSize));
   
    //glDrawElements(GL_POINTS, particleShadCreator->shaders[partShadIndex].numVertices, GL_UNSIGNED_INT,0);
    glBindVertexArray(vao);

    //glDrawElements(GL_TRIANGLES, particleShadCreator->shaders[partShadIndex].numVertices, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_POINTS, 0, particleShadCreator->shaders[partShadIndex].numVertices);
    glBindVertexArray(0);
}
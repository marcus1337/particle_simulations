#include "ParticleObject.h"
#include "ParticleCreator.h"

using namespace std;



ParticleObject::ParticleObject(){
}

ParticleObject::ParticleObject(const ParticleObject& a) {
    *this = a;
    for (auto& x : particles) {
        x.partData.objData = &objData;
    }
}

void ParticleObject::init2(float partRadius_, float scaleVal_, glm::vec4 color_, ShaderShape* shaderShape_, ParticleShader* particleShadCreator_) {
    partRadius = partRadius_;
    shaderShape = shaderShape_;
    color = color_;
    particleShadCreator = particleShadCreator_;

    ParticleCreator pcreator(shaderShape->ctx.mesh.vertices, partRadius/2);
    auto partPositions = pcreator.createParticles();

    //cout << "TEST " << partPositions.size() << endl;
    objData.centerOfMass = glm::vec3(0, 0, 0);
  //  float miny = 100;

    for (int i = 0; i < partPositions.size(); i++) {
        partPositions[i] *= scaleVal_;
        objData.centerOfMass += partPositions[i];
   //     miny = std::min<float>(miny, partPositions[i].y);
    }

    objData.centerOfMass /= partPositions.size();
    //objData.centerOfMass = glm::vec3(1000, 0, 0); //comment out to not have center at 0

    partRadius /= 2;
    partRadius *= scaleVal_;
    partRadius /= 2;

    objData.partRadius = partRadius;

    for (int i = 0; i < partPositions.size(); i++) {
        objData.prepareData(1.0f, partPositions[i] - objData.centerOfMass);
    }

    partShadIndex = particleShadCreator->getShaderIndex(ParticleShader::FOX, partRadius, color, partPositions);

    int indice_ = 0;
    for (auto p : partPositions) {
        ParticleData partData(p,partRadius,&objData);

        Particle particle(partData, particleShader, p, partRadius, 1.0f);
        particle.indice = indice_;
        particles.push_back(particle);
        indice_++;
    }
    indices.resize(particles.size());
   // glm::vec3 EulerAngles(45*2, 0, 0);
   // objData.rotation = glm::quat(EulerAngles);
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

std::string getVecStr(glm::vec3 adata) {
    return std::string("[" + to_string(adata.x) + "," + to_string(adata.y) + "," + to_string(adata.z) + "]");
}

void ParticleObject::doPhysics() {

    glm::vec3 Fc(0,0,0);
    glm::vec3 Tc(0,0,0);
    //int counter = 0;
    for (auto& p : particles) {
        //if (counter == 0)
         //   cout << "R " << getVecStr(p.partData.r) << " R0 " << getVecStr(p.partData.r0) << " DIFF " <<
          //  (p.partData.r.y - p.partData.r0.y) << endl;
        //counter++;
        Fc += p.partData.getFc();
        Tc += p.partData.getTc();
        p.partData.update();
    }

    objData.update(Fc, Tc);
    //cout << "TEST: " << objData.W.x << " " << objData.W.y << " " << objData.W.z << endl;
}


void ParticleObject::draw(glm::mat4& VP) {
    for (auto& particle : particles) {
        particle.draw(VP);
    }
}

void ParticleObject::draw2(glm::mat4& VP, glm::mat4& P, glm::vec3 cameraPos_) {
    //glm::mat4 rotateMat = glm::toMat4(orientation);
    glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), objData.position);

   // cout << "WAT " << getVecStr(objData.position) << " _ " << getVecStr(particles[0].partData.objData->position) << " RAD : " << partRadius << endl;
  //  glm::vec3 EulerAngles(90, 45, 0);
   // rotation = glm::quat(EulerAngles);

    glm::mat4 transTemp = glm::translate(glm::mat4(1.0f), -objData.centerOfMass);
    glm::mat4 transTempBack = glm::translate(glm::mat4(1.0f), objData.centerOfMass);
    glm::mat4 RotationMatrix = glm::toMat4(objData.rotation);
    RotationMatrix = transTempBack * RotationMatrix * transTemp;

    glm::mat4 modModel = translateMat * RotationMatrix;
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

    if (color.w < 1.0f) {
        std::sort(particles.begin(), particles.end(), [&](const Particle& v1, const Particle& v2) {
            glm::vec4 p1(v1.position, 1.0f);
            glm::vec4 p2(v2.position, 1.0f);
            p1 = modModel* p1;
            p2 = modModel* p2;
            auto d1 = glm::distance(p1, glm::vec4(cameraPos_,1.0f));
            auto d2 = glm::distance(p2, glm::vec4(cameraPos_,1.0f));
            return d1 > d2;
        });
        for (int i = 0; i < particles.size(); i++) {
            indices[i] = particles[i].indice;
        }
        glDepthMask(GL_FALSE);
        glDrawElements(GL_POINTS, particleShadCreator->shaders[partShadIndex].numVertices, GL_UNSIGNED_INT, indices.data());
        glDepthMask(GL_TRUE);
    }
    else {
        glDrawArrays(GL_POINTS, 0, particleShadCreator->shaders[partShadIndex].numVertices);
    }

    glBindVertexArray(0);
}


void ParticleObject::copyObject(ParticleObject& other) {
    *this = other;
    for (auto& x : particles) {
        x.partData.objData = &objData;
    }

}
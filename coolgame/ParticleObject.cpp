#include "ParticleObject.h"
#include "ParticleCreator.h"

using namespace std;



ParticleObject::ParticleObject(){
}

/*Copy constructor.*/
ParticleObject::ParticleObject(const ParticleObject& a) {
    *this = a;
    for (auto& x : particles) {
        x.partData.objData = &objData;
    }
}


/*Init VBO and create particles from vertices by testing intersections in ParticleCreator.*/
void ParticleObject::init2(float partRadius_, float scaleVal_, glm::vec4 color_, ShaderShape* shaderShape_, ParticleShader* particleShadCreator_) {
    partRadius = partRadius_;
    shaderShape = shaderShape_;
    color = color_;
    particleShadCreator = particleShadCreator_;

    ParticleCreator pcreator(shaderShape->ctx.mesh.vertices, partRadius/2);
    auto partPositions = pcreator.createParticles();
    objData.centerOfMass = glm::vec3(0, 0, 0);

    for (int i = 0; i < partPositions.size(); i++) {
        partPositions[i] *= scaleVal_;
        objData.centerOfMass += partPositions[i];
    }

    objData.centerOfMass /= partPositions.size();
    //objData.centerOfMass = glm::vec3(1000, 0, 0); //comment out to not have center at 0

    partRadius /= 2; //These divisions by 2 was apparently necessary to have proper radius, not sure why.
    partRadius *= scaleVal_;
    partRadius /= 2;
    objData.partRadius = partRadius;

    for (int i = 0; i < partPositions.size(); i++) { //Make particle positions relative to center of mass.
        objData.prepareData(1.0f, partPositions[i] - objData.centerOfMass);
    }

    partShadIndex = particleShadCreator->getShaderIndex(ParticleShader::FOX, partRadius, color, partPositions);

    int indice_ = 0;
    for (auto p : partPositions) { //Store particles in particle array, along with the whole objects data
        ParticleData partData(p,partRadius,&objData);

        Particle particle(partData, particleShader, p, partRadius, 1.0f);
        particle.indice = indice_;
        particles.push_back(particle);
        indice_++;
    }
    indices.resize(particles.size());
}


/*Apply physics calculations from each particle.*/
void ParticleObject::doPhysics() {

    glm::vec3 Fc(0,0,0);
    glm::vec3 Tc(0,0,0);
    for (auto& p : particles) {
        Fc += p.partData.getFc();
        Tc += p.partData.getTc();
        p.partData.update();
    }

    objData.update(Fc, Tc);
}


/*Draw each particle individually (not used)*/
void ParticleObject::draw(glm::mat4& VP) {
    for (auto& particle : particles) {
        particle.draw(VP);
    }
}

void ParticleObject::draw2(glm::mat4& VP, glm::mat4& P, glm::vec3 cameraPos_) {

    glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), objData.position);
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
   

    glBindVertexArray(vao);

    if (color.w < 1.0f) { //Special rendering if color is transparent.
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

//Copy function
void ParticleObject::copyObject(ParticleObject& other) {
    *this = other;
    for (auto& x : particles) {
        x.partData.objData = &objData;
    }

}
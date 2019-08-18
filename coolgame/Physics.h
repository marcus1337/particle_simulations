#ifndef PHYSICS_H
#define PHYSICS_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>


struct ObjectData {
    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    float partRadius = 1.0f;
    glm::quat rotation = glm::quat(1.0f,0,0,0);

    glm::vec3 centerOfMass = glm::vec3(0, 0, 0);
    glm::vec3 V = glm::vec3(0, 0, 0); //Velocity
    glm::vec3 W = glm::vec3(0, 0, 0); //Angular velocity

    glm::mat3 I0, It_inv;
    glm::mat3 I0_inv = glm::mat3(0);

    glm::vec3 F = glm::vec3(0, 0, 0);
    float mass = 0;

    void prepareData(float mass_, glm::vec3 r);
    void update(glm::vec3 Fc, glm::vec3 Tc);

};

struct ParticleData {
    glm::vec3 r0, r; //initial relative position, should be relative to CENTER OF MASS, r = current rel pos
    glm::vec3 v; //particle velocity;

    glm::vec3 fd, fs, ft;

    float d; // diameter
    float mass = 1.0f;

    float k; //spring constant
    float n; //damp constant, assume all particles are of same size

    ObjectData* objData = nullptr;
    glm::vec3 gravity = glm::vec3(0,9.81f,0);
    //glm::vec3 gravity = glm::vec3(0, 0.001f, 0);

    ParticleData();
    ParticleData(glm::vec3 partPos_, float radius_, ObjectData* objData_);
    void update();
    void addCollision(glm::vec3 o_v, glm::vec3 o_r, float o_mass = 1.f, bool canMove = true);

    glm::vec3 getFc();
    glm::vec3 getTc();
    glm::vec3 actualPos();
};

/*class Physics {

public:

    Physics() {

    }

};*/


#endif
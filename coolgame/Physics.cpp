#include "Physics.h"
#include <glm/gtx/quaternion.hpp>

#include <iostream>

using namespace std;

void ObjectData::prepareData(float mass_, glm::vec3 r) {
    mass += mass_;
    float Ixx = mass_ * (r.y*r.y + r.z*r.z);
    float Iyy = mass_ * (r.x*r.x + r.z*r.z);
    float Izz = mass_ * (r.x*r.x + r.y*r.y);
    float Iyx = -mass_ * r.x * r.y;
    float Izx = -mass_ * r.x * r.z;
    float Izy = -mass_ * r.y * r.z;

    I0 += glm::mat3(
        Ixx, Iyx, Izx,
        Iyx, Iyy, Izy,
        Izx, Izy, Izz);

    //I0_inv = invMat(I0);
    I0_inv = glm::inverse(I0);

}

std::string getVecStr2(glm::vec3 adata) {
    return std::string("[" + to_string(adata.x) + "," + to_string(adata.y) + "," + to_string(adata.z) + "]");
}

void ObjectData::update(glm::vec3 Fc, glm::vec3 Tc) {

    glm::mat4 RM = glm::toMat4(rotation);
    glm::mat4 I0_inv4 = glm::mat4(I0_inv);
    glm::mat4 RM_T = glm::transpose(RM);

  //  cout << "A " << I0_inv4[0][0] << endl;
    It_inv = glm::mat3(RM * I0_inv4 * RM_T);

    Fc /= 8000.f*15;
    Tc /= 8000.f*15;

    F = Fc;

    V += F / mass;
    W += It_inv * Tc;

    glm::vec3 a = W / glm::length(W);
    float ang = glm::length(W);
    glm::quat dq(cosf(ang / 2), a*sinf(ang / 2));
    rotation = glm::cross(dq, rotation);
    rotation = glm::normalize(rotation);

    //if(V.x != 0 || V.z != 0)
    //cout << "TEST " << getVecStr2(V) << endl;
    position += V;

}

ParticleData::ParticleData(){}

ParticleData::ParticleData(glm::vec3 partPos_, float radius_, ObjectData* objData_) {
    objData = objData_;
    r0 = partPos_ - objData_->centerOfMass;
    d = radius_ * 2;
    k = (mass*glm::length(gravity)) / (d * 2);
    n = 1 * 2 * sqrt(2 * k);
    update();
}

void ParticleData::update() {
    glm::quat Qs = glm::conjugate(objData->rotation);
    glm::quat Q = objData->rotation;
    r = Q * r0;// * Qs; //this is different from the instructions.
    v = objData->V + glm::cross(objData->W, r);
    fd = fs = ft = glm::vec3(0, 0, 0);


}

void ParticleData::addCollision(glm::vec3 o_v, glm::vec3 o_r, float o_mass, bool canMove) {
    glm::vec3 rij = o_r - actualPos();
    glm::vec3 vij = o_v - v;
    float rij_len = glm::length(rij);
    if (rij_len == 0)
        return;

    fs += (-k * (d - rij_len)) * (rij / rij_len);
    fd += n * vij;

    glm::vec3 vij_t = vij - (glm::dot(vij, rij / rij_len) * (rij / rij_len));
    ft += k * vij_t;
}

glm::vec3 ParticleData::getFc() {
    return fd + fs + ft + (gravity*(-1.f)*mass);
}
glm::vec3 ParticleData::getTc() {
    return glm::cross(r, (getFc()));
}

glm::vec3 ParticleData::actualPos() {
    return objData->position + objData->centerOfMass + r;
}

void ParticleData::collisionParticle(ParticleData& po) {

    float radiusSum = objData->partRadius + po.objData->partRadius;
    glm::vec3 pos1 = actualPos();
    glm::vec3 pos2 = po.actualPos();

    glm::vec3 collisionNormal = pos2 - pos1;
    float distancesq = glm::length2(collisionNormal);
    if (distancesq <= radiusSum * radiusSum) { //This collision check was inspired by "Essential Mathematics for Games and Interactive Applications"
        collisionNormal = glm::normalize(collisionNormal);
        float distance = sqrtf(distancesq);
        float penetration = radiusSum - distance;

        objData->position -= 0.5f*penetration*collisionNormal;
        po.objData->position += 0.5f*penetration*collisionNormal;

        glm::vec3 relVelocity = v - po.v;
        float vDotN = glm::dot(relVelocity,collisionNormal);
        if (vDotN < 0)
            return;

        glm::vec3 rij = pos2 - pos1;
        glm::vec3 vij = po.v - v;
        float rij_len = glm::length(rij);
        if (rij_len == 0)
            return;

        glm::vec3 vij_t = vij - (glm::dot(vij, rij / rij_len) * (rij / rij_len));
        glm::vec3 fsTmp = (-k * (d - rij_len)) * (rij / rij_len);
        glm::vec3 fdTmp = n * vij;
        glm::vec3 ftTmp = k * vij_t;

        fs += fsTmp;
        fd += fdTmp;
        ft += ftTmp;
        po.fs -= fsTmp;
        po.fd -= fdTmp;
        po.ft -= ftTmp;
        
    }

}
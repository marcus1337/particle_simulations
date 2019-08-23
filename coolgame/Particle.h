#ifndef PARTICLE_H
#define PARTICLE_H

#include "ShaderShape.h"
#include <string>
#include "Physics.h"

class Particle {

    void drawShader(ShaderShape*, glm::mat4& MVP);

public:
    ShaderShape* shader;
    ShaderShape* hitShader;

    float radius = 1.0f;
    float scaleVal = 1.0f;

    bool isColliding = false;

    Particle() {};
    Particle(ShaderShape* shader_) : position(0, 0, 0), radius(0),
    shader(shader_){}
    Particle(ShaderShape* shader_, glm::vec3 pos_, float radius_, float scaleVal_) : position(pos_), radius(radius_), // lx(0), ly(0), lz(0),
        shader(shader_), scaleVal(scaleVal_) {}
    Particle(ParticleData partData_, ShaderShape* shader_, glm::vec3 pos_, float radius_, float scaleVal_) : position(pos_), radius(radius_), // lx(0), ly(0), lz(0),
        shader(shader_), scaleVal(scaleVal_), partData(partData_){}

    void setTrans(float, float, float);
    void addTrans(float, float, float);
    std::string getCoordStr();

    bool checkCollision(Particle& otherParticle);
    static bool checkCollision(glm::vec3 pos1, glm::vec3 pos2, float radius1, float radius2);

    void printStuff();
    void draw(glm::mat4& VP);
    void draw2(glm::mat4& VP, glm::mat4& P, glm::mat4& V);

    glm::mat4 model;
    glm::vec3 position;
    glm::vec3* cameraPos;

    int* screenWidth = nullptr;
    int* screenHeight = nullptr;

    float alpha = 1.f;
    int indice = -1;
    ParticleData partData;

};

#endif
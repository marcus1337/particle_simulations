#ifndef PARTICLEOBJECT_H
#define PARTICLEOBJECT_H

#include <string>
#include <vector>
#include "ShaderShape.h"
#include "Particle.h"

#include "ParticleCreator.h"
#include "ParticleShader.h"

class ParticleObject {
public:
    float partDiameter = 1.0f;
    float partRadius = 1.0f;
    glm::vec4 color;
    glm::vec3 position = glm::vec3(0.f,0.f,0.f);
    int *screenWidth = nullptr; 
    int *screenHeight = nullptr;

    ShaderShape* shaderShape = nullptr;
    ShaderShape* particleShader = nullptr;
    ParticleShader* particleShadCreator = nullptr;
    int partShadIndex = -1;

    ParticleObject();
    void init(float partDiameter_, ShaderShape* shaderShape_, ShaderShape* particleShader_, ShaderShape* particleShader2_);
    
    void init2(float partRadius, float scaleVal, glm::vec4 color_ , ShaderShape* shaderShape_, ParticleShader* particleShadCreator_);


    std::vector<Particle> particles;

    void draw(glm::mat4& VP);
    void draw2(glm::mat4& VP, glm::mat4& P);
    void draw3(glm::mat4& VP, glm::mat4& P);

};


#endif
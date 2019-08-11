#ifndef SHAPE_H
#define SHAPE_H

#include "ShaderShape.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Shape {

    ShaderShape& shader;

public:
    Shape(ShaderShape& _shader);
    void draw(glm::mat4& VP);
    glm::mat4 model;

    glm::vec3 mscale;
    glm::vec3 position;
    glm::quat orientation;

};


#endif
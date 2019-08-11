#ifndef PARTICLECREATOR_H
#define PARTICLECREATOR_H

#include "MiscCode/utils.h"
#include "MiscCode/utils2.h"
#include "MiscCode/utils3.h"

class ParticleCreator {

public:

    std::vector<glm::vec3>& vertices;
    float xmin, ymin, zmin;
    float xmax, ymax, zmax;
    float voxLen = 1.f;
    float midVoxVal = 1.f;

    std::vector<std::vector<std::vector<float>>> zvalue_hits; //x * y 2D int array for z value hits, to be filled with unsorted integers, 0 means no particles
    glm::vec3 numBoxIndices; //tells how many actual boxes we decided to make in each direction decided by voxLen (rounding down)
    glm::vec3 cubeDimensions; //tells the unsigned length of the box from smallest point to largest point (not point but x,y,z)

    float checkLineTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 lineA, glm::vec3 lineB, bool& didHit_);
    static bool sameFloat(float f1, float f2);
    static int signedVolume(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);


    std::vector<std::vector<std::vector<bool>>> allHitVoxels;

    ParticleCreator(std::vector<glm::vec3>& _vertices, float voxLen_);
    std::vector<glm::vec3> createParticles();
    std::vector<glm::vec3> createParticles2();


};


#endif
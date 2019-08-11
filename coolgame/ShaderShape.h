#ifndef SHADERSHAPE_H
#define SHADERSHAPE_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "MiscCode/utils3.h";


class ShaderShape {
    std::string path3D = "C:\\Users\\Marcus\\source\\repos\\coolgame\\Debug\\3d_models\\";

public:
    Context ctx;
    GLuint shaderProgram;

    //std::string objName = "teapot.obj";
    std::string objName = "low-poly-fox-by-pixelmannen.obj";
    std::string objName2 = "icosphere.obj";

    std::string vertShadName = "mesh.vert";
    std::string fragShadName = "mesh.frag";

    void init();

    void initParticleShader1();

    void cleanup();

};


#endif
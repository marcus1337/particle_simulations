#pragma once
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include "EnumStuff.h"
#include "IOstuff.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <fstream>
#include <sstream>

#include "MyWindow.h"

#include "ShaderShape.h"
#include "Shape.h"

#include "SkyBox.h"

#include "ParticleObject.h"
#include "CommandHandler.h"
#include "ParticleShader.h"

#include "Floor.h"

class GameHandler{

    MyWindow mywindow;
    ShaderShape shapeshader, particleShader, particleShader2;
    ShaderShape particleTestShader;
    ParticleShader partShadCreator, partShadCreator2;
    Floor floor;

    std::vector<Shape> shapes;
    SkyBox skybox;
    CommandHandler commandhandler;

    const int MS_FRAME = 15;
    double lag = 0.0f;
    double current, previous;

    std::vector<ParticleObject> particleObjs;

    Particle testParticle, testParticle2, tp3;

    //Particle tps[125];

public:

    GameHandler();
    ~GameHandler();

    bool gameTick();
    void render();
    void run();

    void checkCollisions();

};
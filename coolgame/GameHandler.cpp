#include "GameHandler.h"


using namespace std;

GameHandler::GameHandler() {
    bool errorCheck = mywindow.initWindow();
    if (errorCheck)
        exit(EXIT_FAILURE);

    //Initialize OBJ-file shapes
    skybox.init(&mywindow);
    shapeshader.init();
    shapes.push_back(Shape(shapeshader));
    shapes[0].mscale = glm::vec3(0.01f, 0.01f, 0.01f);


    /////////////Initialize fox-objects, initial velocities, colors, etc.
    particleObjs.push_back(ParticleObject());
    particleObjs[0].screenWidth = &mywindow.SCR_WIDTH;
    particleObjs[0].screenHeight = &mywindow.SCR_HEIGHT;
    particleObjs[0].init2(17.f, 0.01f, glm::vec4(1, 1, 0, 1.0f), &shapeshader, &partShadCreator);

    particleObjs.push_back(ParticleObject());
    particleObjs[1].screenWidth = &mywindow.SCR_WIDTH;
    particleObjs[1].screenHeight = &mywindow.SCR_HEIGHT;
    particleObjs[1].init2(17.f, 0.01f, glm::vec4(1, 0, 0, 1.0f), &shapeshader, &partShadCreator);
    particleObjs[1].objData.position.y = 3;

    particleObjs.push_back(ParticleObject());
    particleObjs[2].screenWidth = &mywindow.SCR_WIDTH;
    particleObjs[2].screenHeight = &mywindow.SCR_HEIGHT;
    particleObjs[2].init2(17.f, 0.01f, glm::vec4(0.7, 0, 1, 1.0f), &shapeshader, &partShadCreator);
    particleObjs[2].objData.position.y = 2;
    particleObjs[2].objData.position.z = -2;
    particleObjs[2].objData.V = glm::vec3(0, 0, 0.005f);
    ////////////////////////////////////////////////////////////////////

    floor.init(); //Quad with texture for rendering a floor

    mywindow.commandhandler = &commandhandler; //Can be used for controlling the position of an object, not used now.
    previous = current = glfwGetTime() * 1000;

}

/*Clear up the memory.*/
GameHandler::~GameHandler() {
    shapeshader.cleanup();
    particleShader.cleanup();
    for (auto po : programs) {
        glDeleteProgram(po.second);
    }
}

/*Makes retrieving particle data less verbose.*/
ParticleData& GameHandler::getP(int objInd, int partInd) {
    return particleObjs[objInd].particles[partInd].partData;
}

void GameHandler::checkCollisions() {

    CollisionReducer collRed(particleObjs[0].partRadius); //Makes collision detection O(n) instead of O(n^2)

    for (int i = 0; i < particleObjs.size(); i++) { //Initialize grid voxel data with particles
        for (int j = 0; j < particleObjs[i].particles.size(); j++) {
            auto& part = particleObjs[i].particles[j];
            glm::vec3 pPos = part.partData.actualPos();
            collRed.addPart(i, j, pPos.x, pPos.y, pPos.z);
        }
    }

    for (auto& it : collRed.voxels) { //Check all voxels both individually and their adjacent voxels for collision detection
        const KeyXYZ& a = it.first;

        for (int i = 0; i < collRed.voxels[a].size(); i++) {
            for (int j = i+1; j < collRed.voxels[a].size(); j++) {
                int64_t obInd1 = collRed.voxels[a][i].objIndex;
                int64_t obInd2 = collRed.voxels[a][j].objIndex;
                int64_t parInd1 = collRed.voxels[a][i].partIndex;
                int64_t parInd2 = collRed.voxels[a][j].partIndex;

                if (obInd1 != obInd2) {
                    auto& p = getP(obInd1, parInd1);
                    auto& p2 = getP(obInd2, parInd2);
                    p.collisionParticle(p2);
                }
            }
        }

        for (int64_t ix = a.x - 1; ix <= a.x + 1; ix++) {
            for (int64_t iy = a.y - 1; iy <= a.y + 1; iy++) {
                for (int64_t iz = a.z - 1; iz <= a.z + 1; iz++) {
                     KeyXYZ b(ix, iy, iz);
                      if (collRed.mapHas(b)) {
                          
                          for (int i = 0; i < collRed.voxels[a].size(); i++) {
                              for (int j = 0; j < collRed.voxels[b].size(); j++) {
                                  int64_t obInd1 = collRed.voxels[a][i].objIndex;
                                  int64_t obInd2 = collRed.voxels[b][j].objIndex;
                                  int64_t parInd1 = collRed.voxels[a][i].partIndex;
                                  int64_t parInd2 = collRed.voxels[b][j].partIndex;

                                  if (obInd1 != obInd2) {
                                      auto& p = getP(obInd1, parInd1);
                                      auto& p2 = getP(obInd2, parInd2);
                                      p.collisionParticle(p2);
                                  }
                              }
                          }

                      }
                }
            }
        }
    }

    //Check collision with the floor
    for (int i = 0; i < particleObjs.size(); i++) {
        for (Particle& part : particleObjs[i].particles) {
            glm::vec3 pos = part.partData.actualPos();
            if (pos.y - part.radius < floor.position.y) {
                particleObjs[i].objData.V.y /= 2;
                part.partData.addCollision(glm::vec3(part.partData.v.x, part.partData.v.y + part.partData.gravity.y, part.partData.v.z), glm::vec3(pos.x, pos.y - part.radius / 2, pos.z), 1.f, false);
                particleObjs[i].objData.position.y += floor.position.y - (pos.y - part.radius);
            }
        }
    }

    //Apply translations, rotations, etc by calculating the physics from particles.
    for (int i = 0; i < particleObjs.size(); i++) {
        particleObjs[i].doPhysics();
    }

}

/*Draw every object to the screen here.*/
void GameHandler::render() {
    glm::mat4 VP = mywindow.projection * mywindow.view;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    skybox.draw(VP);
    glEnable(GL_DEPTH_TEST);

    //shapes[0].draw(VP); //can be used for rendering OBJ-files

    for (int i = 0; i < particleObjs.size(); i++) {  //Render VBO objects
        particleObjs[i].draw2(VP, mywindow.projection, mywindow.camera.Position);
    }

    floor.draw(VP, mywindow.projection, mywindow.camera.Position);
    glfwSwapBuffers(mywindow.window);
}


/*Makes each time step consistent*/
bool GameHandler::gameTick() {
    bool anUpdate = false;
    current = glfwGetTime() * 1000;
    double diff = current - previous;
    previous = current;
    lag += diff;
    anUpdate = false;

    while (lag >= MS_FRAME) {
        lag -= MS_FRAME;
        mywindow.processActions();
        checkCollisions();
        anUpdate = true;
    }
    return anUpdate;
}

#include <windows.h> //windows.h used for Sleep()

void GameHandler::run() {
    while (!glfwWindowShouldClose(mywindow.window))
    {
        Sleep(5);
        glfwPollEvents();

        bool anUpdate = gameTick();

        if (anUpdate) {
            mywindow.handleFrame();
            render();
        }
    }
}
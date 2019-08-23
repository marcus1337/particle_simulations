#include "GameHandler.h"


using namespace std;
using namespace FileNames;

GameHandler::GameHandler() {
    bool errorCheck = mywindow.initWindow();
    if (errorCheck)
        exit(EXIT_FAILURE);

   // getchar();

    skybox.init(&mywindow);
    shapeshader.init();
    shapes.push_back(Shape(shapeshader));
    shapes[0].mscale = glm::vec3(0.01f, 0.01f, 0.01f);

    particleShader.objName = "icosphere2.obj";
    particleShader.init();
    particleShader2.objName = "icosphere2.obj";
    particleShader2.fragShadName = "particle.frag";
    particleShader2.vertShadName = "particle.vert";
    particleShader2.init();


    /////////////
    particleObjs.push_back(ParticleObject());
    particleObjs[0].screenWidth = &mywindow.SCR_WIDTH;
    particleObjs[0].screenHeight = &mywindow.SCR_HEIGHT;
    particleObjs[0].init2(17.f, 0.01f, glm::vec4(1, 1, 0, 1.0f), &shapeshader, &partShadCreator);
   // particleObj.init(17.0f, &shapeshader, &particleShader, &particleShader2);

    particleObjs.push_back(ParticleObject());
    //particleObjs[1].copyObject(particleObjs[0]);
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

   /* particleObjs.push_back(ParticleObject());
    particleObjs[3].screenWidth = &mywindow.SCR_WIDTH;
    particleObjs[3].screenHeight = &mywindow.SCR_HEIGHT;
    particleObjs[3].init2(17.f, 0.01f, glm::vec4(0.7, 0.5, 1, 1.0f), &shapeshader, &partShadCreator);
    particleObjs[3].objData.position.x = 5;
    particleObjs[3].objData.position.y = 1;
    particleObjs[3].objData.position.z = 0;
    particleObjs[3].objData.V = glm::vec3(-0.020, 0, 0);
    particleObjs[3].objData.rotation = glm::quat(glm::vec3(90.f, 45.f, 0.f));*/


    floor.init();

    //////////////
    particleTestShader.initParticleShader1();
    
    testParticle.shader = &particleTestShader;
    testParticle.position = glm::vec3(1, 1, 0);
    testParticle.radius = 17.0f / 2.0f;
    testParticle.radius *= 0.01f;
    testParticle.radius /= 2;
    testParticle.scaleVal = 0.01f*testParticle.radius;
    testParticle.cameraPos = &mywindow.camera.Position;
    testParticle.screenWidth = &mywindow.SCR_WIDTH;
    testParticle.screenHeight = &mywindow.SCR_HEIGHT;

    testParticle2.shader = &particleTestShader;
    testParticle2.position = glm::vec3(2, 1, 0);
    testParticle2.radius = 17.0f / 2.0f;
    testParticle2.scaleVal = 0.01f*testParticle.radius;
    testParticle2.cameraPos = &mywindow.camera.Position;
    testParticle2.screenWidth = &mywindow.SCR_WIDTH;
    testParticle2.screenHeight = &mywindow.SCR_HEIGHT;

   // testParticle.radius = 0.5f;
    testParticle2.radius = 0.5f;

    /*for (int i = 0; i < 125; i++) {
        tps[i] = testParticle;
        tps[i].isColliding = true;
    }*/

    commandhandler.movablePosition = &testParticle.position;
    mywindow.commandhandler = &commandhandler;

    //shapes[0].position.x = 200;
   // shapes[0].position.z = 130;
    //shapes[0].orientation = glm::quat(0.7071067f, 0, 0.7071067f, 0);
    previous = current = glfwGetTime() * 1000;
    // double starttime = glfwGetTime();
    // double endtime = glfwGetTime();

}

GameHandler::~GameHandler() {
    shapeshader.cleanup();
    particleShader.cleanup();
    for (auto po : programs) {
        glDeleteProgram(po.second);
    }
}

std::string getVecStr3(glm::vec3 adata) {
    return std::string("[" + to_string(adata.x) + "," + to_string(adata.y) + "," + to_string(adata.z) + "]");
}

ParticleData& GameHandler::getP(int objInd, int partInd) {
    return particleObjs[objInd].particles[partInd].partData;
}

void GameHandler::checkCollisions() {

    CollisionReducer collRed(particleObjs[0].partRadius);

    for (int i = 0; i < particleObjs.size(); i++) {
        for (int j = 0; j < particleObjs[i].particles.size(); j++) {
            auto& part = particleObjs[i].particles[j];
            glm::vec3 pPos = part.partData.actualPos();
            collRed.addPart(i, j, pPos.x, pPos.y, pPos.z);
        }
    }

    int counter = 0;
    for (auto& it : collRed.voxels) {
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

   // cout << "co " << counter << endl;


   /* int counter = 0;
    for (auto& it : collRed.voxels) {
        for (auto& a : it.second.storedParticles) {
            for (auto& b : it.second.storedParticles) {
                if (a.objIndex != b.objIndex) {
                    particleObjs[a.objIndex].particles[a.partIndex].
                        partData.collisionParticle(particleObjs[b.objIndex].particles[b.partIndex].partData);
                }
            }
        }
    }*/
           // auto& p = getP(a.objIndex, a.partIndex);
           // for (int64_t ix = a.kx - 1; ix <= a.kx + 1; ix++) {
             //   for (int64_t iy = a.ky - 1; iy <= a.ky + 1; iy++) {
               //     for (int64_t iz = a.kz - 1; iz <= a.kz + 1; iz++) {
                        //counter++;
                       // KeyXYZ key1(ix, iy, iz);
                      //  if (collRed.mapHas(key1)) {

                       // }

                 //   }
                //}
           // }



    //if(collRed.potentialCollisions.size() > 0)
     //   cout << "COLLS: " << collRed.potentialCollisions.size() << endl;

   // testParticle.isColliding = false;
   // testParticle.isColliding = testParticle.checkCollision(testParticle2);
    //int counter = 0;
    //testParticle.partData.v = glm::vec3(20, 0, 0);


  //  cout << "WAT " << particleObjs[0].objData.position.y << endl;

    /*for (int i = 0; i < particleObjs.size(); i++) {
        for (int j = i+1; j < particleObjs.size(); j++) {
            for (Particle& part : particleObjs[i].particles) {
                for (Particle& part2 : particleObjs[j].particles) {
                    part.partData.collisionParticle(part2.partData);
                }
            }
        }
    }*/

   // for (Particle& part : particleObj[0].particles) {
        //tp3.position = part.partData.actualPos();
        //tps[counter].position = part.partData.actualPos();
        //counter++;
        //if (Particle::checkCollision(part.partData.actualPos(), testParticle.position, part.radius,testParticle.radius)) {
        //    testParticle.isColliding = true;
        //    part.partData.addCollision(glm::vec3(20, 0, 0), testParticle.position);
        //cout << "TEST " << part.position.x << "," << part.position.y << "," << part.position.z << endl;
        //}
       // for (Particle& part2 : particleObj[1].particles) {
            //if (Particle::checkCollision(part.partData.actualPos(), part2.partData.actualPos(), part.partData.objData->partRadius, part2.partData.objData->partRadius)) {
                //cout << "TEST: " << getVecStr3(part.partData.actualPos()) << " _ " << getVecStr3(part2.partData.actualPos()) << " R_ " << part.partData.objData->partRadius << " _ " << part2.partData.objData->partRadius << endl;
              //    part.partData.collisionParticle(part2.partData);
        //    }
           
      //  }
   // }

    //Check collision with the floor
    for (int i = 0; i < particleObjs.size(); i++) {
        for (Particle& part : particleObjs[i].particles) {

            glm::vec3 pos = part.partData.actualPos();
            if (pos.y - part.radius < floor.position.y) {
                //  part.partData.addCollision(particleObj.objData.V*-1.f, glm::vec3(pos.x, pos.y - part.radius / 2, pos.z), 1.f, false);

                particleObjs[i].objData.V.y /= 2;
                part.partData.addCollision(glm::vec3(part.partData.v.x, part.partData.v.y + part.partData.gravity.y, part.partData.v.z), glm::vec3(pos.x, pos.y - part.radius / 2, pos.z), 1.f, false);

                //part.partData.addCollision(glm::vec3(0, 9.81, 0), glm::vec3(pos.x, pos.y - part.radius/2, pos.z), 1.f, false);
                particleObjs[i].objData.position.y += floor.position.y - (pos.y - part.radius);
                //cout << "TEST: " << (floor.position.y) << " r " << part.radius << " _ " << pos.y << endl;
            }
        }
    }

   // cout << "TEST " << particleObj.objData.V.y << endl;

    for (int i = 0; i < particleObjs.size(); i++) {
        particleObjs[i].doPhysics();
    }

}

void GameHandler::render() {
    glm::mat4 VP = mywindow.projection * mywindow.view;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    skybox.draw(VP);

    glEnable(GL_DEPTH_TEST);

    testParticle.draw2(VP, mywindow.projection, mywindow.view);
    testParticle2.draw2(VP, mywindow.projection, mywindow.view);
    //tp3.draw2(VP, mywindow.projection, mywindow.view);

   // for (int i = 0; i < 125; i++) {
   //     tps[i].draw2(VP, mywindow.projection, mywindow.view);
   // }
    
    for (int i = 0; i < particleObjs.size(); i++) {
        particleObjs[i].draw2(VP, mywindow.projection, mywindow.camera.Position);
    }


    for (Shape& shape : shapes) {
   //     shape.draw(VP);
    }

    floor.draw(VP, mywindow.projection, mywindow.camera.Position);

    glfwSwapBuffers(mywindow.window);
}

bool GameHandler::gameTick() {
    bool anUpdate = false;
    current = glfwGetTime() * 1000;
    double diff = current - previous;
    previous = current;
    lag += diff;
    anUpdate = false;
    //cout << "TEST " << diff << " _ " << lag << endl;
    while (lag >= MS_FRAME) {
        lag -= MS_FRAME;
        mywindow.processActions();
        checkCollisions();
        anUpdate = true;
    }
    return anUpdate;
}


void GameHandler::run() {
    while (!glfwWindowShouldClose(mywindow.window))
    {
        //starttime = glfwGetTime()*1000;
        Sleep(5);

        glfwPollEvents();

        bool anUpdate = gameTick();

        if (anUpdate) {
            mywindow.handleFrame();
            render();

            /*double diff = starttime - endtime;
            endtime = starttime;
            cout << "DIFF " << 1000.0f/diff << endl;*/
        }
    }
}
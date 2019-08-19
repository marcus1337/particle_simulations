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

void GameHandler::checkCollisions() {

   // testParticle.isColliding = false;
   // testParticle.isColliding = testParticle.checkCollision(testParticle2);
    //int counter = 0;
    //testParticle.partData.v = glm::vec3(20, 0, 0);


  //  cout << "WAT " << particleObjs[0].objData.position.y << endl;

    for (int i = 0; i < particleObjs.size(); i++) {
        for (int j = i+1; j < particleObjs.size(); j++) {
            for (Particle& part : particleObjs[i].particles) {
                for (Particle& part2 : particleObjs[j].particles) {
                    part.partData.collisionParticle(part2.partData);
                }
            }
        }
    }

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

              //  if (i == 0) {
             //       cout << "EY " << getVecStr3(part.partData.v) << " _ " << getVecStr3(pos) << endl;
             //   }

                particleObjs[i].objData.V.y /= 2;
                part.partData.addCollision(glm::vec3(part.partData.v.x, part.partData.v.y + 9.81 / 2, part.partData.v.z), glm::vec3(pos.x, pos.y - part.radius / 2, pos.z), 1.f, false);
               // cout << "TEST " << part.radius << endl;

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
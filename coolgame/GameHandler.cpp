#include "GameHandler.h"


using namespace std;
using namespace FileNames;

GameHandler::GameHandler() {
    bool errorCheck = mywindow.initWindow();
    if (errorCheck)
        exit(EXIT_FAILURE);

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
    particleObj.screenWidth = &mywindow.SCR_WIDTH;
    particleObj.screenHeight = &mywindow.SCR_HEIGHT;
    particleObj.init2(17.0f,0.01f,glm::vec4(1,1,0,1),&shapeshader, &partShadCreator);
    

    //////////////
    particleTestShader.initParticleShader1();
    
    testParticle.shader = &particleTestShader;
    testParticle.position = glm::vec3(1, 1, 0);
    testParticle.radius = 17.0f / 2.0f;
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

    testParticle.radius = 0.5f;
    testParticle2.radius = 0.5f;


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
}

void GameHandler::checkCollisions() {

    testParticle.isColliding = testParticle.checkCollision(testParticle2);

    /*for (Particle& part : particleObj.particles) {
        if (part.checkCollision(testParticle)) {
            part.isColliding = true;
        }
        else {
            part.isColliding = false;
        }
        break;
    }*/
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

    particleObj.draw2(VP, mywindow.projection);

    //glDepthMask(GL_FALSE);
    testParticle.draw2(VP, mywindow.projection, mywindow.view);
    testParticle2.draw2(VP, mywindow.projection, mywindow.view);
   // glDepthMask(GL_TRUE);

    for (Shape& shape : shapes) {
    //    shape.draw(VP);
    }

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
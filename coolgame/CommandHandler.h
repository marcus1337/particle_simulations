#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class CommandHandler {

    bool movRight, movLeft, movUp, movDown;
    bool movForward, movBack;

public:

    CommandHandler() {
        movRight = movLeft = movUp = movDown = false;
        movForward = movBack = false;
    }

    glm::vec3* movablePosition = nullptr;
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void doActions();

};


#endif
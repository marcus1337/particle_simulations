#include "CommandHandler.h"

void CommandHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //if (movablePosition != nullptr) {}

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        movLeft = true;
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE) {
        movLeft = false;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        movRight = true;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) {
        movRight = false;
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        movUp = true;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE) {
        movUp = false;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        movDown = true;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
        movDown = false;
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        movForward = true;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_RELEASE) {
        movForward = false;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        movBack = true;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_RELEASE) {
        movBack = false;
    }

}


void CommandHandler::doActions() {
    float velocity = 0.03f;

    if (movablePosition != nullptr) {
        if (movUp) {
            movablePosition->y -= 1.0f*velocity;
        }
        if (movDown) {
            movablePosition->y += 1.0f*velocity;
        }
        if (movLeft) {
            movablePosition->x -= 1.0f*velocity;
        }
        if (movRight) {
            movablePosition->x += 1.0f*velocity;
        }
        if (movForward) {
            movablePosition->z -= 1.0f*velocity;
        }
        if (movBack) {
            movablePosition->z += 1.0f*velocity;
        }

    }
}
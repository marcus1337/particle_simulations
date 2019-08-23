#ifndef MYWINDOW_H
#define MYWINDOW_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include "CommandHandler.h"

class MyWindow {

public:

    CommandHandler* commandhandler = nullptr;

    bool quitProgram = false;
    bool goForward = false;
    bool goBack = false;
    bool goLeft = false;
    bool goRight = false;
    bool rotateRight = false;
    bool rotateLeft = false;

    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;
    GLFWwindow* window;

    // settings
    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 600;

    // camera
    Camera camera;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    MyWindow();
    ~MyWindow();

    //Callback functions for keyboard, mouse, window events...
    inline static auto mouse_callback(GLFWwindow* window, double xpos, double ypos);
    inline static auto scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    inline static auto key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    inline static auto window_size_callback(GLFWwindow* window, int width, int height);

    void processActions(); //process keyboard inputs
    bool initWindow();

    glm::mat4 projection, view;
    void handleFrame();

    float P11 = 1.f; //used for gl_pointsize calculations

};

#endif

#include "MyWindow.h"
#include <iostream>

MyWindow::MyWindow():camera(glm::vec3(0.0f, 0.0f, 3.0f)){}

using namespace std;

auto MyWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    MyWindow* mywindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

    if (mywindow->commandhandler != nullptr) {
        mywindow->commandhandler->key_callback(window, key, scancode, action, mods);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        mywindow->quitProgram = true;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        mywindow->goForward = true;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        mywindow->goForward = false;
    }
        //mywindow->camera.ProcessKeyboard(FORWARD, mywindow->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        mywindow->goBack = true;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        mywindow->goBack = false;
    }
        //mywindow->camera.ProcessKeyboard(BACKWARD, mywindow->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mywindow->goLeft = true;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        mywindow->goLeft = false;
    }
        //mywindow->camera.ProcessKeyboard(LEFT, mywindow->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        mywindow->goRight = true;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        mywindow->goRight = false;
    }
       // mywindow->camera.ProcessKeyboard(RIGHT, mywindow->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        mywindow->rotateRight = true;
    }
        //mywindow->camera.ProcessKeyboard(ROTATERIGHT, mywindow->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        mywindow->rotateLeft = true;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        mywindow->rotateRight = true;
    }
    //mywindow->camera.ProcessKeyboard(ROTATERIGHT, mywindow->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        mywindow->rotateLeft = true;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
        mywindow->rotateRight = false;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) {
        mywindow->rotateLeft = false;
    }

        //mywindow->camera.ProcessKeyboard(ROTATELEFT, mywindow->deltaTime);

}

auto MyWindow::window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    MyWindow* mywindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));
    mywindow->SCR_WIDTH = width;
    mywindow->SCR_HEIGHT = height;


    // glMatrixMode(GL_PROJECTION);
   //  glLoadIdentity();
   //  perspectiveGL(45.0, (float)width / (float)height, 1.0, 500.0);
}


auto MyWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    MyWindow* mw = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));
    if (mw->firstMouse)
    {
        mw->lastX = xpos;
        mw->lastY = ypos;
        mw->firstMouse = false;
    }

    float xoffset = xpos - mw->lastX;
    float yoffset = mw->lastY - ypos; // reversed since y-coordinates go from bottom to top
    mw->lastX = xpos;
    mw->lastY = ypos;
    mw->camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
auto MyWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    MyWindow* mw = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));
    mw->camera.ProcessMouseScroll(yoffset);
}

bool MyWindow::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
   

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_WIDTH);
    glfwSetWindowSizeCallback(window, window_size_callback);

    cout << "width: " << SCR_WIDTH << " height: " << SCR_HEIGHT << endl;

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    return EXIT_SUCCESS;
}

void MyWindow::handleFrame() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
       
    //fov_y == 2 arctan h / (2*fy)
    if (SCR_HEIGHT == 0)
        SCR_HEIGHT = 1;

    P11 = 1.f/ tanf(glm::radians(camera.Zoom)/2.f);
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    view = camera.GetViewMatrix();
}

MyWindow::~MyWindow() {
    glfwTerminate();
}

void MyWindow::processActions() {

    if(goForward)
        camera.ProcessKeyboard(FORWARD, 0);
    if (goBack)
        camera.ProcessKeyboard(BACKWARD, 0);
    if (goLeft)
        camera.ProcessKeyboard(LEFT, 0);
    if (goRight)
        camera.ProcessKeyboard(RIGHT, 0);
    if (rotateLeft) {
        camera.ProcessKeyboard(ROTATELEFT, 0);
    }
    if (rotateRight) {
        camera.ProcessKeyboard(ROTATERIGHT, 0);
    }

    if (commandhandler != nullptr)
        commandhandler->doActions();

}
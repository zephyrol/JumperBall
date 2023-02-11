/*
 * File: main.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 septembre 2019, 20:52
 */

#include <cstdlib>
#include <iostream>
#include <scene/MapGenerator.h>
#include "Window.h"


GLFWwindow *initLibraries() {
    GLFWwindow *window;
    if (!glfwInit()) {
        std::cerr << "Failed to init glfw" << std::endl;
    }
    glfwWindowHint(GLFW_RESIZABLE, true);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*window = glfwCreateWindow(RESOLUTION_X,RESOLUTION_Y,
       "JumperBall",glfwGetPrimaryMonitor(),
       nullptr);*/
    window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "JumperBall", nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) {
        std::cerr << "Unable to load OpenGL functions !" << std::endl;
        exit(EXIT_FAILURE);
    }

    return window;

}

void cleanLibraries() {
    glfwTerminate();
}

int main(int argc, char **argv) {

    if (argc == 3) {

        std::ifstream file;
        const std::string fileToOpen = argv[1];
        file.open(fileToOpen); // Opening file to read
        if (!file) {
            std::cerr << "ERROR: Opening " << fileToOpen << " impossible .."
                      << std::endl;
            JBTypesMethods::displayInstallError();
            return EXIT_FAILURE;
        }
        const std::string arg(argv[2]);
        if (arg == "-compress") {
            MapGenerator::compressNew(file);
        } else {
            std::cerr << "ERROR: Unknown option" << std::endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    GLFWwindow *glfwWindow = initLibraries();

    int frameBufferWidth;
    int frameBufferHeight;
    glfwGetFramebufferSize(glfwWindow, &frameBufferWidth, &frameBufferHeight);

    int windowWidth;
    int windowHeight;
    glfwGetWindowSize(glfwWindow, &windowWidth, &windowHeight);

    Window window(glfwWindow, frameBufferWidth, frameBufferHeight, windowWidth, windowHeight);
    window.run();

    cleanLibraries();
    return EXIT_SUCCESS;
}

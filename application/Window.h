/*
 * File: Window.h
 * Author: Morgenthaler S
 *
 * Created on 2 novembre 2019, 11:17
 */

#ifndef WINDOW_H
#define WINDOW_H
#define RESOLUTION_X 1024
#define RESOLUTION_Y 768
#include <Controller.h>
#include <GLFW/glfw3.h>

class Window {
public:

Window(
    GLFWwindow*glfwWindow,
    int frameBufferWidth,
    int frameBufferHeight,
    int windowWidth,
    int windowHeight
);

void run();

private:

GLFWwindow*_window;
int _frameBufferWidth;
int _frameBufferHeight;
int _windowWidth;
int _windowHeight;
const std::vector<unsigned char> _binaryFont;

Controller _controller;

bool inputManagement();
static JBTypes::FileContent createFilesContent();
static std::vector<unsigned char> createBinaryFont();

};

#endif /* WINDOW_H */

/*
 * File: Window.h
 * Author: Morgenthaler S
 *
 * Created on 2 novembre 2019, 11:17
 */

#ifndef WINDOW_H
#define WINDOW_H
#include <system/Types.h>
#include <Utility.h>
#include <Controller.h>
#define RESOLUTION_X 1024
#define RESOLUTION_Y 768

class Window {
public:

Window(GLFWwindow*glfwWindow, int width, int height);

void run();

private:

GLFWwindow*_window;
int _width;
int _height;
Controller _controller;

bool inputManagement();

};

#endif /* WINDOW_H */

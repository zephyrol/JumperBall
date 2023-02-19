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

#include "controller/Controller.h"
#include <GLFW/glfw3.h>

class Window {
public:

    Window(
        GLFWwindow *glfwWindow,
        int frameBufferWidth,
        int frameBufferHeight,
        int windowWidth,
        int windowHeight
    );

    void run();

private:

    GLFWwindow *_window;
    int _frameBufferWidth;
    int _frameBufferHeight;
    int _windowHeight;
    int _windowWidth;
    const std::vector<unsigned char> _binaryFont;

    Controller _controller;

    bool inputManagement();

    static JBTypes::FileContent createFilesContent();

    /**
     * Read a file in the current or binary directories.
     * @param fileName Name of the file
     * @return The content of the file as string.
     */
    static std::string readFile(const std::string& fileName);

    static std::string readSaveFile();

    static std::string extractSpecificPartOfOutput(const std::string& updateOutput, const std::string& key);

    static void writeSaveFile(const std::string& updateOutput);

    static std::vector<unsigned char> createBinaryFont();

};

#endif /* WINDOW_H */

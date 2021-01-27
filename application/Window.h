
/* 
 * File:   Window.h
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

    //--CONSTRUCTORS & DESTRUCTORS--//
    Window(GLFWwindow* glfwWindow);

    //----------METHODS-------------//
    void          run();

private:

    //--------ATTRIBUTES-----------//
    GLFWwindow*   _window;
    Controller    _controller;

    //----------METHODS-------------//
    bool          inputManagement();
    void          runMenu();

    static std::shared_ptr<Menu> initMainMenu(size_t currentLevel);

};

#endif /* WINDOW_H */


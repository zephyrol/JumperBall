/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Window.cpp
 * Author: Morgenthaler S
 *
 * Created on 2 novembre 2019, 11:17
 */

#include <fstream>
#include <istream>
#include <sstream>
#include "Window.h"
#include <initializer_list>


Window::Window(GLFWwindow* glfwWindow):
    _window(glfwWindow),
    _controller()
{
}

bool Window::inputManagement() {

    const std::function<Controller::Status(const std::vector<unsigned int>&&)>
        getButtonStatus = [this](const std::vector<unsigned int>&& keys)
            -> Controller::Status {
                for ( unsigned int key : keys ) {
                    if (glfwGetKey(_window, key) == GLFW_PRESS) {
                        return Controller::Status::Pressed;
                    }
                }
                return Controller::Status::Released;
        };

    // ESCAPE
    const Controller::Status escapeStatus = getButtonStatus({GLFW_KEY_ESCAPE});
    _controller.interactionButtons(Controller::Button::Escape,
                                   escapeStatus);

    // VALIDATE
    const Controller::Status validateStatus = 
        getButtonStatus({ GLFW_KEY_ENTER, GLFW_KEY_SPACE });
    _controller.interactionButtons(Controller::Button::Validate,
                                   validateStatus);
                            
    // RIGHT
    const Controller::Status rightStatus = 
        getButtonStatus({ GLFW_KEY_RIGHT, GLFW_KEY_L });
    _controller.interactionButtons(Controller::Button::Right,
                                   rightStatus);

    // LEFT 
    const Controller::Status leftStatus = 
        getButtonStatus({ GLFW_KEY_LEFT, GLFW_KEY_H });
    _controller.interactionButtons(Controller::Button::Left,
                                   leftStatus);

    // UP 
    const Controller::Status upStatus = 
        getButtonStatus({ GLFW_KEY_UP, GLFW_KEY_K });
    _controller.interactionButtons(Controller::Button::Up,
                                   upStatus);

    // MOUSE BUTTON 1;
    const Controller::Status mouseButton1Status =
        glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS
            ? Controller::Status::Pressed
            : Controller::Status::Released;

    const auto getPosXY = [this]() -> std::pair<float, float> {
        double posX, posY;
        glfwGetCursorPos(_window, &posX, &posY);
        return {static_cast<float>(posX), static_cast<float>(posY)};
    };
    const std::pair<float, float> posXY = getPosXY();

    const float posX = posXY.first / static_cast<float>(RESOLUTION_X);
    const float posY =
        1.f - posXY.second / static_cast<float>(RESOLUTION_Y);
    // GLFW defines y=0 as the top
    _controller.interactionMouse(mouseButton1Status,
                                 posX,
                                 posY);

    const bool exit = glfwWindowShouldClose(_window) != 0 ||
        _controller.requestToLeave();

    return exit;
}


void Window::run() {
    glfwSwapInterval(0);
    glfwSetInputMode(_window, GLFW_STICKY_KEYS,GL_TRUE) ;

    auto before = JBTypesMethods::getTimePointMSNow();
    unsigned int counter = 0;
    while (!inputManagement())
    {
        _controller.runController();
        glfwSwapBuffers(_window);
        glfwPollEvents();

        ++counter;
        const auto after = JBTypesMethods::getTimePointMSNow();
        auto diff = after - before;
        const std::chrono::duration<float> durationFloatDifference= diff;
        const float diffF = durationFloatDifference.count();
        if (diffF > 1.f) {
            before = after;
            std::cout << counter << " FPS"  << std::endl;
            counter = 0;
        }

        _controller.waitController();
    }

}

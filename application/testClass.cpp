/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testClass.cpp
 * Author: Morgenthaler S
 * 
 * Created on 2 novembre 2019, 11:17
 */

#include <fstream>
#include <istream>
#include <sstream>
#include "testClass.h"
#include <initializer_list>


testClass::testClass(GLFWwindow* window
        ):
    _window(window),
    _controller()
{
}



bool testClass::inputManagement() {

    bool exit = false;
    
    glfwSetInputMode(_window,GLFW_STICKY_KEYS,GL_TRUE) ;
    
    if (glfwWindowShouldClose(_window) != 0 ) {
        exit = true;
    }

    //_ball->update(); TODO MOVE IT
    //_camera.follow(*_ball); // TODO MOVE IT
    
    if(glfwGetKey(_window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        _controller.interactionButtons(Controller::Button::Escape,
                                       Controller::Status::Pressed);
    }
    
    if( glfwGetKey(_window,GLFW_KEY_ENTER) == GLFW_PRESS ||
       glfwGetKey(_window,GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        _controller.interactionButtons(Controller::Button::Validate,
                                       Controller::Status::Pressed);
    }
    
    if(glfwGetKey(_window,GLFW_KEY_RIGHT) == GLFW_PRESS ||
       glfwGetKey(_window,GLFW_KEY_L) == GLFW_PRESS) {
        _controller.interactionButtons(Controller::Button::Right,
                                       Controller::Status::Pressed);
    }
    
    if(glfwGetKey(_window,GLFW_KEY_LEFT) == GLFW_PRESS ||
       glfwGetKey(_window,GLFW_KEY_H) == GLFW_PRESS) {
        _controller.interactionButtons(Controller::Button::Left,
                                       Controller::Status::Pressed);
    }
    
    if(glfwGetKey(_window,GLFW_KEY_UP) == GLFW_PRESS ||
       glfwGetKey(_window,GLFW_KEY_K) == GLFW_PRESS) {
        _controller.interactionButtons(Controller::Button::Up,
                                       Controller::Status::Pressed);
    }
    
    if(glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1 ) == GLFW_PRESS) {
        double posX, posY;
        glfwGetCursorPos(_window,&posX ,&posY);
        posX = posX / static_cast<double>(RESOLUTION_X);
        posY = 1.-posY / static_cast<double>(RESOLUTION_Y); //GLFW defines
        //y=0 as the top
        
        _controller.interactionMouse(Controller::Status::Pressed,
                                     static_cast<float>(posX),
                                     static_cast<float>(posY));
    }
    
    if(glfwGetKey(_window,GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        _controller.interactionButtons(Controller::Button::Escape,
                                       Controller::Status::Released);
    }
    
    if(glfwGetKey(_window,GLFW_KEY_ENTER) == GLFW_RELEASE ||
       glfwGetKey(_window,GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        _controller.interactionButtons(Controller::Button::Validate,
                                       Controller::Status::Released);
    }
    
    if(glfwGetKey(_window,GLFW_KEY_RIGHT) == GLFW_RELEASE ||
       glfwGetKey(_window,GLFW_KEY_L) == GLFW_RELEASE) {
        _controller.interactionButtons(Controller::Button::Right,
                                       Controller::Status::Released);
    }
    
    if(glfwGetKey(_window,GLFW_KEY_LEFT) == GLFW_RELEASE ||
       glfwGetKey(_window,GLFW_KEY_H) == GLFW_RELEASE) {
        _controller.interactionButtons(Controller::Button::Left,
                                       Controller::Status::Released);
    }
    
    if(glfwGetKey(_window,GLFW_KEY_UP) == GLFW_RELEASE ||
       glfwGetKey(_window,GLFW_KEY_K) == GLFW_RELEASE) {
        _controller.interactionButtons(Controller::Button::Up,
                                       Controller::Status::Released);
    }
    
    if(glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1 ) == GLFW_RELEASE) {
        double posX, posY;
        glfwGetCursorPos(_window,&posX ,&posY);
        posX = posX / static_cast<double>(RESOLUTION_X);
        posY = 1.-posY / static_cast<double>(RESOLUTION_Y);
        
        _controller.interactionMouse(Controller::Status::Released,
                                     static_cast<float>(posX),
                                     static_cast<float>(posY));
    }
    
    if (_controller.requestToLeave()) {
        exit = true;
    }

    return exit;
}



void testClass::run()
{

    auto before = JumperBallTypesMethods::getTimePointMSNow();
    unsigned int counter = 0;
    while (!inputManagement())
	{

        _controller.run();

        glfwSwapInterval(0);
		glfwSwapBuffers(_window);

		glfwPollEvents();

		counter++;
		auto after = JumperBallTypesMethods::getTimePointMSNow();
		auto diff = after - before;
		const std::chrono::duration<float> durationFloatDifference= diff;
		float diffF = durationFloatDifference.count();
		if (diffF > 1.f) {
			before = after;
			std::cout << counter << " FPS"  << std::endl;
			counter = 0;
		}

        /*TODO MOVE IT IN CONTROLLER
		if (_player.statut() == Player::Statut::INGAME) {
			_ball->update();
			_camera.follow(*_ball);
        }*/
	}

}

testClass::~testClass() {
    glfwTerminate();
    TextRendering::clearFreeTypeRessources();
}

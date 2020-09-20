/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testClass.h
 * Author: Morgenthaler S
 *
 * Created on 2 novembre 2019, 11:17
 */

#ifndef TESTCLASS_H
#define TESTCLASS_H
#include <Types.h>
#include <Utility.h>
#include <Controller.h>
#define RESOLUTION_X 1024
#define RESOLUTION_Y 768

class testClass {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    testClass(  GLFWwindow* window );

    
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

#endif /* TESTCLASS_H */


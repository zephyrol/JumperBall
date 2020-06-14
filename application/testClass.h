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
#include <Shader.h>
#include <ShaderProgram.h>
#include <Rendering.h>
#include <graphicMenu/Menu.h>
#include <Controller.h>

class testClass {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    testClass();
    ~testClass();

    
    //----------METHODS-------------//
    void          runController(Rendering& r, const std::shared_ptr<Ball>& b,
                                Camera& c);
    void          runMenu(Rendering& r, Ball& b, Camera& c, Map& m);


private:

    //--------ATTRIBUTES-----------//
    GLFWwindow*   _window;
    Player        _player;
    Controller    _controller;

};

#endif /* TESTCLASS_H */


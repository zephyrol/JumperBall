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
    testClass( const std::shared_ptr<Ball>& ball,
              Camera& cam,
              Map& map
              );
    ~testClass();

    
    //----------METHODS-------------//
    void          run();
    void          assignRenderingEngine(std::shared_ptr<Rendering> rendering);


private:

    //--------ATTRIBUTES-----------//
    GLFWwindow*   _window;
    Player        _player;
    Controller    _controller;
    std::shared_ptr<Rendering>    _renderingEngine;
    const std::shared_ptr<Ball> _ball;
    Map&          _map;
    Camera&       _camera;

    //----------METHODS-------------//
    bool          runController();
    void          runMenu();
    void          runGame();

};

#endif /* TESTCLASS_H */


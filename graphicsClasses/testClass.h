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
#include "Shader.h"
#include "ShaderProgram.h"
#include "Rendering.h"
#include <TextRendering.h>

class testClass {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    testClass();
    ~testClass();

    
    //----------METHODS-------------//
    testClass     (const testClass&);
    testClass&    operator = (const testClass&);
    void          run(Rendering& r, Ball& b, Camera& c);


private:

    //--------ATTRIBUTES-----------//
    GLFWwindow*   _window;

};

#endif /* TESTCLASS_H */


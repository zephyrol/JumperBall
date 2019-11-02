/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testClass.h
 * Author: seb
 *
 * Created on 2 novembre 2019, 11:17
 */

#ifndef TESTCLASS_H
#define TESTCLASS_H
#include <glad/include/glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <Types.h>

class testClass {
public:
    testClass();
    
    testClass(const testClass&);
    testClass& operator = (const testClass&);
    void run();
    virtual ~testClass() = default;
private:
    GLFWwindow* window;

};

#endif /* TESTCLASS_H */


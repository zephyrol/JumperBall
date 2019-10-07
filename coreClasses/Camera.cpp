/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera.cpp
 * Author: seb
 * 
 * Created on 5 octobre 2019, 13:12
 */

#include "Camera.h"

Camera::Camera() : _posX(0), _posY(0), _posZ(0),
                   _dirX(0), _dirY(0), _dirZ(0),
                   _rightX(0), _rightY(0), _rightZ(0)
{
}

/*Camera::Camera(const Camera& orig) {
}*/

Camera::~Camera() {
}


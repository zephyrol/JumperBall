/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera.h
 * Author: seb
 *
 * Created on 5 octobre 2019, 13:12
 */

#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    Camera();
    Camera(const Camera& orig);
    virtual ~Camera();

    float _posX, _posY, _posZ;
    float _dirX, _dirY, _dirZ;
    float _rightX, _rightY, _rightZ;
    static constexpr float _zNear = 0.1f;
    static constexpr float _zFar  = 100.f;

private:

};
#endif /* CAMERA_H */


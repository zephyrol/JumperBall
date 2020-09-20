/*
 * File:   BallMesh.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 septembre 2020, 11h20
 */
#include "BallMesh.h"

BallMesh::BallMesh(const Ball& ball):
    Mesh<Ball,GraphicBall>(ball)
{

}

void BallMesh::updateWorld()
{
    const JBTypes::vec3f& positionBall = _frame.get3DPosition();
    setWorld(glm::translate(glm::mat4(1.f), glm::vec3(positionBall.x,
                            positionBall.y ,positionBall.z)));
}



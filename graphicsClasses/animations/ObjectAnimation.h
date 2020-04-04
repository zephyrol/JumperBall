/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ObjectAnimation.h
 * Author: Morgenthaler S
 *
 * Created on 3 avril 2020, 11:36
 */

#ifndef OBJECTANIMATION_H
#define OBJECTANIMATION_H
#include <Ball.h>
#include "Animation.h"
#include "../Utility.h"

class ObjectAnimation : public Animation {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    ObjectAnimation                 ( const Object& object,
                                      const glm::vec3& blockPosition,
                                      const JumperBallTypes::Direction& dir);
    virtual ~ObjectAnimation        ();


    //----------METHODS------------//
    void                                updateTrans()                  override;
    glm::mat4                           model()                  const override;
    glm::mat4                           scaleRotation()          const override;
private:

    //--------ATTRIBUTES-----------//
    const Object&                       _object;
    JumperBallTypes::timePointMs        _referenceTimePointCreation;
    const JumperBallTypes::Direction    _direction;
    const glm::mat4                     _translationToBlock;
    const glm::mat4                     _initialRotation;

    glm::mat4                           _movingRotation;
    glm::mat4                           _movingScale;

    static const glm::mat4 translationOnBlock;
    static const glm::mat4 translationCenter;
    static const glm::mat4 inversedTanslationCenter;
};

#endif /* OBJECTANIMATION_H */


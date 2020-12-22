#ifndef SPECIALANIMATION_H
#define SPECIALANIMATION_H
#include <scene/special/SpecialState.h>
#include "Utility.h"
#include "Animation.h"

/*
 * File:   SpecialAnimation.h
 * Author: Morgenthaler S
 *
 * Created on 18 decembre 2020, 20h15
 */

class SpecialAnimation : public Animation {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    SpecialAnimation                 (const SpecialState& special);
    virtual ~SpecialAnimation        ()                                =default;

    //----------METHODS------------//
    void                             updateTrans()                     override;

private:
    //--------ATTRIBUTES-----------//
    const SpecialState&              _special;
    const glm::mat4                  _initialTranslation;
    const glm::mat4                  _initialRotation;
    const glm::mat4                  _initialTranslationRotation;

    glm::mat4                        _movingRotation;
    glm::mat4                        _movingTranslation;
    glm::mat4                        _movingScale;

    glm::mat4                        initInitialTranslation()             const;
    glm::mat4                        initInitialRotation()                const;

    void transTeleporter();
    void transSwitch();

    static const glm::vec3           rotationAxis;
};


#endif // SPECIALANIMATION_H 

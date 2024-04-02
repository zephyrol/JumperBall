/*
 * File: Special.h
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#ifndef __SPECIAL_H__
#define __SPECIAL_H__

#include <scene/Shape.h>
#include "system/Chronometer.h"
#include "scene/SceneElement.h"

class Special;

using Special_sptr = std::shared_ptr<Special>;
using CstSpecial_sptr = std::shared_ptr<const Special>;
using vecCstSpecial_sptr = std::vector<CstSpecial_sptr>;
using vecSpecial_sptr = std::vector<Special_sptr>;
using Special_uptr = std::unique_ptr<Special>;

class Special : public SceneElement {
public:

    Special(
        CstChronometer_sptr chronometer,
        const JBTypes::Color &color,
        const JBTypes::Dir &dir,
        const glm::u32vec3 &position,
        bool isActivated = true
    );

    const JBTypes::Color &getColor() const;

    const JBTypes::Dir &direction() const;

    const glm::u32vec3 &position() const;

    bool isActivated() const;

    virtual void applySpecialEffect() = 0;

    virtual bool doesModifyActivator() const;

    void switchOnOff();

    std::string getDynamicGroupHash() const override;

protected:
    const glm::vec3 &positionF() const;

    const CstChronometer_sptr _chronometer;
    const JBTypes::Dir _direction;
    const glm::vec3 _directionVec;

private:
    const JBTypes::Color _color;
    const std::string _colorAttributeName;
    const glm::u32vec3 _position;
    const glm::vec3 _positionF;
    bool _isActivated;

};


#endif // __SPECIAL_H__

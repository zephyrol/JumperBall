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
#include "scene/Displayable.h"

class Special;

using Special_sptr = std::shared_ptr<Special>;
using CstSpecial_sptr = std::shared_ptr<const Special>;
using vecCstSpecial_sptr = std::vector<CstSpecial_sptr>;
using vecSpecial_sptr = std::vector<Special_sptr>;
using Special_uptr = std::unique_ptr<Special>;

class Special : public Displayable {
public:

    Special(
        CstChronometer_sptr chronometer,
        const JBTypes::Color &color,
        const JBTypes::Dir &dir,
        const JBTypes::vec3ui &position,
        bool isActivated = true
    );

    const JBTypes::Color &getColor() const;

    const JBTypes::Dir &direction() const;

    const JBTypes::vec3ui &position() const;

    bool isActivated() const;

    virtual bool isAnimated() const;

    virtual void applySpecialEffect() = 0;

    void switchOnOff();

    std::vector<unsigned char> getStaticUnsignedByteValues() const override;

    Displayable::StaticValues<JBTypes::vec3f> getStaticVec3fValues() const override;

    Displayable::DynamicNames getDynamicFloatNames() const override;

    Displayable::DynamicValues<float> getDynamicFloatValues() const override;

    std::string getDynamicGroupHash() const override;

    bool dynamicsMayChange() const override;

private:
    const CstChronometer_sptr _chronometer;
    const JBTypes::Dir _direction;
    const JBTypes::Color _color;
    const std::string _colorAttributeName;
    const JBTypes::vec3ui _position;
    bool _isActivated;

};


#endif // __SPECIAL_H__

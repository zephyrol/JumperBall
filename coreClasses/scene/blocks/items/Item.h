/*
 * File: Item.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "system/Types.h"
#include "scene/Displayable.h"
#include "system/Chronometer.h"
#include <scene/Shape.h>

class Item;

using Item_sptr = std::shared_ptr<Item>;
using CstItem_sptr = std::shared_ptr<const Item>;
using vecCstItem_sptr = std::vector<CstItem_sptr>;
using vecItem_sptr = std::vector<Item_sptr>;
using Item_uptr = std::unique_ptr<Item>;

class Item : public Displayable {
public:

    Item(const JBTypes::vec3ui &position, const JBTypes::Dir &direction, CstChronometer_sptr chronometer);

    const JBTypes::vec3ui &position() const;

    const JBTypes::vec3f &get3DPosition() const;

    const JBTypes::Dir &direction() const;

    bool isGotten() const;

    float getTimeSinceObtaining() const;

    virtual void catchingTest() = 0;

    virtual vecCstShape_sptr getShapes() const = 0;

    static constexpr float itemBoundingSphereRadius = 0.15f;

    Displayable::StaticValues<unsigned char> getStaticUnsignedByteValues() const override;

    Displayable::StaticValues<JBTypes::vec3f> getStaticVec3fValues() const override;

    Displayable::DynamicNames getDynamicFloatNames() const override;

    Displayable::DynamicValues<float> getDynamicFloatValues() const override;

    Displayable::GlobalState getGlobalState() const override;

    bool globalStateMayChange() const override;

    void setAsGotten();

private:

    JBTypes::vec3f compute3DPosition() const;

    const CstChronometer_sptr _chronometer;
    const JBTypes::vec3ui _position;
    const JBTypes::Dir _direction;
    const JBTypes::vec3f _3DPosition;
    bool _gotten;
    float _obtainingTime;

};

#endif /* OBJECT_H */

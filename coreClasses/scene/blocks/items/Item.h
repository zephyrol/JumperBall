/*
 * File: Item.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "system/Types.h"
#include "system/Chronometer.h"
#include "scene/SceneElement.h"
#include <scene/Shape.h>

class Item;

using Item_sptr = std::shared_ptr<Item>;
using CstItem_sptr = std::shared_ptr<const Item>;
using vecCstItem_sptr = std::vector<CstItem_sptr>;
using vecItem_sptr = std::vector<Item_sptr>;
using Item_uptr = std::unique_ptr<Item>;

class Item : public SceneElement {
public:

    Item(const JBTypes::vec3ui &position, const JBTypes::Dir &direction, CstChronometer_sptr chronometer);

    const JBTypes::vec3ui &position() const;

    const JBTypes::vec3f &get3DPosition() const;

    const JBTypes::Dir &direction() const;

    bool isGotten() const;

    float getTimeSinceObtaining() const;

    virtual void catchingTest() = 0;

    static constexpr float itemBoundingSphereRadiusJumping = 0.15f;
    static constexpr float itemBoundingSphereRadiusStaying = 0.3f;

    DynamicValues <JBTypes::vec3f> getDynamicVec3fValues() const override;

    DynamicValues <JBTypes::Quaternion> getDynamicQuaternionValues() const override;

    std::string getDynamicGroupHash() const override;

    void setAsGotten();

protected:

    static Transformation getVerticalCylinderRotation(const JBTypes::Dir &direction);

private:

    JBTypes::vec3f compute3DPosition() const;

    const CstChronometer_sptr _chronometer;
    const JBTypes::vec3ui _position;
    const JBTypes::Dir _direction;
    const JBTypes::vec3f _directionVec;
    const JBTypes::vec3f _translationWay;
    const JBTypes::vec3f _3DPosition;
    std::unique_ptr<float> _obtainingTime;

};

#endif /* OBJECT_H */

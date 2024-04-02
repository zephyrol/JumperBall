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

    Item(const glm::u32vec3 &position, const JBTypes::Dir &direction, CstChronometer_sptr chronometer);

    const glm::u32vec3 &position() const;

    const glm::vec3 &get3DPosition() const;

    const JBTypes::Dir &direction() const;

    bool isGotten() const;

    float getTimeSinceObtaining() const;

    virtual void catchingTest() = 0;

    static constexpr float itemBoundingSphereRadiusJumping = 0.15f;
    static constexpr float itemBoundingSphereRadiusStaying = 0.3f;

    DynamicValues <glm::vec3> getDynamicVec3fValues() const override;

    DynamicValues <glm::quat> getDynamicQuaternionValues() const override;

    std::string getDynamicGroupHash() const override;

    void setAsGotten();

protected:

    static Transformation getVerticalCylinderRotation(const JBTypes::Dir &direction);

private:

    glm::vec3 compute3DPosition() const;

    const CstChronometer_sptr _chronometer;
    const glm::u32vec3 _position;
    const JBTypes::Dir _direction;
    const glm::vec3 _directionVec;
    const glm::vec3 _translationWay;
    const glm::vec3 _3DPosition;
    std::unique_ptr<float> _obtainingTime;

};

#endif /* OBJECT_H */

/*
 * File: Clock.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:48
 */

#ifndef CLOCK_H
#define CLOCK_H

#include "scene/Ball.h"
#include "InteractiveItem.h"

class Clock : public InteractiveItem {
public:
    Clock(const glm::u32vec3 &position, const JBTypes::Dir &direction, const Ball_sptr &ball);

    vecCstShape_sptr getShapes() const override;

private:
    void catchItem() override;
};

#endif /* CLOCK_H */

/*
 * File: Bonus.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:48
 */

#ifndef BONUS_H
#define BONUS_H

#include "InteractiveItem.h"

class Bonus : public InteractiveItem {
public:
    Bonus(
        const glm::u32vec3 &position,
        const JBTypes::Dir &direction,
        unsigned int value,
        const Ball_sptr &ball
    );

    unsigned int value() const;

    virtual ~Bonus() = default;

private:
    unsigned int _value;

};

#endif /* BONUS_H */

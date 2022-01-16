/*
 * File: Key.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:45
 */

#ifndef KEY_H
#define KEY_H

#include "InteractiveItem.h"

class Key: public InteractiveItem {
public:
Key(
    const JBTypes::vec3ui& position,
    const JBTypes::Dir& direction,
    const Ball_sptr &ball
);

vecCstShape_sptr getShapes() const override;

private:
    void catchItem() override;
};

#endif /* KEY_H */

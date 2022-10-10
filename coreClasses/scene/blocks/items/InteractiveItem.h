//
// Created by Sébastien Morgenthaler on 16/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_INTERACTIVEITEM_H
#define JUMPERBALLAPPLICATION_INTERACTIVEITEM_H


#include "scene/Ball.h"

class InteractiveItem : public Item {
public:
    InteractiveItem(
        const JBTypes::vec3ui &position,
        const JBTypes::Dir &direction,
        const Ball_sptr &ball
    );

    void catchingTest() override;

    virtual void catchItem();

protected:
    const Ball_wptr _ball;
};


#endif //JUMPERBALLAPPLICATION_INTERACTIVEITEM_H

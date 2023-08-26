//
// Created by S.Morgenthaler on 23/10/2021.
//

#include "InteractiveSpecial.h"

InteractiveSpecial::InteractiveSpecial(
    const JBTypes::Color &color,
    const JBTypes::vec3ui &position,
    const JBTypes::Dir &direction,
    const Ball_sptr &ball,
    bool isActivated
) : Special(ball->getCreationChronometer(), color, direction, position, isActivated),
    _ball(ball) {
}

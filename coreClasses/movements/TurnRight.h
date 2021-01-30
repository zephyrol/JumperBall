//
// TurnRight.h
// JumperBallApplication
//
// Created by Sebastien Morgenthaler on 31/07/2020.
//

#ifndef TurnRight_h
#define TurnRight_h
#include "Movement.h"


class TurnRight:public Movement <JBTypes::Dir, 2> {
public:
TurnRight();
~TurnRight() = default;
};


#endif /* TurnRight_h */

//
// TurnBack.h
// JumperBallApplication
//
// Created by Sebastien Morgenthaler on 31/07/2020.
//
#ifndef TurnBack_h
#define TurnBack_h
#include "Movement.h"

class TurnBack:public Movement <JBTypes::Dir, 1> {
public:
TurnBack();
~TurnBack() = default;

};



#endif /* TurnBack_h */

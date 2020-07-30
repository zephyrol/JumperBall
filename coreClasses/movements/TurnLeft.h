//
//  TurnLeft.h
//  JumperBallApplication
//
//  Created by Sebastien Morgenthaler on 30/07/2020.
//

#ifndef TurnLeft_h
#define TurnLeft_h
#include "Movement.h"


class TurnLeft : public Movement<JBTypes::Dir,2> {
    public :
    TurnLeft();
    ~TurnLeft() = default;
};


#endif /* TurnLeft_h */

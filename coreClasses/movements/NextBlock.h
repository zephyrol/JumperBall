//
//  NextBlock.h
//  JumperBallApplication
//
//  Created by Sebastien Morgenthaler on 30/07/2020.
//

#ifndef NextBlock_h
#define NextBlock_h
#include "Movement.h"


class NextBlock : public Movement<std::array<int,12>,2> {
public :
    NextBlock();
    ~NextBlock() = default;
};


#endif /* NextBlock_h */

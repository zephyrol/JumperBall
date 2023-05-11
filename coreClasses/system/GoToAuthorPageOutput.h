//
// Created by S.Morgenthaler on 11/05/2023.
//

#ifndef JUMPERBALLAPPLICATION_GOTOAUTHORPAGEOUTPUT_H
#define JUMPERBALLAPPLICATION_GOTOAUTHORPAGEOUTPUT_H
#include "UpdateOutput.h"


class GoToAuthorPageOutput: public UpdateOutput {

public:
    explicit GoToAuthorPageOutput(std::string &&author);

};


#endif //JUMPERBALLAPPLICATION_GOTOAUTHORPAGEOUTPUT_H

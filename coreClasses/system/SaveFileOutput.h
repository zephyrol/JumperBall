//
// Created by S.Morgenthaler on 04/12/2022.
//

#ifndef JUMPERBALLAPPLICATION_SAVEFILEOUTPUT_H
#define JUMPERBALLAPPLICATION_SAVEFILEOUTPUT_H

#include "UpdateOutput.h"


class SaveFileOutput : public UpdateOutput {
public:
    explicit SaveFileOutput(std::string &&saveContent);
};


#endif //JUMPERBALLAPPLICATION_SAVEFILEOUTPUT_H

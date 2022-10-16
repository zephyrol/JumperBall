//
// Created by S.Morgenthaler on 16/10/2022.
//

#ifndef JUMPERBALLAPPLICATION_SOUNDOUTPUT_H
#define JUMPERBALLAPPLICATION_SOUNDOUTPUT_H


#include "UpdateOutput.h"

class SoundOutput : public UpdateOutput {
public:
    explicit SoundOutput(std::string &&soundName);
};


#endif //JUMPERBALLAPPLICATION_SOUNDOUTPUT_H

//
// Created by S.Morgenthaler on 11/05/2023.
//

#ifndef JUMPERBALLAPPLICATION_SOUNDSTATUSOUTPUT_H
#define JUMPERBALLAPPLICATION_SOUNDSTATUSOUTPUT_H

#include "UpdateOutput.h"


class SoundStatusOutput : public UpdateOutput {
public:
    explicit SoundStatusOutput(std::string &&soundStatus);

};


#endif //JUMPERBALLAPPLICATION_SOUNDSTATUSOUTPUT_H

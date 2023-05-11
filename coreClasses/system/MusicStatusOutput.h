//
// Created by S.Morgenthaler on 11/05/2023.
//

#ifndef JUMPERBALLAPPLICATION_MUSICSTATUSOUTPUT_H
#define JUMPERBALLAPPLICATION_MUSICSTATUSOUTPUT_H

#include "UpdateOutput.h"

class MusicStatusOutput : public UpdateOutput {
public:
    explicit MusicStatusOutput(std::string &&musicStatus);
};


#endif //JUMPERBALLAPPLICATION_MUSICSTATUSOUTPUT_H

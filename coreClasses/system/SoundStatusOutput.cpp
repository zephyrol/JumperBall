//
// Created by S.Morgenthaler on 11/05/2023.
//

#include "SoundStatusOutput.h"

SoundStatusOutput::SoundStatusOutput(std::string &&soundStatus):
    UpdateOutput("soundStatus", std::move(soundStatus)){

}

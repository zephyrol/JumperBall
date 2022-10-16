//
// Created by S.Morgenthaler on 16/10/2022.
//

#include "SoundOutput.h"

SoundOutput::SoundOutput(std::string &&soundName)
    : UpdateOutput("Sound", std::move(soundName)) {

}

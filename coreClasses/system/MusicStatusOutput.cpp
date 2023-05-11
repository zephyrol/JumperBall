//
// Created by S.Morgenthaler on 11/05/2023.
//

#include "MusicStatusOutput.h"

MusicStatusOutput::MusicStatusOutput(std::string &&musicStatus)
    : UpdateOutput("musicStatus", std::move(musicStatus)) {

}

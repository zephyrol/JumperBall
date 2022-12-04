//
// Created by S.Morgenthaler on 04/12/2022.
//

#include "SaveFileOutput.h"

SaveFileOutput::SaveFileOutput(std::string &&saveContent)
    : UpdateOutput("Save", std::move(saveContent)) {

}

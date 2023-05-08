//
// Created by S.Morgenthaler on 16/10/2022.
//

#include "UpdateOutput.h"

UpdateOutput::UpdateOutput(std::string &&base, std::string &&meta) :
    _base(std::move(base)), _meta(std::move(meta)) {

}

std::string UpdateOutput::getOutput() const {
    const std::string outputBase = _base + ";";
    return _meta.empty()
        ? outputBase
        : outputBase + _meta + ";";
}

std::string UpdateOutput::combineUpdateOutputs(vecCstUpdateOutput_sptr &&updateOutputs) {
    std::string output {};
    for(const auto& updateOutput: updateOutputs) {
        output += updateOutput->getOutput();
    }
    updateOutputs.clear();
    return output;
}


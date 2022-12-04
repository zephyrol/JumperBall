//
// Created by S.Morgenthaler on 16/10/2022.
//

#ifndef JUMPERBALLAPPLICATION_UPDATEOUTPUT_H
#define JUMPERBALLAPPLICATION_UPDATEOUTPUT_H

#include "Types.h"


class UpdateOutput;
using CstUpdateOutput_sptr = std::shared_ptr<const UpdateOutput>;
using vecCstUpdateOutput_sptr = std::vector<CstUpdateOutput_sptr>;

class UpdateOutput {

public:
    explicit UpdateOutput(std::string &&base, std::string &&meta);

    std::string getOutput() const;

    /**
     * Empty the update and get the concatenation of outputs
     */
    static std::string combineUpdateOutputs(vecCstUpdateOutput_sptr&& updateOutputs);

private:
    const std::string _base;
    const std::string _meta;
};


#endif //JUMPERBALLAPPLICATION_UPDATEOUTPUT_H

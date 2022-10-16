//
// Created by S.Morgenthaler on 16/10/2022.
//

#ifndef JUMPERBALLAPPLICATION_UPDATEOUTPUT_H
#define JUMPERBALLAPPLICATION_UPDATEOUTPUT_H

#include "Types.h"


class UpdateOutput {

public:
    explicit UpdateOutput(std::string &&base, std::string &&meta);

    std::string getOutput() const;

private:
    const std::string _base;
    const std::string _meta;
};


#endif //JUMPERBALLAPPLICATION_UPDATEOUTPUT_H

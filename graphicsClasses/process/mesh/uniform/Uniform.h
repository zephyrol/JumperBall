//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORM_H
#define JUMPERBALLAPPLICATION_UNIFORM_H

#include "UniformBase.h"

template<class T>
class Uniform : public UniformBase {
public:
    Uniform(const CstShaderProgram_sptr &shaderProgram, const std::string &name);

    virtual void update(const T &value) = 0;
};

template<class T>
Uniform<T>::Uniform(const CstShaderProgram_sptr &shaderProgram, const std::string &name):
    UniformBase(shaderProgram, name) {

}


#endif //JUMPERBALLAPPLICATION_UNIFORM_H

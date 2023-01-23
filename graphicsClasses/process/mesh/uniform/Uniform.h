//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORM_H
#define JUMPERBALLAPPLICATION_UNIFORM_H

#include "UniformBase.h"


template<class T>
class Uniform;

template<class T>
using Uniform_ptr = std::shared_ptr<Uniform<T>>;

template<class T>
using vecUniform_sptr = std::vector<Uniform_ptr<T> >;

template<class T>
class Uniform : public UniformBase {
public:
    Uniform(const T &value, const CstShaderProgram_sptr &shaderProgram, const std::string &name);

    void update(const T &value);
    const T& getValue() const;

protected:
    T _value;
};

template<class T>
const T &Uniform<T>::getValue() const {
    return _value;
}

template<class T>
void Uniform<T>::update(const T &value) {
    _value = value;
}

template<class T>
Uniform<T>::Uniform(const T &value, const CstShaderProgram_sptr &shaderProgram, const std::string &name):
    UniformBase(shaderProgram, name),
    _value(value) {
}

#endif //JUMPERBALLAPPLICATION_UNIFORM_H

//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMBASE_H
#define JUMPERBALLAPPLICATION_UNIFORMBASE_H
#include "Utility.h"
#include "ShaderProgram.h"

class UniformBase {
public:
    UniformBase(
        const CstShaderProgram_sptr& shaderProgram,
        const std::string& name
    );
    UniformBase(const UniformBase& uniformBase) = delete;
    UniformBase& operator=(const UniformBase& uniformBase) = delete;

    virtual void bind() = 0;

protected:
    const GLint _location;
};


#endif //JUMPERBALLAPPLICATION_UNIFORMBASE_H

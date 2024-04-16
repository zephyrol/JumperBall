//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMBASE_H
#define JUMPERBALLAPPLICATION_UNIFORMBASE_H

#include "ShaderProgram.h"
#include "Utility.h"

class UniformBase;
using UniformBase_sptr = std::shared_ptr<UniformBase>;
using vecUniformBase_sptr = std::vector<UniformBase_sptr>;

class UniformBase {
   public:
    /**
     * @param shaderProgram The referenced shader program.
     * The created uniform is only compatible with this shader!
     * @param name The name of the uniform used in the shader code.
     */
    UniformBase(const ShaderProgram_uptr& shaderProgram, const std::string& name);

    /**
     * A uniform should be unique! If a uniform is used on several shader programs, use an
     * uniform buffer object.
     */
    UniformBase(const UniformBase& uniformBase) = delete;

    UniformBase& operator=(const UniformBase& uniformBase) = delete;

    /**
     * Bind the uniform with its current value to the shader program.
     * Use this function only if the value has changed.
     * Warning: The referenced shader program needs to be used before calling this method!
     */
    virtual void bind() const = 0;

    virtual ~UniformBase() = default;

   protected:
    /*
     * Uniform location in the referenced shader program.
     */
    const GLint _location;
};

#endif  // JUMPERBALLAPPLICATION_UNIFORMBASE_H

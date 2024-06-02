/*
 * File: ShaderProgram.h
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 17:13
 */

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"

class ShaderProgram;

using ShaderProgram_sptr = std::shared_ptr<ShaderProgram>;
using CstShaderProgram_sptr = std::shared_ptr<const ShaderProgram>;
using vecCstShaderProgram_sptr = std::vector<CstShaderProgram_sptr>;

class ShaderProgram {
   public:
    ShaderProgram(CstShader_uptr&& vertexShader, CstShader_uptr&& fragmentShader, GLuint shaderProgramHandle);

    ShaderProgram(const ShaderProgram& shaderProgram) = delete;

    ShaderProgram& operator=(const ShaderProgram& shaderProgram) = delete;

    ~ShaderProgram();

    GLuint getHandle() const;

    /**
     * For a uniform sampler variable representing by its name, set a
     * texture index (The number of the active texture).
     * Warning: don't forget to call ShaderProgram::use() method before!
     */
    void setTextureIndex(const std::string& textureName, GLint index);

    /**
     * For a uniform integer representing by its uniform location, set its content in the current
     * shader program.
     * Warning: don't forget to call ShaderProgram::use() method before!
     */
    static void setInteger(GLint integerUniformLocation, int value);

    /**
     * For a uniform array of vec2 representing by its name, set its content
     * Warning: don't forget to call ShaderProgram::use() method before!
     */
    void setUniformArrayVec2(const std::string& uniformArrayName, const std::vector<GLfloat>& vec2sData);

    /**
     * Get uniform location giving its name
     * @param uniformName Name of the uniform in the shader source code
     * @return Uniform Location
     */
    GLint getUniformLocation(const std::string& uniformName) const;

    /**
     * For a uniform array of vec4 representing by its name, set its content
     * Warning: don't forget to call ShaderProgram::use() method before!
     */
    void setUniformArrayVec4(const std::string& uniformArrayName, const std::vector<GLfloat>& vec4sData);

    static ShaderProgram_sptr createInstance(
        const JBTypes::FileContent& fileContent,
        const std::string& vs,
        const std::string& fs,
        const std::vector<std::string>& defines = {},
        const std::vector<std::pair<std::string, GLshort>>& constShorts = {},
        const std::vector<std::pair<std::string, GLfloat>>& constFloats = {},
        const std::vector<std::pair<std::string, glm::vec2>>& constVec2s = {});

    void use() const;

   private:
    const GLuint _shaderProgramHandle;
    const CstShader_uptr _vertexShader;
    const CstShader_uptr _fragmentShader;
    static void verifyLinkStatus(GLuint shaderProgramHandle);
};

#endif /* SHADERPROGRAM_H */

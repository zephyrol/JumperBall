/*
 * File: Shader.h
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 16:10
 */

#ifndef SHADER_H
#define SHADER_H

#include "Utility.h"

class Shader;

using Shader_uptr = std::unique_ptr<Shader>;
using CstShader_uptr = std::unique_ptr<const Shader>;

class Shader {
   public:
    Shader(const GLenum& shaderType,
           const JBTypes::FileContent& fileContent,
           const std::string& shaderFilename,
           const std::vector<std::string>& defines,
           const std::vector<std::pair<std::string, GLshort>>& constShorts,
           const std::vector<std::pair<std::string, GLfloat>>& constFloats,
           const std::vector<std::pair<std::string, glm::vec2>>& constVec2s);

    /**
     * Copy constructor and assignment are deleted because a OpenGL shader id is unique.
     */
    Shader(const Shader& shader) = delete;
    Shader& operator=(const Shader& shader) = delete;

    /**
     * Move constructor and assignment are deleted because the destructor free the GPU memory.
     */
    Shader(Shader&& shaderProgram) = delete;
    Shader& operator=(Shader&& shaderProgram) = delete;

    ~Shader();

    /**
     * Get OpenGL shader Id. Use this getter ONLY to reference the ID to OpenGL API.
     * Do not store it anywhere!
     */
    GLuint getHandle() const;

    GLenum getShaderType() const;

    static CstShader_uptr createVertexShader(
        const JBTypes::FileContent& fileContent,
        const std::string& shaderName,
        const std::vector<std::string>& defines,
        const std::vector<std::pair<std::string, GLshort>>& constShorts,
        const std::vector<std::pair<std::string, GLfloat>>& constFloats,
        const std::vector<std::pair<std::string, glm::vec2>>& constVec2s);

    static CstShader_uptr createFragmentShader(
        const JBTypes::FileContent& fileContent,
        const std::string& shaderName,
        const std::vector<std::string>& defines,
        const std::vector<std::pair<std::string, GLshort>>& constShorts,
        const std::vector<std::pair<std::string, GLfloat>>& constFloats,
        const std::vector<std::pair<std::string, glm::vec2>>& constVec2s);

   private:
    static std::string completeShaderCode(const std::string& shaderCode,
                                          const std::vector<std::string>& defines,
                                          const std::vector<std::pair<std::string, GLshort>>& constShorts,
                                          const std::vector<std::pair<std::string, GLfloat>>& constFloats,
                                          const std::vector<std::pair<std::string, glm::vec2>>& constVec2s);

    const GLuint _shaderHandle;
    const GLenum _shaderType;
    const std::string _shaderFilename;
    const std::string _shaderCode;

    void verifyCompileStatus(const std::string& shaderCode) const;
};

#endif /* SHADER_H */

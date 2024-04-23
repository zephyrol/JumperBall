/*
 * File: Shader.cpp
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 16:10
 */

#include "Shader.h"
#define CHECK_SHADER_COMPILE_STATUS(shader) \
do { \
GLint compileStatus; \
glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus); \
GLint logLength; \
glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength); \
char *log = (char *)malloc(logLength); \
glGetShaderInfoLog(shader, logLength, NULL, log); \
fprintf(stderr, "Shader compilation failed:\n%s\n", log); \
free(log); \
} while (0)

Shader::Shader(
    const GLenum &shaderType,
    const JBTypes::FileContent &fileContent,
    const std::string &shaderFilename,
    const std::vector<std::string> &defines,
    const std::vector<std::pair<std::string, GLshort>> &constShorts,
    const std::vector<std::pair<std::string, GLfloat>> &constFloats,
    const std::vector<std::pair<std::string, glm::vec2>> &constVec2s
) :
    _shaderHandle(glCreateShader(shaderType)),
    _shaderType(shaderType),
    _shaderFilename(shaderFilename),
    _shaderCode(completeShaderCode(
                    fileContent.at(shaderFilename),
                    defines,
                    constShorts,
                    constFloats,
                    constVec2s
                )
    ) {
    if (_shaderHandle == 0) {
        std::cerr << "Error during creation of the shader ..." << std::endl;
        exit(EXIT_FAILURE);
    }

    constexpr GLsizei numberOfStrings = 1;
    const GLchar *const glCode = _shaderCode.c_str();

    glShaderSource(_shaderHandle, numberOfStrings, &glCode, nullptr);
    glCompileShader(_shaderHandle);
    verifyCompileStatus(_shaderCode);
}

void Shader::verifyCompileStatus(const std::string &shaderCode) const {

    GLint status;
    glGetShaderiv(_shaderHandle, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        std::cerr << "Shader compilation failed : " << _shaderFilename << std::endl;
        std::cerr << "Shader code" << std::endl << shaderCode << std::endl;
        GLint logLength;
        glGetShaderiv(_shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            std::string log(logLength, ' ');
            GLsizei message;
            glGetShaderInfoLog(_shaderHandle, logLength, &message, &log[0]);
            std::cerr << "Error log : " << std::endl << log;
        }
    }
}

Shader::~Shader() {
    // glDeleteShader(_shaderHandle);
}

GLuint Shader::getHandle() const {
    return _shaderHandle;
}

GLenum Shader::getShaderType() const {
    return _shaderType;
}

CstShader_uptr Shader::createVertexShader(
    const JBTypes::FileContent &fileContent,
    const std::string &shaderName,
    const std::vector<std::string> &defines,
    const std::vector<std::pair<std::string, GLshort>> &constShorts,
    const std::vector<std::pair<std::string, GLfloat>> &constFloats,
    const std::vector<std::pair<std::string, glm::vec2>> &constVec2s
) {
    return std::unique_ptr<const Shader>(new Shader(
        GL_VERTEX_SHADER,
        fileContent,
        shaderName,
        defines,
        constShorts,
        constFloats,
        constVec2s
    ));
}

CstShader_uptr Shader::createFragmentShader(
    const JBTypes::FileContent &fileContent,
    const std::string &shaderName,
    const std::vector<std::string> &defines,
    const std::vector<std::pair<std::string, GLshort>> &constShorts,
    const std::vector<std::pair<std::string, GLfloat>> &constFloats,
    const std::vector<std::pair<std::string, glm::vec2>> &constVec2s
) {
    return std::unique_ptr<const Shader>(new Shader(
        GL_FRAGMENT_SHADER,
        fileContent,
        shaderName,
        defines,
        constShorts,
        constFloats,
        constVec2s
    ));
}

std::string Shader::completeShaderCode(
    const std::string &shaderCode,
    const std::vector<std::string> &defines,
    const std::vector<std::pair<std::string, GLshort>> &constShorts,
    const std::vector<std::pair<std::string, GLfloat>> &constFloats,
    const std::vector<std::pair<std::string, glm::vec2>> &constVec2s
) {
    const std::map<size_t, std::string> shaderHeader = {
        {0, "#version 330 core\n"},
        {1, "#version 300 es\nprecision highp float;\nprecision mediump int;\nprecision highp sampler2DShadow;\n"},
        {2, "#version 300 es\nprecision highp float;\nprecision mediump int;\nprecision highp sampler2DShadow;\n"}
    };
    std::string finalShader = shaderHeader.at(JB_SYSTEM);

    for (const auto &item: constShorts) {
        const auto &constName = item.first;
        const auto &constValue = item.second;
        finalShader += "const int " + constName + " = " + std::to_string(constValue) + ";\n";
    }

    for (const auto &item: constFloats) {
        const auto &constName = item.first;
        const auto &constValue = item.second;
        finalShader += "const float " + constName + " = " + std::to_string(constValue) + ";\n";
    }

    for (const auto &item: constVec2s) {
        const auto &constName = item.first;
        const auto &constValue = item.second;
        finalShader += "const vec2 " + constName + " = vec2("
                       + std::to_string(constValue.x) + ", " + std::to_string(constValue.y) + ");\n";
    }
    finalShader += shaderCode;

    const std::string ifdefKey = "#ifdef";
    const std::string endifKey = "#endif";

    size_t index;
    while ((index = finalShader.find(ifdefKey)) != std::string::npos) {
        const auto defineIndex = index + ifdefKey.length() + 1; // 1 for (

        const auto closedBracketIndex = finalShader.find(')', defineIndex);
        const auto defineValue = std::string(
            finalShader.begin() + defineIndex,
            finalShader.begin() + closedBracketIndex
        );
        const auto isMatchingAnyDefines = std::any_of(
            defines.begin(),
            defines.end(),
            [&defineValue](const std::string &define) { return define == defineValue; }
        );
        if (isMatchingAnyDefines) {
            finalShader.replace(index, closedBracketIndex - index + 1, ""); // +1 to include closed bracket
            const auto endifIndex = finalShader.find(endifKey);
            finalShader.replace(endifIndex, endifKey.length(), "");
        } else {
            finalShader.replace(index, finalShader.find(endifKey) + endifKey.length() - index, "");
        }
    }
    return finalShader;
}

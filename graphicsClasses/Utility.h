#ifndef UTILITY_H
#define UTILITY_H
#define __APPLE_IOS__
#ifdef __ANDROID__
#include <GLES3/gl3.h>
#define JB_SYSTEM 1
#elif defined __APPLE_IOS__
#define GLES_SILENCE_DEPRECATION
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#define JB_SYSTEM 2
#else

#include <glad.h>

#define JB_SYSTEM 0
#endif

#include <system/Types.h>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace Utility {

    glm::mat4 rotationUpToDir(JBTypes::Dir dir);

    float evalGauss1D(float x, float sigma);

    std::vector<GLfloat> genGaussBuffer(size_t patchSize, float sigma);

    glm::vec3 colorAsVec3(const JBTypes::Color &color);

    std::vector<glm::vec3> GLfloatListToGlmVec3(const std::vector<GLfloat> &list);

    std::vector<glm::vec2> GLfloatListToGlmVec2(const std::vector<GLfloat> &list);

    glm::vec4 convertToOpenGLFormat(const JBTypes::Quaternion &q);

    glm::vec3 convertToOpenGLFormat(const JBTypes::vec3f &vec3f);

    glm::vec2 convertToOpenGLFormat(const JBTypes::vec2f &vec2f);

    GLshort convertToOpenGLFormat(const short &s);

    GLfloat convertToOpenGLFormat(const float &f);

    GLint convertToOpenGLFormat(const int &i);

    GLubyte convertToOpenGLFormat(unsigned char uChar);

    float menuPositionToOpenGLScreenFormat(float value);
}

#endif /* UTILITY_H */

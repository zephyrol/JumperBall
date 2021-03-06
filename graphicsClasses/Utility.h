#ifndef UTILITY_H
#define UTILITY_H
#include <glad.h>
#include <GLFW/glfw3.h>
#include <system/Types.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace Utility {

std::string readFileSrc(const std::string& filePath);
void printMatrix(const glm::mat4& m);
std::vector <GLfloat> computeNormals(const std::vector <GLfloat>& positions);
glm::mat4 rotationUpToDir(JBTypes::Dir dir);

float evalGauss1D(float x, float sigma);
std::vector <GLfloat> genGaussBuffer(size_t patchSize, float sigma);

glm::vec3 convertRBGToCIExyY(const glm::vec3& rbgColor);
glm::vec3 convertCIExyYToRGB(const glm::vec3& CIExyYColor);

glm::vec3 colorAsVec3(const JBTypes::Color& color);

float getLuminance(const glm::vec3& rgbColor);
std::vector <GLfloat> uniColorsCube(const glm::vec3& color);

std::vector <glm::vec3> GLfloatListToGlmVec3(const std::vector <GLfloat>& list);

std::vector <glm::vec2> GLfloatListToGlmVec2(const std::vector <GLfloat>& list);

glm::vec4 convertToOpenGLFormat(const JBTypes::Quaternion& q);
glm::vec3 convertToOpenGLFormat(const JBTypes::vec3f& vec3f);
glm::vec2 convertToOpenGLFormat(const JBTypes::vec2f& vec2f);
GLfloat convertToOpenGLFormat(const float& f);
GLubyte convertToOpenGLFormat(unsigned char uChar);


float xScreenToPortrait(float x);
float getWindowRatio();

GLsizei getWidthFromHeight(unsigned int resolutionY);
template<typename T> void concatVector (std::vector <T>& current, const std::vector <T>& other) {
    current.insert(current.end(), other.begin(), other.end());
}

float menuPositionToOpenGLScreenFormat(float value);


extern const glm::mat3 XYZToRGB;
extern const glm::mat3 RGBToXYZ;
extern unsigned int windowResolutionX;
extern unsigned int windowResolutionY;

}

#endif /* UTILITY_H */

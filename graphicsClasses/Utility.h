#ifndef UTILITY_H
#define UTILITY_H
#include <Types.h>
#include "Shader.h"
#include <glm/glm.hpp>
#define RESOLUTION_X 1024 
#define RESOLUTION_Y 768

namespace Utility {

    std::string                       readFileSrc(const std::string& filePath);
    void                              printMatrix(const glm::mat4& m);
    std::vector<GLfloat>              computeNormals(const std::vector<GLfloat>&
                                                      positions);
    glm::mat4                         rotationUpToDir(
                                        JumperBallTypes::Direction dir);

    
    extern const std::vector<GLfloat> positionsCube;
    extern const std::vector<GLfloat> colorsCube;
    extern const std::vector<GLfloat> normalsCube;

    extern const std::vector<GLfloat> positionsPike;
    extern const std::vector<GLfloat> colorsPike;
    extern const std::vector<GLfloat> normalsPike;
    
}

#endif /* UTILITY_H */


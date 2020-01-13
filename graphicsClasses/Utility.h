#ifndef UTILITY_H
#define UTILITY_H
#include <Types.h>
#include "Shader.h"
#include <glm/glm.hpp>
#define RESOLUTION_X 1024 
#define RESOLUTION_Y 768

namespace Utility {

    std::string                       readFileSrc( const std::string& filePath);

    std::vector<GLfloat>              getPositionsLocalCube();
    std::vector<GLfloat>              getNormalsLocalCube();
    std::vector<GLfloat>              getColorsLocalCube();


    std::vector<GLfloat>              getPositionsPike();
    std::vector<GLfloat>              getNormalsLocalPike();
    std::vector<GLfloat>              getColorsLocalPike();

    void                              printMatrix(const glm::mat4& m);

    extern const std::vector<GLfloat> positionsCube;
    extern const std::vector<GLfloat> colorsCube;
    extern const std::vector<GLfloat> normalsCube;

    extern const std::vector<GLfloat> positionsPike;
    extern const std::vector<GLfloat> colorsPike;
    extern const std::vector<GLfloat> normalsPike;
    
}

#endif /* UTILITY_H */


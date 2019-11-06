#ifndef UTILITY_H
#define UTILITY_H
#include <Types.h>
#include "Shader.h"

namespace Utility {
    std::string readFileSrc( const std::string& filePath);

    std::vector<GLfloat> getPositionsLocalCube();
    std::vector<GLfloat> getNormalsLocalCube();
    std::vector<GLfloat> getColorsLocalCube();
}

#endif /* UTILITY_H */


#ifndef UTILITY_H
#define UTILITY_H
#include <Types.h>
#include "Shader.h"
#include <glm/glm.hpp>
#define RESOLUTION_X 1024 
#define RESOLUTION_Y 768

namespace Utility {
    std::string           readFileSrc( const std::string& filePath);

    std::vector<GLfloat>  getPositionsLocalCube();
    std::vector<GLfloat>  getNormalsLocalCube();
    std::vector<GLfloat>  getColorsLocalCube();


    void                  printMatrix(const glm::mat4& m);
}

#endif /* UTILITY_H */


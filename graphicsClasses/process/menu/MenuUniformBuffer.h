//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_MENUUNIFORMBUFFER_H
#define JUMPERBALLAPPLICATION_MENUUNIFORMBUFFER_H


#include "process/UniformBuffer.h"

class MenuUniformBuffer: public UniformBuffer {
public:

    MenuUniformBuffer(
        const vecCstShaderProgram_sptr& menuShaderPrograms
    );

    void update(GLfloat positionY);

private:
    static constexpr size_t sizeMenuUniformBuffer = sizeof(GLfloat);

    glm::vec1 _positionY;
};


#endif //JUMPERBALLAPPLICATION_MENUUNIFORMBUFFER_H

//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTEDATAVEC3_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTEDATAVEC3_H
#include "VertexAttributeDataBase.h"

class VertexAttributeDataVec3: public VertexAttributeDataBase {

public:
    explicit VertexAttributeDataVec3(const glm::vec3& value);

private:
    const glm::vec3 _value;
};


#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTEDATAVEC3_H

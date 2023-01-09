//
// Created by Sebastien Morgenthaler on 04/01/2023.
//

#include "VertexAttributeBase.h"

VertexAttributeBase::VertexAttributeBase(GLenum dataType) :
    _dataType(dataType) {

}

GLenum VertexAttributeBase::getDataType() const {
    return _dataType;
}

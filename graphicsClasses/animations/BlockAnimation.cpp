/*
 * File:   BlockAnimation.cpp
 * Author: Morgenthaler S
 *
 * Created on 15 aout 2020, 11h00
 */
#include "BlockAnimation.h"

BlockAnimation::BlockAnimation(const BlockState &block):
    _block(block)
{
}

void BlockAnimation::updateTrans()
{
    const std::array<float,6> transf = _block.localTransform();

    // we use only translation for now
    const glm::vec3 transVec { transf.at(0), transf.at(1),
        transf.at(2) };
    const glm::vec3 scaleVec { transf.at(3), transf.at(4),
        transf.at(5) };
    _translation = glm::translate(transVec);
    _scaleRotation = glm::scale(scaleVec);
    _model = _translation * _scaleRotation;
}

/*
 * File:   BlockAnimation.cpp
 * Author: Morgenthaler S
 *
 * Created on 15 aout 2020, 11h00
 */
#include "BlockAnimation.h"

BlockAnimation::BlockAnimation(const Block& block):
    _block(block){

}

void BlockAnimation::updateTrans()
{
    const std::array<float,9> transf = _block.localTransform();

    // we use only translation for now
    const glm::vec3 transVec { transf.at(0), transf.at(1), transf.at(2) };
    _translation = glm::translate(transVec);
    _model = _translation;
}

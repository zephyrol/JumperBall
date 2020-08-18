/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SharpBlock.cpp
 * Author: Morgenthaler S
 * 
 * Created on 5 octobre 2019, 12:51
 */

#include "SharpBlock.h"

SharpBlock::SharpBlock(const std::array<bool,6>& facesSharps):
            Block(true),
            _facesSharps(facesSharps)
{
}


Block::categoryOfBlocksInFile SharpBlock::getType() const {
    return Block::categoryOfBlocksInFile::Sharp;
}

Block::Effect SharpBlock::interaction(
        const JBTypes::Dir&,
        const JBTypes::timePointMs&,
        const JBTypes::vec3f& positions,
        const std::array<unsigned int,3>& posBlock) {

    auto isInSharpZone = [](
    const JBTypes::vec3f& position,
            float x_min, float x_max, 
            float y_min, float y_max,
            float z_min, float z_max ){
        
        return  position.x > x_min && position.x < x_max &&
                position.y > y_min && position.y < y_max &&
                position.z > z_min && position.z < z_max;
        
    };
    
    constexpr float sizeSharp     = 0.51f;
    constexpr float offsetCenter  = 0.19f;
    float posBlockfX = static_cast<float> (posBlock.at(0));
    float posBlockfY = static_cast<float> (posBlock.at(1));
    float posBlockfZ = static_cast<float> (posBlock.at(2));
    for (size_t i = 0 ; i < _facesSharps.size();++i) {
        
        if(_facesSharps.at(i)) {
            float posBlockfXMin = posBlockfX;
            float posBlockfXMax = posBlockfX+1;
            float posBlockfYMin = posBlockfY;
            float posBlockfYMax = posBlockfY+1;
            float posBlockfZMin = posBlockfZ;
            float posBlockfZMax = posBlockfZ+1;
            
            JBTypes::Dir dir = 
                    JBTypesMethods::integerAsDirection(
                        static_cast<unsigned int>(i));
            JBTypes::vec3f dirVec = 
                    JBTypesMethods::directionAsVector(dir);
            
            if ( dirVec.x > EPSILON_F || dirVec.x < -EPSILON_F) {
              posBlockfYMin +=  offsetCenter;
              posBlockfYMax -=  offsetCenter;
              posBlockfZMin +=  offsetCenter;
              posBlockfZMax -=  offsetCenter;
              if (dirVec.x < 0 )  {
                  posBlockfXMin -= sizeSharp;
              }
              else if (dirVec.x > 0) {
                  posBlockfXMax += sizeSharp;               
              }
            }
            if ( dirVec.y > EPSILON_F || dirVec.y < -EPSILON_F) {
              posBlockfXMin +=  offsetCenter;
              posBlockfXMax -=  offsetCenter;
              posBlockfZMin +=  offsetCenter;
              posBlockfZMax -=  offsetCenter;
              if (dirVec.y < 0 )  {
                  posBlockfYMin -= sizeSharp;               
              }
              else if (dirVec.y > 0) {
                  posBlockfYMax += sizeSharp;               
              }
            }
            if ( dirVec.z > EPSILON_F || dirVec.z < -EPSILON_F) {
              posBlockfXMin +=  offsetCenter;
              posBlockfXMax -=  offsetCenter;
              posBlockfYMin +=  offsetCenter;
              posBlockfYMax -=  offsetCenter;
              if (dirVec.y < 0 )  {
                  posBlockfZMin -= sizeSharp;               
              }
              else if (dirVec.y > 0) {
                  posBlockfZMax += sizeSharp;               
              }
            }

            if (isInSharpZone(positions,
                    posBlockfXMin,
                    posBlockfXMax,
                    posBlockfYMin,
                    posBlockfYMax,
                    posBlockfZMin,
                    posBlockfZMax
                    )){
                //_hitBall = true;
                return Block::Effect::Burst;
            }
        }
    }
    return Block::Effect::Nothing;
}


std::array<bool, 6 > SharpBlock::faceInfo() const {
    return _facesSharps;
}


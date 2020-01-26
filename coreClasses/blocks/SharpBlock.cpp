/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SharpBlock.cpp
 * Author: seb
 * 
 * Created on 5 octobre 2019, 12:51
 */

#include "SharpBlock.h"

SharpBlock::SharpBlock() : 
            _facesSharps{true,true,true,true,true,true},
            _hitBall(false)
{
}

SharpBlock::~SharpBlock() {
}

Block::categoryOfBlocksInFile SharpBlock::getType() const {
    return Block::categoryOfBlocksInFile::Sharp;
}

void SharpBlock::interaction(
const JumperBallTypes::Direction& ballDir, 
        const JumperBallTypes::timePointMs& currentTime, 
        const JumperBallTypes::vec3f& positions,
        const std::array<unsigned int,3>& posBlock) {

    static_cast<void>(ballDir);
    static_cast<void>(currentTime);
    static_cast<void>(posBlock);
    auto isInSharpZone = [](
    const JumperBallTypes::vec3f& position,
            float x_min, float x_max, 
            float y_min, float y_max,
            float z_min, float z_max ){
        
        return  position.x > x_min && position.x < x_max &&
                position.y > y_min && position.y < y_max &&
                position.z > z_min && position.z < z_max;
        
    };
    
    constexpr float sizeSharp = 0.6f;
    float posBlockfX = static_cast<float> (positions.x);
    float posBlockfY = static_cast<float> (positions.y);
    float posBlockfZ = static_cast<float> (positions.z);
    for (size_t i = 0 ; i < _facesSharps.size();++i) {
        
        if(_facesSharps.at(i)) {
            float posBlockfXMin = posBlockfX;
            float posBlockfXMax = posBlockfX+1;
            float posBlockfYMin = posBlockfY;
            float posBlockfYMax = posBlockfY+1;
            float posBlockfZMin = posBlockfZ;
            float posBlockfZMax = posBlockfZ+1;
            
            JumperBallTypes::Direction dir = 
                    JumperBallTypesMethods::integerAsDirection(i);
            JumperBallTypes::vec3f dirVec = 
                    JumperBallTypesMethods::directionAsVector(dir);
            if (dirVec.x < 0 ) 
              posBlockfXMin -= sizeSharp;               
            else 
              posBlockfXMax += sizeSharp;               
            if (dirVec.y < 0 ) 
              posBlockfYMin -= sizeSharp;               
            else 
              posBlockfYMax += sizeSharp;               
            if (dirVec.z < 0 ) 
              posBlockfZMin -= sizeSharp;               
            else 
              posBlockfZMax += sizeSharp;               

            if (isInSharpZone(positions,
                    posBlockfXMin,
                    posBlockfXMax,
                    posBlockfYMin,
                    posBlockfYMax,
                    posBlockfZMin,
                    posBlockfZMax
                    ))
                    _hitBall = true;
        }
    }
}


std::array<bool, 6 > SharpBlock::faceInfo() const {
    return _facesSharps;
}


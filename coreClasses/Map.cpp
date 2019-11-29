/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.cpp
 * Author: seb
 * 
 * Created on 1 octobre 2019, 21:18
 */

#include <iostream>
#include "Map.h"

unsigned int Map::nbMaps = 0;

Map::Map() : _id (nbMaps),
             //_blocks{},
             _map3DData{},
             _boundingBoxXMax (0),
             _boundingBoxYMax (0),
             _boundingBoxZMax (0),
             _beginX (0),
             _beginY (0),
             _beginZ (0)
{
    Map::nbMaps++;
}
Map::Map(std::ifstream& file):_id (nbMaps),        
                             //_blocks{},
                             _map3DData{},
                             _boundingBoxXMax (0),
                             _boundingBoxYMax (0),
                             _boundingBoxZMax (0),
                             _beginX (0),
                             _beginY (0),
                             _beginZ (0)
{
    unsigned int width;
    unsigned int deep;
    unsigned int height;

    file >> width; 
    file >> deep; 
    file >> height;
    
    _boundingBoxXMax = width;
    _boundingBoxYMax = height;
    _boundingBoxZMax = deep;

    file >> _beginX;
    file >> _beginZ;
    file >> _beginY;

    for (unsigned int i = 0 ; i < width * deep * height ; ++i) {

        unsigned int valueBlock ;
        std::shared_ptr <Block> block = nullptr ;

        file >> valueBlock;
        switch (valueBlock) {
            case 0:
              break ;
            case 1:
              block = std::make_shared<BaseBlock>(); break ;
            case 2:
              block = std::make_shared<FireBlock>(); break ;
            case 3:
              block = std::make_shared<IceBlock>(); break ;
            case 4:
              block = std::make_shared<SpicyBlock>(); break ;
            case 5:
              block = std::make_shared<BrittleBlock>(); break ;
            default :
               break;
        }
        
        _map3DData.push_back(block); 

    }
    Map::nbMaps++;
}

/*Map::Map(const Map& orig) {
}*/

Map::~Map() {
}

std::shared_ptr<Block> Map::map3DData(int x, int y, int z) const {
    
    std::shared_ptr<Block> block;
    if (x >= static_cast<int>(_boundingBoxXMax) ||  
            y >= static_cast<int>(_boundingBoxYMax) ||
            z >= static_cast<int>(_boundingBoxZMax) ||
            x < 0 || y < 0 || z < 0 )
        block = nullptr;
    else 
        block = _map3DData.at(_boundingBoxXMax* (z + y * _boundingBoxZMax) + x);
    return block;
}

void Map::printMap() const {
    for ( unsigned int y = 0 ; y < _boundingBoxYMax ; y++ ) {
        for ( unsigned int z = 0 ; z < _boundingBoxZMax ; z++ ){
            for ( unsigned int x = 0 ; x < _boundingBoxXMax ; x++ ){
                const std::shared_ptr<const Block>& block = map3DData(x,y,z);
                if (block) {
                    std::cout << static_cast<const unsigned int>
                            (block->getType()) << " ";
                }
                else std::cout << static_cast<const unsigned int>
                            (Block::categoryOfBlocksInFile::None) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

unsigned int Map::boundingBoxXMax() const {
    return _boundingBoxXMax;
}

unsigned int Map::boundingBoxYMax() const {
    return _boundingBoxYMax;
}

unsigned int Map::boundingBoxZMax() const {
    return _boundingBoxZMax;
}

unsigned int Map::beginX() const {
    return _beginX;
}

unsigned int Map::beginY() const {
    return _beginY;
}

unsigned int Map::beginZ() const {
    return _beginZ;
}



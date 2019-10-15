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

#include "Map.h"

unsigned int Map::nbMaps = 0;

Map::Map() : _id (nbMaps),
             //_blocks{},
             _map3DData{},
             _boundingBoxXMax (0),
             _boundingBoxYMax (0),
             _boundingBoxZMax (0)
{
    Map::nbMaps++;
}
Map::Map(std::ifstream& file):_id (nbMaps),        
                             //_blocks{},
                             _map3DData{},
                             _boundingBoxXMax (0),
                             _boundingBoxYMax (0),
                             _boundingBoxZMax (0)
{
    unsigned int width;
    unsigned int deep;
    unsigned int height;

    file >> width; 
    file >> deep; 
    file >> height;

    /*auto getXYZ = [&width,&deep,&height] (unsigned int i) 
        -> std::vector<unsigned int>
    {
       unsigned int x = (i%(width*deep))%width;
       unsigned int z = (i%(width*deep))/width;
       unsigned int y = i/height; 

       return std::vector<unsigned int> {x,y,z};
    };*/

    for (unsigned int i = 0 ; i < width * deep * height ; ++i) {

        unsigned int valueBlock ;
        Map::categoryOfBlocksInFile blockType;
        (void)blockType;
        std::shared_ptr <Block> block = nullptr ;

        file >> valueBlock;
        switch (valueBlock) {
            case 0:
              blockType = Map::categoryOfBlocksInFile::None;
              break ;
            case 1:
              blockType = Map::categoryOfBlocksInFile::Base;
              block = std::make_shared<BaseBlock>();
              break ;
            case 2:
              blockType = Map::categoryOfBlocksInFile::Fire;
              block = std::make_shared<FireBlock>();
              break ;
            case 3:
              blockType = Map::categoryOfBlocksInFile::Ice;
              block = std::make_shared<IceBlock>();
              break ;
            case 4:
              blockType = Map::categoryOfBlocksInFile::Spicy;
              block = std::make_shared<SpicyBlock>();
              break ;
            default :
              blockType = Map::categoryOfBlocksInFile::Brittle;
              block = std::make_shared<BrittleBlock>();
               break;
        }
        
        _map3DData.push_back(block); 

    }
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
        block = _map3DData.at(_boundingBoxXMax* (y + z * _boundingBoxYMax) + x);
    return block;
}

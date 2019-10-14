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
#include <cassert>

unsigned int Map::nbMaps = 0;

Map::Map() : _id (nbMaps),
             _blocks{},
             _map3DData{},
             _boundingBoxXMax (0),
             _boundingBoxYMax (0),
             _boundingBoxZMax (0)
{
    Map::nbMaps++;
}

/*Map::Map(const Map& orig) {
}*/

Map::~Map() {
}

std::shared_ptr<Block> Map::map3DData(unsigned int x, unsigned int y,
    unsigned int z) const {

    assert (x < _boundingBoxXMax ||  y < _boundingBoxYMax 
            || z < _boundingBoxZMax );
        
    return _map3DData.at( z *_boundingBoxYMax * y * _boundingBoxXMax + x);
}

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

Map::Map() : _id (nbMaps),
             _blocks{},
             _boundingBoxXMax (0),
             _boundingBoxYMax (0)
{
}

/*Map::Map(const Map& orig) {
}*/

Map::~Map() {
}


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.h
 * Author: seb
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef MAP_H
#define MAP_H
#include "Types.h"
#include "blocks/Block.h"

class Map {
public:
    enum class KindOfData {Raw,Optimized};
    
    Map();
    Map(const Map& orig);
    Map(const std::string content, Map::KindOfData kData );
    Map(std::ifstream file);
    virtual ~Map();
private:
    unsigned int _id;
    std::vector<Block> _blocks;
    unsigned int _boundingBoxXMax;
    unsigned int _boundingBoxYMax;
    
    static unsigned int nbMaps;
};

unsigned int Map::nbMaps = 0;
#endif /* MAP_H */


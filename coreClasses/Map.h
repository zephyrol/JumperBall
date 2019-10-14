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

    std::shared_ptr<Block> map3DData(int x, int y, int z) const;

    virtual ~Map();
    
private:
    const unsigned int _id;
    std::vector<Block> _blocks;
    std::vector< std::shared_ptr<Block> > _map3DData;
    unsigned int _boundingBoxXMax;
    unsigned int _boundingBoxYMax;
    unsigned int _boundingBoxZMax;
    
    static unsigned int nbMaps;
};

#endif /* MAP_H */


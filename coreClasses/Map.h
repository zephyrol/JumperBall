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

class Map {
public:
    Map();
    Map(const Map& orig);
    Map(const std::string content);
    Map(std::ifstream file);
    virtual ~Map();
private:
    unsigned int _id;
    std::vector _blocks;
    unsigned int _boundingBoxXMax;
    unsigned int _boundingBoxYMax;
  
};

#endif /* MAP_H */


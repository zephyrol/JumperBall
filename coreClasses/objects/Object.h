/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Object.h
 * Author: seb
 *
 * Created on 27 mars 2020, 19:40
 */

#ifndef OBJECT_H
#define OBJECT_H
#include "../Types.h"
#include "../blocks/Block.h"

class Object {
public:
    Object( const std::shared_ptr<const Block>& block, 
            JumperBallTypes::Direction side);

    virtual ~Object() = default;

    enum class CategoryOfObjects {Key,Coin,Clock};

    virtual CategoryOfObjects getCategory() = 0;

private:
    const std::shared_ptr<const Block>                _block;  
    JumperBallTypes::Direction                        _side;
    bool                                              _gotten;
};

#endif /* OBJECT_H */


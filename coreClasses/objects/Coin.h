/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Coin.h
 * Author: seb
 *
 * Created on 27 mars 2020, 20:49
 */

#ifndef COIN_H
#define COIN_H
#include "Bonus.h"

class Coin : public Bonus {
public:
    Coin( const std::shared_ptr<const Block>& block,
          JumperBallTypes::Direction side);
    Object::CategoryOfObjects getCategory() override;

private:

};

#endif /* COIN_H */


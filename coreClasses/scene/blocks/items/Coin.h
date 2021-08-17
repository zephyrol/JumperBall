/*
 * File: Coin.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:49
 */

#ifndef COIN_H
#define COIN_H
#include "Bonus.h"

class Coin:public Bonus {
public:
Coin(const JBTypes::vec3ui& position, const JBTypes::Dir& direction);
vecCstShape_sptr getShapes() const override;

private:

};

#endif /* COIN_H */

/*
 * File: Coin.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:49
 */

#ifndef COIN_H
#define COIN_H

#include "Bonus.h"

class Coin : public Bonus {
public:
    Coin(const glm::u32vec3 &position, const JBTypes::Dir &direction, const Ball_sptr &ball);

    vecCstShape_sptr getShapes() const override;

private:
    void catchItem() override;

};

#endif /* COIN_H */

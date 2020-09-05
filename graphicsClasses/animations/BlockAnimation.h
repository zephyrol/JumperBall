#ifndef BLOCKANIMATION_H
#define BLOCKANIMATION_H
#include <blocks/Block.h>
#include "../Utility.h"
#include "Animation.h"

/*
 * File:   BlockAnimation.h
 * Author: Morgenthaler S
 *
 * Created on 15 aout 2020, 11h00
 */

class BlockAnimation : public Animation {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    BlockAnimation                 (const Block& block);
    virtual ~BlockAnimation        ()                                  =default;


    //----------METHODS------------//
    void                          updateTrans()                        override;

private:

    //--------ATTRIBUTES-----------//
    const Block&                  _block;
};


#endif // BLOCKANIMATION_H

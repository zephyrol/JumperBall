#ifndef BLOCKANIMATION_H
#define BLOCKANIMATION_H
#include "scene/GraphicBlock.h"
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
    BlockAnimation                 (const GraphicBlock& block);
    virtual ~BlockAnimation        ()                                  =default;


    //----------METHODS------------//
    void                          updateTrans()                        override;

private:
    //--------ATTRIBUTES-----------//
    const GraphicBlock&                  _block;
};


#endif // BLOCKANIMATION_H

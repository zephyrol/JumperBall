/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Block.h
 * Author: seb
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef BLOCK_H
#define BLOCK_H

class Block {
public:
    Block();
    Block(const Block& orig);
    virtual ~Block() ;

    enum class categoryOfBlocksInFile{None,Base,Fire,Ice,Spicy,Brittle};

    virtual void interaction() = 0;
    virtual categoryOfBlocksInFile getType() const = 0;

private:

};

#endif /* BLOCK_H */


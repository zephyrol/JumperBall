/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Block.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef BLOCK_H
#define BLOCK_H
#include "../Types.h"
#include "../objects/Clock.h"
#include "../objects/Coin.h"
#include "../objects/Key.h"

class Block {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Block                             ();
    Block                             (const std::array<float,9>& 
                                                                localTransform);
    virtual                           ~Block()                        = default;


    //------------TYPES------------//
    enum class categoryOfBlocksInFile {None,Base,Fire,Ice,Sharp,Brittle,Jump};


    //----------METHODS-------------//
    virtual void                      interaction(
                                    const JBTypes::Dir& 
                                            ballDir,
                                    const JBTypes::timePointMs&
                                            currentTime,
                                    const JBTypes::vec3f& posBall,
                                    const std::array<unsigned int,3>& posBlock);
    virtual void                      detectionEvent(
                                        const JBTypes::Dir&
                                                ballDir,
                                        const JBTypes::timePointMs&
                                                currentTime
                                                );
    virtual void                      createObject(
                                        Object::CategoryOfObjects category, 
                                        JBTypes::Dir dir);


    //-------CONST METHODS----------//
    virtual                           categoryOfBlocksInFile getType() const =0;
    virtual std::array<bool,6>        faceInfo()                          const;
    virtual bool                      stillExists()                       const;
    virtual bool                      burstBall()                         const;
    virtual const std::array<float,9>&       
                                      localTransform()                    const;
    virtual const std::array<std::shared_ptr<const Object>,6 >&
                                      objects()                           const; 

    //--------STATIC METHODS-------//
    static JBTypes::vec3f             positionObject
                                        (const std::array<unsigned int,3>& pos,
                                         unsigned int dirUint);

protected:
    //--------ATTRIBUTES-----------//
    std::array<float,9>               _localTransform;
    std::array<std::shared_ptr<const Object>,6> 
                                      _objects;
};

#endif /* BLOCK_H */


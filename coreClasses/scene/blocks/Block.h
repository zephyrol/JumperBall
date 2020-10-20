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
#include "system/Types.h"
#include "scene/objects/Clock.h"
#include "scene/objects/Coin.h"
#include "scene/objects/Key.h"

class Block {
public:


    //--CONSTRUCTORS & DESTRUCTORS--//
    Block                             (bool hasInteraction = false,
                                       bool isFixed = true);
    virtual                           ~Block()                        = default;


    //------------TYPES------------//
    enum class Effect                 { Nothing,Burst,Burn,Slide,Jump};


    //----------METHODS-------------//
    virtual Effect                    interaction(
                                    const JBTypes::Dir& ballDir,
                                    const JBTypes::timePointMs& currentTime,
                                    const JBTypes::vec3f& posBall,
                                    const std::array<unsigned int,3>& posBlock);
    virtual Effect                    detectionEvent(
                                        const JBTypes::Dir& ballDir,
                                        const JBTypes::timePointMs& currentTime
                                                );
    virtual void                      createObject(
                                        Object::CategoryOfObjects category, 
                                        JBTypes::Dir dir);


    //-------CONST METHODS----------//
    virtual std::array<bool,6>        faceInfo()                          const;
    virtual bool                      stillExists()                       const;
    virtual const std::array<float,6>&       
                                      localTransform()                    const;
    virtual const std::shared_ptr<const Object> object(size_t number)     const;
    virtual bool                      hasInteraction()                    const;
    virtual bool                      hasObjects()                        const;
    virtual void                      catchObject(
                                const std::array<unsigned int, 3>&
                                    blockPosition,
                                const JBTypes::vec3f& entityPosition,
                                float radiusEntity);
    virtual const bool &isFixed()                           const;

    //--------STATIC METHODS-------//
    static JBTypes::vec3f             objectPosition
                                        (const std::array<unsigned int,3>& pos,
                                         unsigned int dirUint);
    static constexpr size_t           objectsNumber = 6;

    const std::array<std::shared_ptr<Object>,6>& objects() const;

protected:
    //--------ATTRIBUTES-----------//
    std::array<float,6>               _localTransform;
    std::array<std::shared_ptr<Object>,6> 
                                      _objects;
    const bool                        _hasInteraction;
    bool                              _hasObjects;
    const bool                        _isFixed;
};

#endif /* BLOCK_H */

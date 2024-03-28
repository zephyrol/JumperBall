//
// Created by S.Morgenthaler on 28/03/2024.
//

#ifndef MOVABLEOBJECT_H
#define MOVABLEOBJECT_H

class MovableObject;
using CstMovableObject_sptr = std::shared_ptr<const MovableObject>;

class MovableObject {
public:
    virtual ~MovableObject() = default;
    virtual bool isTurningRight() const = 0;
    virtual bool isTurningLeft() const = 0;
    virtual bool isGoingAhead() const = 0;
    virtual bool isJumping() const = 0;
    virtual bool hasChangedAFace() const = 0;
    virtual unsigned int numberOfKeys() const = 0;
};



#endif //MOVABLEOBJECT_H

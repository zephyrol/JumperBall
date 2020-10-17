/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Object.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#ifndef OBJECT_H
#define OBJECT_H
#include "system/Types.h"

class Object {
public:

    enum class CategoryOfObjects {Key,Coin,Clock};

    Object(const CategoryOfObjects& category);
    virtual ~Object() = default;
    const CategoryOfObjects& getCategory() const ;
    
    bool isGotten() const;
    const JBTypes::timePointMs& timeOfObtaining() const;
    const JBTypes::timePointMs& timeOfCreation() const;
    void catchingTest( const JBTypes::vec3f& objectPosition,
                       const JBTypes::vec3f& entityPosition,
                       float radiusEntity);

    static constexpr float radiusBoundingSphere = 0.15f;
private:
    bool _gotten;
    const JBTypes::timePointMs _timeOfCreation;
    JBTypes::timePointMs _timeOfObtaining;
    const CategoryOfObjects _category;
};

#endif /* OBJECT_H */


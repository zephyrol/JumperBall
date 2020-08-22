
/* 
 * File:   GraphicBody.h
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2019, 15:56
 */

#ifndef GRAPHICBODY_H
#define GRAPHICBODY_H
#include "../MeshComponent.h"
#include "../../geometry/GeometricShape.h"

class GraphicBody
{
public:
    GraphicBody();
    virtual std::vector<MeshComponent> genComponents() const = 0;
    virtual ~GraphicBody() = default;

private:
};

#endif // GRAPHICBODY_H
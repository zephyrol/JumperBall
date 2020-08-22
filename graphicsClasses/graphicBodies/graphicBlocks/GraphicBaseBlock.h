/*
 * File:   GraphicBaseBlock.h
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2019, 11:07
 */

#ifndef GRAPHICBLOCK_H
#define GRAPHICBLOCK_H
#include "../GraphicBody.h"

class GraphicBaseBlock : public GraphicBody
{
public:
    GraphicBaseBlock();

private:
    static std::map<std::string, std::shared_ptr<GeometricShape> >
                                commonBaseBlockShapes;

};

#endif // GRAPHICBLOCK_H

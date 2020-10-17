/*
 * File:   GraphicBlock.h
 * Author: Morgenthaler S
 *
 */
#ifndef GRAPHICBLOCK_H
#define GRAPHICBLOCK_H
#include <scene/blocks/Block.h>
#include "GraphicObject.h"

class GraphicBlock
{
public:
    GraphicBlock(const Block& block);
    void update();

    const std::array<float,6>& localTransform() const;
    const Block& block() const;
    const std::array<std::shared_ptr<GraphicObject>, 6>& graphicObjects() const;
    const bool& isFixed() const;

private:
    const Block&        _block;
    std::array<float,6> _localTransform;
    std::array<std::shared_ptr<GraphicObject>, 6> _graphicObjects;
    const bool&         _isFixed;

    std::array<std::shared_ptr<GraphicObject>, 6> createGraphicObjects() const;
};

#endif // GRAPHICBLOCK_H

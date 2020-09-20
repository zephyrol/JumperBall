#ifndef GRAPHICBLOCK_H
#define GRAPHICBLOCK_H
#include <blocks/Block.h>
#include "GraphicObject.h"

class GraphicBlock
{
public:
    GraphicBlock(const Block& block);
    void update();

    const std::array<float,9>& localTransform() const;
    const Block& block() const;
    const std::array<std::shared_ptr<GraphicObject>, 6>& graphicObjects() const;

private:
    const Block&        _block;
    std::array<float,9> _localTransform;
    std::array<std::shared_ptr<GraphicObject>, 6> _graphicObjects;

    std::array<std::shared_ptr<GraphicObject>, 6> createGraphicObjects() const;
};

#endif // GRAPHICBLOCK_H

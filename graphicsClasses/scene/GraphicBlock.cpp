#include "GraphicBlock.h"

GraphicBlock::GraphicBlock(const Block& block):
    _block(block),
    _localTransform(block.localTransform()),
    _graphicObjects(createGraphicObjects()),
    _isFixed(block.isFixed())
{
}

void GraphicBlock::update()
{
    _localTransform = _block.localTransform();
    for (std::shared_ptr<GraphicObject>& graphicObject: _graphicObjects){
        if (graphicObject){
            graphicObject->update();
        }
    }
}

const std::array<float, 6> &GraphicBlock::localTransform() const
{
   return _localTransform;
}

const Block &GraphicBlock::block() const
{
    return _block;
}

const std::array<std::shared_ptr<GraphicObject>, 6> &
GraphicBlock::graphicObjects() const
{
    return _graphicObjects;
}

const bool &GraphicBlock::isFixed() const
{
    return _isFixed;
}

std::array<std::shared_ptr<GraphicObject>, 6>
    GraphicBlock::createGraphicObjects() const
{
    std::array<std::shared_ptr<GraphicObject>, 6> graphicObjects;

    for (size_t i = 0; i < _block.objects().size(); ++i){
        if (const auto& object = _block.objects().at(i)) {
            graphicObjects.at(i) = std::make_shared<GraphicObject>(*object);
        }
    }

    return graphicObjects;
}


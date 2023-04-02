/*
 * File: Map.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

#include "Page.h"
#include "gameMenu/nodes/LeftNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/DownNode.h"
#include <algorithm>

Page::Page(Player_sptr &&player) :
    _player(std::move(player)),
    _currentSelectedLabel(0) // id 0 means "no selection"
{
}

Page_wptr Page::parent() {
    return {};
}

void Page::update(const Mouse &) {
}

vecCstLabel_sptr Page::labels() const {
    return {};
}

std::vector<std::string> Page::shaderDefines() const {
    return {};
}

Displayable::GlobalState Page::getGlobalState() const {
    return GlobalState::United;
}

Displayable::DynamicNames Page::getDynamicIntNames() const {
    return {"selectedLabel"};
}

Displayable::DynamicValues<int> Page::getDynamicIntValues() const {
    return {_currentSelectedLabel};
}

std::vector<std::pair<std::string, float> > Page::getVertexShaderConstants() const {
    return {};
}

std::shared_ptr<ArrowLabel> Page::createArrowLabel(
    const Node_sptr &headerNode,
    int labelId,
    bool onUp,
    float ratio
) {
    const auto leftNode = std::make_shared<LeftNode>(headerNode, 1.f);
    Node_sptr node;
    if (onUp) {
        node = std::make_shared<UpNode>(leftNode, ratio);
    } else {
        node = std::make_shared<DownNode>(leftNode, ratio);
    }
    auto arrowLabel = std::make_shared<ArrowLabel>(
        node,
        JBTypes::Color::Blue,
        labelId
    );
    return arrowLabel;
}

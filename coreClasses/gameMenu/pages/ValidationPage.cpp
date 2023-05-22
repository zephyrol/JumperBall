//
// Created by S.Morgenthaler on 22/05/2023.
//

#include "ValidationPage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/DownNode.h"

ValidationPage_sptr ValidationPage::createInstance(
    size_t skinNumber,
    Player_sptr player,
    const Page_sptr &parent,
    float ratio
) {
    auto nodes = createNodes(ratio);
    return std::make_shared<ValidationPage>(
        skinNumber,
        std::move(player),
        std::move(nodes.at(0)),
        std::move(nodes.at(1)),
        std::move(nodes.at(2)),
        createBackgroundLabel(nodes.at(3)),
        parent
    );
}

ValidationPage::ValidationPage(
    size_t skinNumber,
    Player_sptr &&player,
    Node_sptr &&buyNode,
    Node_sptr &&yesNode,
    Node_sptr &&noNode,
    Label_sptr &&backgroundLabel,
    const Page_sptr &parent
) : Page(std::move(player)),
    _skinNumber(skinNumber),
    _parent(parent),
    _buyNode(std::move(buyNode)),
    _yesNode(std::move(yesNode)),
    _noNode(std::move(noNode)),
    _backgroundLabel(std::move(backgroundLabel)) {
}

vecNode_sptr ValidationPage::createNodes(float ratio) {
    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto mainTitleNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 16.f
    );

    const auto buyTitle = std::make_shared<UpNode>(
        mainTitleNode,
        4.f
    );

    const auto optionsParentNode = std::make_shared<CenteredNode>(
        mainTitleNode,
        1.5f
    );
    constexpr float optionsNodeRatio = 7.f;

    const auto yesNode = std::make_shared<UpNode>(
        optionsParentNode,
        optionsNodeRatio
    );

    const auto noNode = std::make_shared<DownNode>(
        optionsParentNode,
        optionsNodeRatio
    );

    return {buyTitle, yesNode, noNode, optionsParentNode};
}

void ValidationPage::resize(float ratio) {
    const auto &nodes = createNodes(ratio);
    _buyNode = nodes.at(0);
    _yesNode = nodes.at(1);
    _noNode = nodes.at(2);
    _backgroundLabel = createBackgroundLabel(nodes.at(3));
}

Page_wptr ValidationPage::parent() {
    return _parent;
}

vecCstTextNode_uptr ValidationPage::genTextNodes() const {
    vecCstTextNode_uptr textNodes;
    const auto english = _player->isUsingEnglishLanguage();
    textNodes.emplace_back(new TextNode(_buyNode, english ? "Do you want to buy?" : "Voulez-vous l'acheter?", 0));
    textNodes.emplace_back(new TextNode(_yesNode, english ? "Yes" : "Oui", yesLabelId));
    textNodes.emplace_back(new TextNode(_noNode, english ? "No" : "Non", noLabelId));
    return textNodes;
}

std::vector<std::string> ValidationPage::shaderDefines() const {
    return {"TRANSPARENT_BACKGROUND"};
}

std::string ValidationPage::getVertexShaderName() const {
    return "titlePageVs.vs";
}

Page_sptr ValidationPage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_yesNode)) {
        return _parent.lock();
    }
    if (intersectTest(_noNode)) {
        return _parent.lock();
    }
    return nullptr;
}

void ValidationPage::update(const Mouse &mouse) {
    if (!mouse.isPressed()) {
        _currentSelectedLabel = 0;
        return;
    }

    // Positions have to be centered
    const auto mouseX = mouse.currentXCoord() - 0.5f;
    const auto mouseY = mouse.currentYCoord() - 0.5f;
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_yesNode)) {
        _currentSelectedLabel = yesLabelId;
    } else if (intersectTest(_noNode)) {
        _currentSelectedLabel = noLabelId;
    } else {
        _currentSelectedLabel = 0;
    }
}

vecCstLabel_sptr ValidationPage::labels() const {
    return {_backgroundLabel};
}


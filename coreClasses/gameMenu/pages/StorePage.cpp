//
// Created by S.Morgenthaler on 14/05/2023.
//

#include "StorePage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/DownNode.h"
#include "gameMenu/nodes/LeftNode.h"
#include "gameMenu/nodes/RightNode.h"
#include "gameMenu/nodes/VerticalNode.h"

StorePage::StorePage(
    Player_sptr &&player,
    Node_sptr &&storeNode,
    Node_sptr &&exitNode,
    std::array<BallSkin, StorePage::numberOfSkins> &&ballSkins,
    Label_sptr &&backgroundLabel,
    const Page_sptr &parent
) : Page(std::move(player)),
    _parent(parent),
    _storeNode(std::move(storeNode)),
    _exitNode(std::move(exitNode)),
    _ballSkins(std::move(ballSkins)),
    _backgroundLabel(std::move(backgroundLabel)) {

}

StorePage_sptr StorePage::createInstance(Player_sptr player, const Page_sptr &parent, float ratio) {
    auto nodes = createNodes(ratio, player->isUsingEnglishLanguage());

    return std::make_shared<StorePage>(
        std::move(player),
        std::move(nodes.at(0)),
        std::move(nodes.at(1)),
        createBallSkins(nodes),
        createBackgroundLabel(std::move(nodes.at(2))),
        parent
    );
}


void StorePage::update(const Mouse &mouse) {
    Page::update(mouse);
}

Page_sptr StorePage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_exitNode)) {
        return _parent.lock();
    }
    return nullptr;
}

void StorePage::resize(float ratio) {
    const auto nodes = createNodes(ratio, _player->isUsingEnglishLanguage());
    _storeNode = nodes.at(0);
    _exitNode = nodes.at(1);
    _ballSkins = createBallSkins(nodes);
    _backgroundLabel = createBackgroundLabel(nodes.at(2));
}

Displayable::DynamicNames StorePage::getDynamicIntNames() const {
    return Page::getDynamicIntNames();
}

Displayable::DynamicValues<int> StorePage::getDynamicIntValues() const {
    return Page::getDynamicIntValues();
}

Page_wptr StorePage::parent() {
    return Page::parent();
}

std::string StorePage::getVertexShaderName() const {
    // return "storePageVs.vs";
    return "titlePageVs.vs";
}

vecCstTextNode_uptr StorePage::genTextNodes() const {
    const auto english = _player->isUsingEnglishLanguage();
    vecCstTextNode_uptr textNodes;
    textNodes.emplace_back(new TextNode(_storeNode, english ? "Store" : "Boutique", 0));
    textNodes.emplace_back(new TextNode(_exitNode, english ? "Return" : "Sortir", 0));

    short id = -1;
    const auto selectString = english ? "Select" : "Choisir";
    for(const auto& ballSkin: _ballSkins) {
        textNodes.emplace_back(new TextNode(ballSkin.digitNode, std::to_string(id + 1), id));
        textNodes.emplace_back(new TextNode(ballSkin.zeroZeroNode, "00", id));
        ++id;
        textNodes.emplace_back( new TextNode(ballSkin.selectNode, selectString, id));
        ++id;
    }

    return textNodes;
}

std::vector<std::string> StorePage::shaderDefines() const {
    // return {"TRANSPARENT_BACKGROUND", "DISCARDING", "TEST_COIN"};
    return {"TRANSPARENT_BACKGROUND"};
}

vecCstLabel_sptr StorePage::labels() const {
    return {
        _backgroundLabel,
        _ballSkins.at(0).coinSymbol,
        _ballSkins.at(1).coinSymbol,
        _ballSkins.at(2).coinSymbol,
        _ballSkins.at(3).coinSymbol,
        _ballSkins.at(4).coinSymbol,
        _ballSkins.at(5).coinSymbol
    };
}

vecNode_sptr StorePage::createNodes(float ratio, bool english) {
    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto mainStoreNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 16.f
    );

    constexpr auto storeTitleRatio = 4.f;
    const auto jumperBallStore = std::make_shared<UpNode>(
        mainStoreNode,
        storeTitleRatio
    );

    constexpr float optionsNodeRatio = 7.f;
    const auto exitNode = std::make_shared<VerticalNode>(
        mainStoreNode,
        optionsNodeRatio,
        english ? -0.4f : -0.45f // Because Q creates an offset
    );

    constexpr auto skinsNodeRatio = 9.f / 10.f;
    const auto skinsNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        skinsNodeRatio
    );

    constexpr auto upDownSkinsRatio = skinsNodeRatio * 2.f;
    const auto upSkins = std::make_shared<UpNode>(
        skinsNode,
        upDownSkinsRatio
    );
    constexpr auto skinNodesRatio = upDownSkinsRatio / 3.f;
    const auto upFirstSkin = std::make_shared<LeftNode>(
        upSkins,
        skinNodesRatio
    );
    const auto upSecondsSkin = std::make_shared<CenteredNode>(
        upSkins,
        skinNodesRatio
    );
    const auto upThirdSkin = std::make_shared<RightNode>(
        upSkins,
        skinNodesRatio
    );

    const auto downSkins = std::make_shared<DownNode>(
        skinsNode,
        skinNodesRatio
    );
    const auto downFirstSkin = std::make_shared<LeftNode>(
        upSkins,
        skinNodesRatio
    );
    const auto downSecondsSkin = std::make_shared<CenteredNode>(
        upSkins,
        skinNodesRatio
    );
    const auto downThirdSkin = std::make_shared<RightNode>(
        upSkins,
        skinNodesRatio
    );

    const auto createBallSkin = [](const Node_sptr &coveringNode) -> std::vector<Node_sptr> {
        const auto baseNode = std::make_shared<CenteredNode>(coveringNode, 1.f);
        const auto scaledBaseNode = std::make_shared<ScaledNode>(baseNode, 2.f / 3.f);

        const auto selectNode = std::make_shared<DownNode>(scaledBaseNode, 4.f);
        const auto priceNode = std::make_shared<LeftNode>(selectNode, 3.f);
        const auto digitNode = std::make_shared<LeftNode>(priceNode, 1.f);
        const auto zeroZeroNode = std::make_shared<LeftNode>(priceNode, 2.f);
        auto coinNode = std::make_shared<RightNode>(selectNode, 1.f);

        const auto ballBaseNode = std::make_shared<UpNode>(scaledBaseNode, 4.f / 3.f);
        const auto ballNode = std::make_shared<CenteredNode>(ballBaseNode, 1.f);

        return {
            ballNode,
            digitNode,
            zeroZeroNode,
            selectNode,
            coinNode,
            coveringNode
        };
    };

    const vecNode_sptr coveringNodes = {
        upFirstSkin,
        upSecondsSkin,
        upThirdSkin,
        downFirstSkin,
        downSecondsSkin,
        downThirdSkin
    };

    return std::accumulate(
        coveringNodes.begin(),
        coveringNodes.end(),
        std::vector<Node_sptr>{jumperBallStore, exitNode, mainStoreNode},
        [&createBallSkin](vecNode_sptr &current, const Node_sptr &coveringNode) {
            auto skinNodes = createBallSkin(coveringNode);
            current.insert(
                current.end(),
                std::make_move_iterator(skinNodes.begin()),
                std::make_move_iterator(skinNodes.end())
            );
            return current;
        }
    );
}

std::array<StorePage::BallSkin, StorePage::numberOfSkins> StorePage::createBallSkins(const vecNode_sptr &nodes) {
    constexpr size_t offset = 3;
    std::array<StorePage::BallSkin, StorePage::numberOfSkins> ballSkins;
    short id = -1;
    for (size_t ballSkinCount = 0; ballSkinCount < StorePage::numberOfSkins; ++ballSkinCount) {
        auto i = offset + ballSkinCount * StorePage::ballSkinNumberOfFields;
        ballSkins.at(ballSkinCount++) = {
           nodes.at(i++),
           nodes.at(i++),
           nodes.at(i++),
           nodes.at(i++),
           std::make_shared<Label>(nodes.at(i++), JBTypes::Color::Yellow, id),
           nodes.at(i++)
        };
        id += 2;
    }
    return ballSkins;
}

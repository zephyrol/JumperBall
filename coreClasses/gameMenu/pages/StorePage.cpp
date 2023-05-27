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
#include "gameMenu/nodes/HorizontalNode.h"

StorePage::StorePage(
    Player_sptr &&player,
    Node_sptr &&storeNode,
    Node_sptr &&sumDigitOne,
    Node_sptr &&sumDigitTwo,
    Node_sptr &&sumDigitThree,
    Node_sptr &&sumDigitFour,
    std::array<BallSkin, StorePage::numberOfSkins> &&ballSkins,
    Label_sptr &&coinSymbol,
    ArrowLabel_sptr &&arrowLabel,
    const Page_sptr &parent
) : Page(std::move(player)),
    _parent(parent),
    _storeNode(std::move(storeNode)),
    _ballSkins(std::move(ballSkins)),
    _sumDigitOne(std::move(sumDigitOne)),
    _sumDigitTwo(std::move(sumDigitTwo)),
    _sumDigitThree(std::move(sumDigitThree)),
    _sumDigitFour(std::move(sumDigitFour)),
    _coinSymbol(std::move(coinSymbol)),
    _arrowLabel(std::move(arrowLabel)),
    _validationPages{} {
}

StorePage_sptr StorePage::createInstance(Player_sptr player, const Page_sptr &parent, float ratio) {
    auto nodes = createNodes(ratio);

    return std::make_shared<StorePage>(
        std::move(player),
        std::move(nodes.at(0)),
        std::move(nodes.at(1)),
        std::move(nodes.at(2)),
        std::move(nodes.at(3)),
        std::move(nodes.at(4)),
        createBallSkins(nodes),
        std::make_shared<Label>(
            std::move(nodes.at(5)),
            JBTypes::Color::White,
            coinMoneyId
        ),
        createStoreArrowLabel(nodes.at(6)),
        parent
    );
}


void StorePage::update(const Mouse &mouse) {
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
    if (intersectTest(_arrowLabel->getNode())) {
        _currentSelectedLabel = static_cast<int>(_arrowLabel->getId());
    }
    for (size_t i = 0; i < _ballSkins.size(); ++i) {
        const auto getId = [&i](decltype(_currentSelectedLabel) offset) {
            const decltype(_currentSelectedLabel) id = 1000 + i * 100 + offset;
            return id;
        };
        if (intersectTest(_ballSkins[i].background->getNode())) {
            _currentSelectedLabel = getId(_player->hasBoughtSkin(i) ? 0 : 1);
        }
    }
}

Page_sptr StorePage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_arrowLabel->getNode())) {
        _player->addValidationSound();
        return _parent.lock();
    }
    for(size_t i = 0; i < _validationPages.size(); ++i) {
        if (intersectTest(_ballSkins.at(i).background->getNode())) {
            if(_player->hasBoughtSkin(i)) {
                _player->setCurrentSkin(i);
                return nullptr;
            }
            if(_player->getMoney() >= (i * 100)) {
                _player->addValidationSound();
                return _validationPages.at(i).lock();
            }
            _player->addNotEnoughMoneySound();
            return nullptr;
        }
    }
    return nullptr;
}

void StorePage::resize(float ratio) {
    const auto nodes = createNodes(ratio);
    _storeNode = nodes.at(0);
    _ballSkins = createBallSkins(nodes);
    _sumDigitOne = nodes.at(1);
    _sumDigitTwo = nodes.at(2);
    _sumDigitThree = nodes.at(3);
    _sumDigitFour = nodes.at(4);
    _coinSymbol = std::make_shared<Label>(std::move(nodes.at(5)), JBTypes::Color::White, coinMoneyId);
    _arrowLabel = createStoreArrowLabel(nodes.at(6));
}

Displayable::DynamicNames StorePage::getDynamicIntNames() const {
    decltype(getDynamicIntNames()) dynamicNames{
        "skinOne",
        "skinTwo",
        "skinThree",
        "skinFour",
        "skinFive",
        "skinSix",
        "sumDigitOne",
        "sumDigitTwo",
        "sumDigitThree",
        "sumDigitFour"
    };
    auto pageDynamicNames = Page::getDynamicIntNames();
    dynamicNames.insert(
        dynamicNames.end(),
        std::make_move_iterator(pageDynamicNames.begin()),
        std::make_move_iterator(pageDynamicNames.end())
    );
    return dynamicNames;
}

Displayable::DynamicValues<int> StorePage::getDynamicIntValues() const {
    const auto currentBallSkin = _player->getCurrentBallSkin();
    const auto getSkinValue = [&currentBallSkin, this](unsigned int skinNumber) {
        // 2 means selected
        if (skinNumber == currentBallSkin) {
            return 2;
        }
        // One means gotten, zero the player does not have it.
        return _player->hasBoughtSkin(static_cast<size_t>(skinNumber)) ? 1 : 0;
    };

    decltype(getDynamicIntValues()) dynamicInts;
    for (size_t i = 0; i < numberOfSkins; ++i) {
        dynamicInts.emplace_back(getSkinValue(i));
    }

    const auto sumMoney = static_cast<int>(_player->getMoney());
    const auto sumDigitOne = sumMoney % 10;
    const auto sumDigitTwo = (sumMoney % 100) / 10;
    const auto sumDigitThree = (sumMoney % 1000) / 100;
    const auto sumDigitFour = sumMoney / 1000;

    constexpr auto sumDigitOneOffset = digitsIdOffset;
    constexpr auto sumDigitTwoOffset = sumDigitOneOffset + 10;
    constexpr auto sumDigitThreeOffset = sumDigitTwoOffset + 10;
    constexpr auto sumDigitFourOffset = sumDigitThreeOffset + 10;

    dynamicInts.emplace_back(sumDigitOneOffset + sumDigitOne);
    dynamicInts.emplace_back(sumDigitTwoOffset + sumDigitTwo);
    dynamicInts.emplace_back(sumDigitThreeOffset + sumDigitThree);
    dynamicInts.emplace_back(sumDigitFourOffset + sumDigitFour);

    auto pageDynamicInts = Page::getDynamicIntValues();
    dynamicInts.insert(
        dynamicInts.end(),
        std::make_move_iterator(pageDynamicInts.begin()),
        std::make_move_iterator(pageDynamicInts.end())
    );
    return dynamicInts;
}

Page_wptr StorePage::parent() {
    return _parent;
}

std::string StorePage::getVertexShaderName() const {
    return "storePageVs.vs";
}

vecCstTextNode_uptr StorePage::genTextNodes() const {
    const auto english = _player->isUsingEnglishLanguage();
    vecCstTextNode_uptr textNodes;
    textNodes.emplace_back(new TextNode(_storeNode, english ? "Store" : "Boutique", 0));

    short id = 1000;
    const auto selectString = english ? "Select" : "Choisir";
    auto price = 0;
    for (const auto &ballSkin: _ballSkins) {
        textNodes.emplace_back(new TextNode(ballSkin.selectNode, selectString, id));

        const auto priceNodesId = static_cast<short>(id + 1);
        textNodes.emplace_back(
            new TextNode(ballSkin.priceNode, std::to_string(price++) + "00", priceNodesId)
        );
        id += 100;
    }

    auto nodeCount = static_cast<short>(StorePage::digitsIdOffset);
    for (const auto &node: std::vector<Node_sptr>{
        _sumDigitOne,
        _sumDigitTwo,
        _sumDigitThree,
        _sumDigitFour
    }) {
        for (unsigned int i = 0; i < 10; ++i) {
            textNodes.emplace_back(new TextNode(
                node,
                std::to_string(i),
                nodeCount
            ));
            ++nodeCount;
        }
    }

    return textNodes;
}

std::vector<std::string> StorePage::shaderDefines() const {
    return {"TRANSPARENT_BACKGROUND", "DISCARDING", "TEST_COIN", "TEST_BALL"};
}

vecCstLabel_sptr StorePage::labels() const {
    vecCstLabel_sptr labels{_coinSymbol};
    for (const auto &ballSkin: _ballSkins) {
        labels.emplace_back(ballSkin.background);
        labels.emplace_back(ballSkin.coinSymbol);
        labels.emplace_back(ballSkin.ballLabel);
    }
    labels.emplace_back(_arrowLabel);
    return labels;
}

vecNode_sptr StorePage::createNodes(float ratio) {
    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto mainStoreNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 16.f
    );

    constexpr auto storeHeaderRatio = 4.f;
    const auto storeHeaderNode = std::make_shared<UpNode>(mainStoreNode, storeHeaderRatio);
    const auto arrowNode = std::make_shared<LeftNode>(storeHeaderNode, 3.f);

    const auto getStoreNode = [&storeHeaderNode]() -> Node_sptr {
        constexpr auto storeTitleRatio = 2.5f;
        const auto storeNode = std::make_shared<RightNode>(storeHeaderNode, storeTitleRatio);
        return std::make_shared<CenteredNode>(storeNode, 4.f);
    };

    const auto storeNode = getStoreNode();
    constexpr auto skinsNodeRatio = 9.f / 10.f;
    const auto skinsNode = std::make_shared<CenteredNode>(
        mainStoreNode,
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
        upDownSkinsRatio
    );
    const auto downFirstSkin = std::make_shared<LeftNode>(
        downSkins,
        skinNodesRatio
    );
    const auto downSecondsSkin = std::make_shared<CenteredNode>(
        downSkins,
        skinNodesRatio
    );
    const auto downThirdSkin = std::make_shared<RightNode>(
        downSkins,
        skinNodesRatio
    );

    const auto createBallSkin = [](const Node_sptr &coveringNode) -> std::vector<Node_sptr> {
        const auto baseNode = std::make_shared<CenteredNode>(coveringNode, 1.f);
        const auto scaledBaseNode = std::make_shared<ScaledNode>(baseNode, 2.f / 3.f);

        const auto selectNode = std::make_shared<DownNode>(scaledBaseNode, 4.f);
        const auto priceNode = std::make_shared<CenteredNode>(selectNode, 3.f);
        const auto coinNodeBase = std::make_shared<RightNode>(selectNode, 1.f);
        const auto scaledCoinNode = std::make_shared<DownNode>(
            coinNodeBase,
            1.4f
        );
        const auto coinNode = std::make_shared<CenteredNode>(
            scaledCoinNode,
            1.f
        );

        const auto ballBaseNode = std::make_shared<UpNode>(scaledBaseNode, 4.f / 3.f);
        const auto ballNode = std::make_shared<CenteredNode>(ballBaseNode, 1.f);

        return {
            ballNode,
            priceNode,
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

    constexpr auto footNodeDefaultRatio = 6.f;
    const auto footNode = std::make_shared<DownNode>(
        mainStoreNode,
        footNodeDefaultRatio
    );
    constexpr auto coinsNodeRatio = 2.5f;
    constexpr auto coinNodesRatio = 1.f;
    const auto coinsNode = std::make_shared<HorizontalNode>(footNode, coinsNodeRatio, 0.03f);
    const auto footCoinNode = std::make_shared<DownNode>(coinsNode, 3.5f);
    const auto littleCoinNode = std::make_shared<CenteredNode>(footCoinNode, 1.f);
    const auto sumDigitOne = std::make_shared<HorizontalNode>(footNode, coinNodesRatio, 0.83f);
    const auto sumDigitTwo = std::make_shared<HorizontalNode>(footNode, coinNodesRatio, 0.68f);
    const auto sumDigitThree = std::make_shared<HorizontalNode>(footNode, coinNodesRatio, 0.53f);
    const auto sumDigitFour = std::make_shared<HorizontalNode>(footNode, coinNodesRatio, 0.38f);

    return std::accumulate(
        coveringNodes.begin(),
        coveringNodes.end(),
        std::vector<Node_sptr>{
            storeNode,
            sumDigitOne,
            sumDigitTwo,
            sumDigitThree,
            sumDigitFour,
            littleCoinNode,
            arrowNode
        },
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
    constexpr size_t offset = 7;
    std::array<StorePage::BallSkin, StorePage::numberOfSkins> ballSkins;
    short id = 1000;
    for (size_t ballSkinCount = 0; ballSkinCount < StorePage::numberOfSkins; ++ballSkinCount) {
        auto i = offset + ballSkinCount * StorePage::ballSkinNumberOfFields;
        ballSkins.at(ballSkinCount) = {
            std::make_shared<Label>(nodes.at(i++), JBTypes::Color::Yellow, id + 2),
            nodes.at(i++),
            nodes.at(i++),
            std::make_shared<Label>(nodes.at(i++), JBTypes::Color::Yellow, id + 3),
            std::make_shared<Label>(nodes.at(i++), JBTypes::Color::White, backgroundId + ballSkinCount)
        };
        id += 100;
    }
    return ballSkins;
}

ArrowLabel_sptr StorePage::createStoreArrowLabel(const Node_sptr &headerNode) {
    return createArrowLabel(headerNode, 4000, false, 1.1f);
}

void StorePage::setValidationPages(const std::array<Page_sptr, StorePage::numberOfSkins> &validationPages) {
    for(size_t i = 0; i < validationPages.size(); ++i) {
        _validationPages[i] = validationPages[i];
    }
}

const short StorePage::backgroundId = 2000;
const short StorePage::coinMoneyId = 400;

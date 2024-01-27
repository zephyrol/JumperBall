//
// Created by S.Morgenthaler on 20/09/2022.
//

#include "InGamePage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/LeftNode.h"
#include "gameMenu/nodes/RightNode.h"
#include "gameMenu/nodes/DownNode.h"
#include "gameMenu/nodes/HorizontalNode.h"

InGamePage::InGamePage(
    Player_sptr &&player,
    ArrowLabel_sptr arrowLabel,
    Node_sptr &&leftDigitNode,
    Node_sptr &&middleDigitNode,
    Node_sptr &&rightDigitNode,
    Node_sptr &&coinsTensDigit,
    Node_sptr &&coinsUnitsDigit,
    Label_sptr key1,
    Label_sptr key2,
    Label_sptr key3,
    Label_sptr key4,
    Label_sptr coinSymbol,
    Label_sptr leftArrow,
    Label_sptr rightArrow,
    Label_sptr upArrow,
    Label_sptr downArrow,
    Label_sptr jumpArrow,
    const Page_sptr &parent,
    CstItemsContainer_sptr itemsContainer
) : Page(std::move(player)),
    _parent(parent),
    _arrowLabel(std::move(arrowLabel)),
    _leftDigitNode(std::move(leftDigitNode)),
    _middleDigitNode(std::move(middleDigitNode)),
    _rightDigitNode(std::move(rightDigitNode)),
    _coinsTensDigit(std::move(coinsTensDigit)),
    _coinsUnitsDigit(std::move(coinsUnitsDigit)),
    _key1(std::move(key1)),
    _key2(std::move(key2)),
    _key3(std::move(key3)),
    _key4(std::move(key4)),
    _coinSymbol(std::move(coinSymbol)),
    _leftArrow(std::move(leftArrow)),
    _rightArrow(std::move(rightArrow)),
    _upArrow(std::move(upArrow)),
    _downArrow(std::move(downArrow)),
    _jumpArrow(std::move(jumpArrow)),
    _nodesToTestIntersection(
        [this]() -> decltype(_nodesToTestIntersection) {
            return {
                _arrowLabel->getNode(),
                _leftArrow->getNode(),
                _rightArrow->getNode(),
                _upArrow->getNode(),
                _downArrow->getNode(),
                _jumpArrow->getNode()
            };
        }()
    ),
    _itemsContainer(std::move(itemsContainer)) {
}

InGamePage_sptr InGamePage::createInstance(
    Player_sptr player,
    const Page_sptr &parent,
    float ratio,
    CstItemsContainer_sptr itemsContainer
) {
    auto nodes = createNodes(ratio);
    auto arrowLabel = createInGameArrowLabel(nodes[0]);
    auto inGamePage = std::make_shared<InGamePage>(
        std::move(player),
        arrowLabel,
        std::move(nodes[1]),
        std::move(nodes[2]),
        std::move(nodes[3]),
        std::move(nodes[4]),
        std::move(nodes[5]),
        std::make_shared<Label>(std::move(nodes[6]), JBTypes::Color::Yellow, key1LabelId),
        std::make_shared<Label>(std::move(nodes[7]), JBTypes::Color::Yellow, key2LabelId),
        std::make_shared<Label>(std::move(nodes[8]), JBTypes::Color::Yellow, key3LabelId),
        std::make_shared<Label>(std::move(nodes[9]), JBTypes::Color::Yellow, key4LabelId),
        std::make_shared<Label>(std::move(nodes[10]), JBTypes::Color::Yellow, coinSymbolLabelId),
        std::make_shared<Label>(std::move(nodes[11]), JBTypes::Color::Yellow, leftArrowLabelId),
        std::make_shared<Label>(std::move(nodes[12]), JBTypes::Color::Yellow, rightArrowLabelId),
        std::make_shared<Label>(std::move(nodes[13]), JBTypes::Color::Yellow, upArrowLabelId),
        std::make_shared<Label>(std::move(nodes[14]), JBTypes::Color::Yellow, downArrowLabelId),
        std::make_shared<Label>(std::move(nodes[15]), JBTypes::Color::Yellow, jumpArrowLabelId),
        parent,
        std::move(itemsContainer)
    );
    return inGamePage;
}

Page_wptr InGamePage::parent() {
    return _parent;
}

Page_sptr InGamePage::click(float mouseX, float mouseY) {
    if (_arrowLabel->getNode()->intersect(mouseX, mouseY, 2.f)) {
        _player->setAsInMenu();
        return _parent.lock();
    }
    return nullptr;
}

void InGamePage::resize(float ratio) {
    const auto nodes = createNodes(ratio);
    _arrowLabel = createInGameArrowLabel(nodes[0]);
    _leftDigitNode = nodes[1];
    _middleDigitNode = nodes[2];
    _rightDigitNode = nodes[3];
    _coinsTensDigit = nodes[4];
    _coinsUnitsDigit = nodes[5];
    _key1 = std::make_shared<Label>(nodes[6], JBTypes::Color::Yellow, key1LabelId);
    _key2 = std::make_shared<Label>(nodes[7], JBTypes::Color::Yellow, key2LabelId);
    _key3 = std::make_shared<Label>(nodes[8], JBTypes::Color::Yellow, key3LabelId);
    _key4 = std::make_shared<Label>(nodes[9], JBTypes::Color::Yellow, key4LabelId);
    _coinSymbol = std::make_shared<Label>(nodes[10], JBTypes::Color::Yellow, coinSymbolLabelId);
    _leftArrow = std::make_shared<Label>(nodes[11], JBTypes::Color::Yellow, leftArrowLabelId);
    _rightArrow = std::make_shared<Label>(nodes[12], JBTypes::Color::Yellow, rightArrowLabelId);
    _upArrow = std::make_shared<Label>(nodes[13], JBTypes::Color::Yellow, upArrowLabelId);
    _downArrow = std::make_shared<Label>(nodes[14], JBTypes::Color::Yellow, downArrowLabelId);
    _jumpArrow = std::make_shared<Label>(nodes[15], JBTypes::Color::Yellow, jumpArrowLabelId);
}

vecCstTextNode_uptr InGamePage::genTextNodes() const {
    decltype(genTextNodes()) textNodes;

    short nodeCount = 0;
    for (const auto &node: std::vector<Node_sptr>{
        _leftDigitNode,
        _middleDigitNode,
        _rightDigitNode,
        _coinsTensDigit,
        _coinsUnitsDigit
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

vecNode_sptr InGamePage::createNodes(float ratio) {

    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);

    constexpr auto notchNodeDefaultRatio = 6.f;
    const auto notchNode = std::make_shared<UpNode>(
        resizedScreenNode,
        std::max(notchNodeDefaultRatio, notchNodeDefaultRatio * ratio)
    );

    constexpr auto sideNodesRatio = 1.f;
    const auto arrowNode = std::make_shared<LeftNode>(notchNode, sideNodesRatio);

    const auto timerNode = std::make_shared<RightNode>(notchNode, sideNodesRatio);
    const auto digitsNode = std::make_shared<UpNode>(timerNode, 2.f);
    constexpr auto digitNodesRatio = 1.f;
    const auto leftDigitNode = std::make_shared<LeftNode>(digitsNode, digitNodesRatio);
    const auto middleDigitNode = std::make_shared<CenteredNode>(digitsNode, digitNodesRatio);
    const auto rightDigitNode = std::make_shared<RightNode>(digitsNode, digitNodesRatio);

    constexpr auto footNodeDefaultRatio = 2.5f;
    const auto footNodeRatio = footNodeDefaultRatio * ratio;
    const auto footNode = std::make_shared<DownNode>(
        resizedScreenNode,
        std::max(footNodeDefaultRatio, footNodeRatio)
    );

    constexpr auto keysCoinNodeRatioScalar = 4.f;
    const auto keysCoinNodeRatio = footNodeRatio * keysCoinNodeRatioScalar;
    const auto keysCoinNode = std::make_shared<DownNode>(
        footNode,
        keysCoinNodeRatio
    );

    constexpr auto keysNodeRatio = 3.f;
    const auto keysNode = std::make_shared<RightNode>(keysCoinNode, keysNodeRatio);

    constexpr auto keyNodesRatio = 1.f;
    const auto rightKeyNode = std::make_shared<HorizontalNode>(keysNode, keyNodesRatio, 1.f);
    const auto middleKeyNode = std::make_shared<HorizontalNode>(keysNode, keyNodesRatio, 0.75f);
    const auto leftKeyNode = std::make_shared<HorizontalNode>(keysNode, keyNodesRatio, 0.5f);
    const auto lastKeyNode = std::make_shared<HorizontalNode>(keysNode, keyNodesRatio, 0.25f);

    constexpr auto coinsNodeRatio = 2.5f;
    constexpr auto coinNodesRatio = 1.f;
    const auto coinsNode = std::make_shared<LeftNode>(keysCoinNode, coinsNodeRatio);
    const auto coinSymbolNode = std::make_shared<LeftNode>(coinsNode, coinNodesRatio);
    const auto footCoinNode = std::make_shared<DownNode>(coinSymbolNode, 1.2f);
    const auto littleCoinNode = std::make_shared<CenteredNode>(footCoinNode, 1.f);
    const auto coinsDigitNode = std::make_shared<RightNode>(coinsNode, coinsNodeRatio - coinNodesRatio);
    const auto coinsTensDigit = std::make_shared<LeftNode>(coinsDigitNode, coinNodesRatio);
    const auto coinsUnitsDigit = std::make_shared<RightNode>(coinsDigitNode, coinNodesRatio);

    constexpr auto arrowsNodeHeight = keysCoinNodeRatioScalar - 1.f;
    constexpr auto arrowsNodeScalar = keysCoinNodeRatioScalar / arrowsNodeHeight;
    const auto arrowsNodeRatio = footNodeRatio * arrowsNodeScalar;
    const auto arrowsNode = std::make_shared<UpNode>(
        footNode,
        arrowsNodeRatio
    );

    return {
        arrowNode,
        leftDigitNode,
        middleDigitNode,
        rightDigitNode,
        coinsTensDigit,
        coinsUnitsDigit,
        rightKeyNode,
        middleKeyNode,
        leftKeyNode,
        lastKeyNode,
        littleCoinNode
    };
}

std::vector<std::string> InGamePage::shaderDefines() const {
    return {"TEST_KEY", "TEST_COIN"};
}

std::string InGamePage::getVertexShaderName() const {
    return "inGamePageVs.vs";
}

Displayable::DynamicNames InGamePage::getDynamicFloatNames() const {
    return {"missingTimeWarning"};
}

Displayable::DynamicValues<float> InGamePage::getDynamicFloatValues() const {
    const auto remainingTime = _player->getRemainingTime();
    if (remainingTime > 9.f) {
        return {0.f};
    }
    constexpr auto periodFactor = 15.f;
    return {1.f - (cosf((remainingTime - 10.f) * periodFactor) * 0.5f + 0.5f)};
}

Displayable::DynamicNames InGamePage::getDynamicIntNames() const {
    decltype(getDynamicIntNames()) dynamicNames{
        "leftDigit",
        "middleDigit",
        "rightDigit",
        "coinsTensDigit",
        "coinsUnitsDigit",
        "currentNumberOfKeys",
        "maxNumberOfKeys"
    };
    auto pageDynamicNames = Page::getDynamicIntNames();
    dynamicNames.insert(
        dynamicNames.end(),
        std::make_move_iterator(pageDynamicNames.begin()),
        std::make_move_iterator(pageDynamicNames.end())
    );
    return dynamicNames;
}

Displayable::DynamicValues<int> InGamePage::getDynamicIntValues() const {

    const auto remainingTime = static_cast<int> (ceilf(_player->getRemainingTime()));
    const auto leftDigit = remainingTime / 100;
    const auto middleDigit = (remainingTime % 100) / 10;
    const auto rightDigit = remainingTime % 10;

    const auto numberOfCoins = static_cast<int>(_itemsContainer->getCurrentNumberOfCoins());
    const auto coinsTensDigit = numberOfCoins / 10;
    const auto coinsUnitsDigit = numberOfCoins % 10;

    constexpr auto middleDigitIdOffset = 10;
    constexpr auto rightDigitIdOffset = middleDigitIdOffset + 10;
    constexpr auto coinsTensDigitIdOffset = rightDigitIdOffset + 10;
    constexpr auto coinsUnitsDigitIdOffset = coinsTensDigitIdOffset + 10;

    decltype(getDynamicIntValues()) dynamicInts{
        leftDigit,
        middleDigit + middleDigitIdOffset,
        rightDigit + rightDigitIdOffset,
        coinsTensDigit + coinsTensDigitIdOffset,
        coinsUnitsDigit + coinsUnitsDigitIdOffset,
        static_cast<int>(_itemsContainer->getCurrentNumberOfKeys()),
        static_cast<int>(_itemsContainer->getMaxNumberOfKeys()),
    };

    auto pageDynamicInts = Page::getDynamicIntValues();
    dynamicInts.insert(
        dynamicInts.end(),
        std::make_move_iterator(pageDynamicInts.begin()),
        std::make_move_iterator(pageDynamicInts.end())
    );
    return dynamicInts;
}

vecCstLabel_sptr InGamePage::labels() const {
    return {_arrowLabel, _key1, _key2, _key3, _key4, _coinSymbol};
}

void InGamePage::setItemsContainer(CstItemsContainer_sptr itemsContainer) {
    _itemsContainer = std::move(itemsContainer);
}

void InGamePage::update(const Mouse &mouse) {
    if (!mouse.isPressed()) {
        _currentSelectedLabel = 0;
        return;
    }

    // Positions have to be centered
    const auto mouseX = mouse.currentXCoord() - 0.5f;
    const auto mouseY = mouse.currentYCoord() - 0.5f;

    if (_arrowLabel->getNode()->intersect(mouseX, mouseY, 2.f)) {
        _currentSelectedLabel = InGamePage::arrowLabelId;
    }
}

std::shared_ptr<ArrowLabel> InGamePage::createInGameArrowLabel(const Node_sptr &headerNode) {
    return createArrowLabel(headerNode, InGamePage::arrowLabelId, true, 1.5f);
}

const short InGamePage::arrowLabelId = -1;
const short InGamePage::key1LabelId = 501;
const short InGamePage::key2LabelId = 502;
const short InGamePage::key3LabelId = 503;
const short InGamePage::key4LabelId = 504;
const short InGamePage::coinSymbolLabelId = 400;
const short InGamePage::leftArrowLabelId = 401;
const short InGamePage::rightArrowLabelId = 402;
const short InGamePage::upArrowLabelId = 403;
const short InGamePage::downArrowLabelId = 404;
const short InGamePage::jumpArrowLabelId = 405;

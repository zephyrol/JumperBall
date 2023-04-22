//
// Created by seb on 11/09/22.
//

#include "LevelsPage.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/VerticalNode.h"
#include "gameMenu/nodes/DownNode.h"
#include "gameMenu/nodes/LeftNode.h"
#include "gameMenu/nodes/RightNode.h"

LevelsPage::LevelsPage(
    Player_sptr &&player,
    Node_sptr &&levelsTitle,
    std::vector<Node_sptr> &&levels,
    Label_sptr &&backgroundLabel,
    std::shared_ptr<ArrowLabel> arrowLabel,
    const Page_sptr &parent
) : ScrollablePage(
    std::move(player),
    3.f
),
    _parent(parent),
    _levelsTitle(std::move(levelsTitle)),
    _heightThreshold(computeHeightThreshold()),
    _levels(std::move(levels)),
    _backgroundLabel(std::move(backgroundLabel)),
    _arrowLabel(std::move(arrowLabel)),
    _inGamePage(nullptr) {
}

LevelsPage_sptr LevelsPage::createInstance(
    Player_sptr player,
    const Page_sptr &parent,
    float ratio
) {
    const auto levelsPageNode = getCommonNode(ratio);
    auto levelsNode = createLevelsNodes(levelsPageNode);
    const auto headerNode = createHeaderNode(levelsPageNode);
    auto levelsTitleNode = createLevelsTitleNode(headerNode);
    auto arrowLabel = createLevelsArrowLabel(createArrowNode(headerNode));

    return std::make_shared<LevelsPage>(
        std::move(player),
        std::move(levelsTitleNode),
        std::move(levelsNode),
        createBackgroundLabel(std::move(levelsPageNode)),
        arrowLabel,
        parent
    );
}

Node_sptr LevelsPage::createHeaderNode(const Node_sptr &commonNode) {
    return std::make_shared<UpNode>(commonNode, 4.f);
}

vecNode_sptr LevelsPage::createLevelsNodes(const Node_sptr &commonNode) {
    vecNode_sptr levelsNodes;
    const auto levelsMainNode = std::make_shared<DownNode>(commonNode, 9.f / 18.f);
    const auto numberOfLevelsFloat = static_cast<float>(LevelsPage::numberOfLevels);
    constexpr auto positionFactor = 4.f;
    for (size_t i = 0; i < LevelsPage::numberOfLevels; i += 3) {
        const auto positionY = static_cast<float>(i) / numberOfLevelsFloat;
        const auto verticalNode = std::make_shared<VerticalNode>(
            levelsMainNode,
            5.f,
            1.f - positionY * positionFactor
        );

        auto leftLevel = LevelsPage::createLevelNode<LeftNode>(verticalNode);
        levelsNodes.push_back(std::move(leftLevel));

        auto middleLevel = LevelsPage::createLevelNode<CenteredNode>(verticalNode);
        levelsNodes.push_back(std::move(middleLevel));

        auto rightLevel = LevelsPage::createLevelNode<RightNode>(verticalNode);
        levelsNodes.push_back(std::move(rightLevel));
    }

    return levelsNodes;
}

void LevelsPage::resize(float ratio) {
    const auto levelsPageNode = getCommonNode(ratio);
    auto levelsNode = createLevelsNodes(levelsPageNode);
    const auto headerNode = createHeaderNode(levelsPageNode);
    auto levelsTitleNode = createLevelsTitleNode(headerNode);
    auto arrowLabel = createLevelsArrowLabel(
        createArrowNode(headerNode)
    );
    _levelsTitle = std::move(levelsTitleNode);
    _heightThreshold = computeHeightThreshold();
    _levels = std::move(levelsNode);
    _arrowLabel = std::move(arrowLabel);
    _backgroundLabel = createBackgroundLabel(levelsPageNode);
}

Node_sptr LevelsPage::getCommonNode(float ratio) {
    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto levelsPageNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 22.f
    );
    return levelsPageNode;
}


Page_sptr LevelsPage::click(float mouseX, float mouseY) {

    const auto intersectTest = [this, &mouseX, &mouseY](const Node_sptr &node) {
        return mouseY < _heightThreshold && node->intersect(mouseX, mouseY - getOffsetY());
    };

    for (size_t i = 0; i < LevelsPage::numberOfLevels; ++i) {
        if (intersectTest(_levels[i])) {
            const auto levelNumber = i + 1;
            if (levelNumber > _player->levelProgression()) {
                return nullptr;
            }
            _player->setCurrentLevel(levelNumber);
            return _inGamePage;
        }
    }
    if (_arrowLabel->getNode()->intersect(mouseX, mouseY)) {
        return _parent.lock();
    }
    return nullptr;
}


void LevelsPage::update(const Mouse &mouse) {
    ScrollablePage::update(mouse);

    if (!mouse.isPressed()) {
        _currentSelectedLabel = 0;
        return;
    }

    // Positions have to be centered
    const auto mouseX = mouse.currentXCoord() - 0.5f;
    const auto mouseY = mouse.currentYCoord() - 0.5f;

    const auto intersectTest = [this, &mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY - getOffsetY());
    };

    for (size_t i = 0; i < LevelsPage::numberOfLevels; ++i) {
        if (intersectTest(_levels[i])) {
            const auto levelNumber = i + 1;
            if (levelNumber <= _player->levelProgression()) {
                _currentSelectedLabel = static_cast<int>(levelNumber);
            }
        }
    }
    if (_arrowLabel->getNode()->intersect(mouseX, mouseY)) {
        _currentSelectedLabel = LevelsPage::arrowLabelId;
    }
}

vecCstLabel_sptr LevelsPage::labels() const {
    return {_arrowLabel, _backgroundLabel};
}

Node_sptr LevelsPage::createLevelsTitleNode(const Node_sptr &headerNode) {
    return std::make_shared<RightNode>(headerNode, 2.5f);
}

Node_sptr LevelsPage::createArrowNode(const Node_sptr &headerNode) {
    return std::make_shared<DownNode>(headerNode, 4.f);
}

Page_wptr LevelsPage::parent() {
    return _parent;
}

void LevelsPage::setInGamePage(Page_sptr inGamePage) {
    _inGamePage = std::move(inGamePage);
}

vecCstTextNode_uptr LevelsPage::genTextNodes() const {
    decltype(genTextNodes()) textNodes;

    for (size_t i = 0; i < LevelsPage::numberOfLevels; ++i) {
        const auto levelNumber = static_cast<int> (i) + 1;
        textNodes.push_back(CstTextNode_uptr(new TextNode(
            _levels[i],
            (levelNumber < 10 ? "0" : "") + std::to_string(levelNumber),
            static_cast<short>(levelNumber)
        )));
    }

    constexpr int levelsTitleLabelId = -2;
    textNodes.push_back(
        CstTextNode_uptr(new TextNode(
            _levelsTitle,
            _player->isUsingEnglishLanguage() ? "Levels" : "Niveaux",
            levelsTitleLabelId)
        )
    );
    return textNodes;
}

std::string LevelsPage::getVertexShaderName() const {
    return "levelsPageVs.vs";
}

std::vector<std::string> LevelsPage::shaderDefines() const {
    return {"TEST_ALPHA_TEXTURE", "DISCARDING", "TRANSPARENT_BACKGROUND"};
}

Displayable::DynamicNames LevelsPage::getDynamicIntNames() const {
    auto dynamicIntNames = Page::getDynamicIntNames();
    dynamicIntNames.emplace_back("levelProgression");
    return dynamicIntNames;
}

Displayable::DynamicValues<int> LevelsPage::getDynamicIntValues() const {
    auto dynamicIntValues = Page::getDynamicIntValues();
    dynamicIntValues.emplace_back(static_cast<int>(_player->levelProgression()));
    return dynamicIntValues;
}

const int LevelsPage::arrowLabelId = -1;

std::vector<std::pair<std::string, float>> LevelsPage::getVertexShaderConstants() const {
    return {{
                "heightThreshold",
                // Convert to OpenGL format
                _heightThreshold * 2.f
            }};
}

float LevelsPage::computeHeightThreshold() const {
    return (_levelsTitle->positionY() - _levelsTitle->height() * 0.5f);
}

std::shared_ptr<ArrowLabel> LevelsPage::createLevelsArrowLabel(const Node_sptr &headerNode) {
    return createArrowLabel(headerNode, LevelsPage::arrowLabelId, false, 1.1f);
}

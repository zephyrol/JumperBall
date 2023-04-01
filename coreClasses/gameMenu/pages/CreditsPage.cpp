//
// Created by S.Morgenthaler on 25/03/2023.
//

#include "CreditsPage.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/DownNode.h"
#include "gameMenu/nodes/VerticalNode.h"

CreditsPage::CreditsPage(
    Player_sptr &&player,
    Node_sptr &&credits,
    Node_sptr &&developmentAndDesign,
    Node_sptr &&mainAuthor,
    Node_sptr &&musicAndSoundsEffects,
    Node_sptr &&secondAuthor,
    Node_sptr &&exitNode,
    const Page_sptr &parent
) : Page(
    std::move(player)
),
    _parent(parent),
    _credits(std::move(credits)),
    _developmentAndDesign(std::move(developmentAndDesign)),
    _mainAuthor(std::move(mainAuthor)),
    _musicAndSoundsEffects(std::move(musicAndSoundsEffects)),
    _secondAuthor(std::move(secondAuthor)),
    _exitNode(std::move(exitNode)) {
}

void CreditsPage::resize(float ratio) {
    const auto &nodes = createNodes(ratio, _player->isUsingEnglishLanguage());
    _credits = nodes.at(0);
    _developmentAndDesign = nodes.at(1);
    _mainAuthor = nodes.at(2);
    _musicAndSoundsEffects = nodes.at(3);
    _secondAuthor = nodes.at(4);
    _exitNode = nodes.at(5);
}

CreditsPage_sptr CreditsPage::createInstance(
    Player_sptr player,
    const Page_sptr &parent,
    float ratio
) {
    auto nodes = createNodes(ratio, player->isUsingEnglishLanguage());
    return std::make_shared<CreditsPage>(
        std::move(player),
        std::move(nodes.at(0)),
        std::move(nodes.at(1)),
        std::move(nodes.at(2)),
        std::move(nodes.at(3)),
        std::move(nodes.at(4)),
        std::move(nodes.at(5)),
        parent
    );
}

vecNode_sptr CreditsPage::createNodes(float ratio, bool english) {

    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto mainCreditsNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 16.f
    );

    const auto jumperBallCredits = std::make_shared<UpNode>(
        mainCreditsNode,
        4.f
    );

    const auto authorNode = std::make_shared<DownNode>(
        mainCreditsNode,
        10.f
    );

    const auto optionsParentNode = std::make_shared<CenteredNode>(
        mainCreditsNode,
        1.5f
    );

    constexpr float optionsNodeRatio = 7.f;
    const auto playNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        english ? 1.05f : 1.1f // Because p creates an offset
    );

    const auto storeNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        english ? 0.6f : 0.55f // Because q creates an offset
    );

    const auto languageNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        0.03f // Because g letter creates an offset
    );

    const auto exitNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        english ? -0.4f : -0.45f // Because Q creates an offset
    );

    return {jumperBallCredits, playNode, storeNode, languageNode, exitNode, authorNode};
}

Page_sptr CreditsPage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_exitNode)) {
        return _parent.lock();
    }
    return nullptr;
}

vecCstTextNode_uptr CreditsPage::genTextNodes() const {
    vecCstTextNode_uptr textNodes;
    const auto english = _player->isUsingEnglishLanguage();
    textNodes.emplace_back(new TextNode(_credits, english ? "Credits" : "Cr;dits", 0));
    textNodes.emplace_back(new TextNode(
        _developmentAndDesign,
        english ? "Development, Design:" : "Developpement, Design:",
       developmentAndDesignLabelId
    ));
    textNodes.emplace_back(new TextNode(_mainAuthor, "S;bastien Morgenthaler", mainAuthorLabelId));
    textNodes.emplace_back(
        new TextNode(
            _musicAndSoundsEffects,
            english ? "Music and sound effects:" : "Musiques et sons:", musicAndSoundsEffectsLabelId)
    );
    textNodes.emplace_back(new TextNode( _secondAuthor, "Julien Goettelmann", secondAuthorLabelId));
    textNodes.emplace_back(new TextNode(_exitNode, english ? "Exit" : "Quitter", exitNodeLabelId));
    return textNodes;
}

std::string CreditsPage::getVertexShaderName() const {
    return "titlePageVs.vs";
}

std::vector<std::string> CreditsPage::shaderDefines() const {
    return {"ALWAYS_ALPHA_TEXTURE"};
}

void CreditsPage::update(const Mouse &mouse) {
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
    if (intersectTest(_developmentAndDesign)) {
        _currentSelectedLabel = developmentAndDesignLabelId;
    } else if (intersectTest(_mainAuthor)) {
        _currentSelectedLabel = mainAuthorLabelId;
    } else if (intersectTest(_musicAndSoundsEffects)) {
        _currentSelectedLabel = musicAndSoundsEffectsLabelId;
    } else if (intersectTest(_secondAuthor)) {
        _currentSelectedLabel = secondAuthorLabelId;
    } else if (intersectTest(_exitNode)) {
        _currentSelectedLabel = exitNodeLabelId;
    } else {
        _currentSelectedLabel = -1;
    }
}

Page_wptr CreditsPage::parent() {
    return _parent;
}


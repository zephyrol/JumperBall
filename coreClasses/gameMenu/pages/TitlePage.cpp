//
// Created by seb on 15/08/22.
//

#include "TitlePage.h"
#include "gameMenu/nodes/CenteredNode.h"
#include "gameMenu/nodes/UpNode.h"
#include "gameMenu/nodes/ScreenNode.h"
#include "gameMenu/nodes/ScaledNode.h"
#include "gameMenu/nodes/DownNode.h"
#include "gameMenu/nodes/VerticalNode.h"

TitlePage::TitlePage(
    Player_sptr &&player,
    Node_sptr &&jumperBallTitle,
    Node_sptr &&play,
    Node_sptr &&store,
    Node_sptr &&language,
    Node_sptr &&musics,
    Node_sptr &&sounds,
    Node_sptr &&credits,
    Node_sptr &&exitNode,
    Node_sptr &&author,
    Label_sptr &&backgroundLabel,
    float currentRatio
) : Page(
    std::move(player)
),
    _jumperBallTitle(std::move(jumperBallTitle)),
    _play(std::move(play)),
    _store(std::move(store)),
    _language(std::move(language)),
    _musics(std::move(musics)),
    _sounds(std::move(sounds)),
    _credits(std::move(credits)),
    _exitNode(std::move(exitNode)),
    _author(std::move(author)),
    _backgroundLabel(std::move(backgroundLabel)),
    _levelsPage(nullptr),
    _creditsPage(nullptr),
    _storePage(nullptr),
    _currentRatio(currentRatio) {
}

void TitlePage::resize(float ratio) {
    _currentRatio = ratio;
    resetNodes();
}

void TitlePage::resetNodes() {
    const auto &nodes = createNodes(_currentRatio, _player->isUsingEnglishLanguage());
    _jumperBallTitle = nodes.at(0);
    _play = nodes.at(1);
    _store = nodes.at(2);
    _language = nodes.at(3);
    _musics = nodes.at(4);
    _sounds = nodes.at(5);
    _credits = nodes.at(6);
    _exitNode = nodes.at(7);
    _author = nodes.at(8);
    _backgroundLabel = createBackgroundLabel(nodes.at(9));
}


TitlePage_sptr TitlePage::createInstance(Player_sptr player, float ratio) {
    auto nodes = createNodes(ratio, player->isUsingEnglishLanguage());
    return std::make_shared<TitlePage>(
        std::move(player),
        std::move(nodes.at(0)),
        std::move(nodes.at(1)),
        std::move(nodes.at(2)),
        std::move(nodes.at(3)),
        std::move(nodes.at(4)),
        std::move(nodes.at(5)),
        std::move(nodes.at(6)),
        std::move(nodes.at(7)),
        std::move(nodes.at(8)),
        createBackgroundLabel(std::move(nodes.at(9))),
        ratio
    );
}

vecNode_sptr TitlePage::createNodes(float ratio, bool english) {

    const auto screenNode = std::make_shared<ScreenNode>(ratio);
    const auto resizedScreenNode = std::make_shared<ScaledNode>(screenNode, 0.95f);
    const auto mainTitleNode = std::make_shared<CenteredNode>(
        resizedScreenNode,
        9.f / 16.f
    );

    const auto jumperBallTitle = std::make_shared<UpNode>(
        mainTitleNode,
        4.f
    );

    const auto authorNode = std::make_shared<DownNode>(
        mainTitleNode,
        10.f
    );

    const auto optionsParentNode = std::make_shared<CenteredNode>(
        mainTitleNode,
        1.5f
    );

    constexpr float optionsNodeRatio = 10.f;
    const auto playNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        english ? 1.36f : 1.4f // Because p creates an offset
    );

    const auto storeNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        english ? 1.06f : 1.02f // Because q creates an offset
    );

    const auto languageNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        0.68f // Because g letter creates an offset
    );

    const auto musicNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        english ? 0.38f : 0.34f // Because q creates an offset
    );

    const auto soundNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        0.05f // Because q creates an offset
    );

    const auto creditsNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        -0.28f
    );

    const auto exitNode = std::make_shared<VerticalNode>(
        optionsParentNode,
        optionsNodeRatio,
        english ? -0.6f : -0.65f // Because Q creates an offset
    );

    return {
        jumperBallTitle,
        playNode,
        storeNode,
        languageNode,
        musicNode,
        soundNode,
        creditsNode,
        exitNode,
        authorNode,
        mainTitleNode
    };
}

Page_sptr TitlePage::click(float mouseX, float mouseY) {
    const auto intersectTest = [&mouseX, &mouseY](const Node_sptr &node) {
        return node->intersect(mouseX, mouseY);
    };
    if (intersectTest(_play)) {
        _player->addValidationSound();
        return _levelsPage;
    }
    if (intersectTest(_language)) {
        _player->switchLangage();
        resetNodes();
    }
    if (intersectTest(_musics)) {
        _player->switchMusicsStatus();
        resetNodes();
    }
    if (intersectTest(_sounds)) {
        _player->switchSoundsStatus();
        resetNodes();
    }
    if (intersectTest(_exitNode)) {
        _player->requestQuit();
    }
    if (intersectTest(_credits)) {
        return _creditsPage;
    }
    if (intersectTest(_store)) {
        return _storePage;
    }
    return nullptr;
}

void TitlePage::setLevelsPage(Page_sptr levelsPage) {
    _levelsPage = std::move(levelsPage);
}

void TitlePage::setCreditsPage(Page_sptr creditsPage) {
    _creditsPage = std::move(creditsPage);
}

void TitlePage::setStorePage(Page_sptr storePage) {
    _storePage = std::move(storePage);
}

vecCstTextNode_uptr TitlePage::genTextNodes() const {
    vecCstTextNode_uptr textNodes;
    const auto english = _player->isUsingEnglishLanguage();
    textNodes.emplace_back(new TextNode(_jumperBallTitle, "Jumper Ball", 0));
    textNodes.emplace_back(new TextNode(_play, english ? "Play" : "Jouer", playLabelId));
    textNodes.emplace_back(new TextNode(_store, english ? "Store" : "Boutique", storeLabelId));
    textNodes.emplace_back(
        new TextNode(_language, english ? "Change language" : "Changer de langue", languageLabelId)
    );
    const std::string musicsStatus = _player->areMusicsActivated() ? "ON" : "OFF";
    textNodes.emplace_back(
        new TextNode(_musics, (english ? "Musics: " : "Musiques: ") + musicsStatus, musicsLabelId)
    );
    const std::string soundsStatus = _player->areSoundsActivated() ? "ON" : "OFF";
    textNodes.emplace_back(
        new TextNode(_sounds, (english ? "Sounds: " : "Sons: ") + soundsStatus, soundsLabelId)
    );
    textNodes.emplace_back(new TextNode(_credits, english ? "Credits" : "Cr;dits", creditsLabelId));
    textNodes.emplace_back(new TextNode(_exitNode, english ? "Exit" : "Quitter", exitLabelId));
    textNodes.emplace_back(new TextNode(
        _author,
        std::string(english ? "Created by" : "Cr;; par") + std::string(" S.Morgenthaler"),
        0
    ));
    return textNodes;
}

std::string TitlePage::getVertexShaderName() const {
    return "titlePageVs.vs";
}

std::vector<std::string> TitlePage::shaderDefines() const {
    return {"TRANSPARENT_BACKGROUND"};
}

void TitlePage::update(const Mouse &mouse) {
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
    if (intersectTest(_play)) {
        _currentSelectedLabel = playLabelId;
    } else if (intersectTest(_exitNode)) {
        _currentSelectedLabel = exitLabelId;
    } else if (intersectTest(_store)) {
        _currentSelectedLabel = storeLabelId;
    } else if (intersectTest(_language)) {
        _currentSelectedLabel = languageLabelId;
    } else if (intersectTest(_musics)) {
        _currentSelectedLabel = musicsLabelId;
    } else if (intersectTest(_sounds)) {
        _currentSelectedLabel = soundsLabelId;
    } else if (intersectTest(_credits)) {
        _currentSelectedLabel = creditsLabelId;
    } else {
        _currentSelectedLabel = -1;
    }
}

vecCstLabel_sptr TitlePage::labels() const {
    return {_backgroundLabel};
}

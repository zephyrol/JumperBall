//
// Created by Morgenthaler Sébastien on 15/08/22.
//

#ifndef JUMPERBALLAPPLICATION_TITLEPAGE_H
#define JUMPERBALLAPPLICATION_TITLEPAGE_H

#include "Page.h"

class TitlePage;

using TitlePage_sptr = std::shared_ptr<TitlePage>;

class TitlePage : public Page {
public:

    static TitlePage_sptr createInstance(Player_sptr player, float ratio);

    void resize(float ratio) override;

    explicit TitlePage(
        Player_sptr &&player,
        Node_sptr &&jumperBallTitle,
        Node_sptr &&play,
        Node_sptr &&store,
        Node_sptr &&exitNode,
        Node_sptr &&author
    );

    void setLevelsPage(Page_sptr levelsPage);

    vecCstTextNode_uptr genTextNodes() const override;

    Page_sptr click(float mouseX, float mouseY) override;

    void update(const Mouse &mouse) override;

    std::string getVertexShaderName() const override;

    std::vector<std::string> shaderDefines() const override;

private:
    Node_sptr _jumperBallTitle;
    Node_sptr _play;
    Node_sptr _store;
    Node_sptr _exitNode;
    Node_sptr _author;

    Page_sptr _levelsPage;

    static constexpr int playLabelId = 1;
    static constexpr int storeLabelId = 2;
    static constexpr int exitLabelId = 3;

    static vecNode_sptr createNodes(float ratio);

};


#endif //JUMPERBALLAPPLICATION_TITLEPAGE_H

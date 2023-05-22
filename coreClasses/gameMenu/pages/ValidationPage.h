//
// Created by S.Morgenthaler on 22/05/2023.
//

#ifndef JUMPERBALLAPPLICATION_VALIDATIONPAGE_H
#define JUMPERBALLAPPLICATION_VALIDATIONPAGE_H

#include "Page.h"

class ValidationPage;

using ValidationPage_sptr = std::shared_ptr<ValidationPage>;

class ValidationPage : public Page {

public:

    ValidationPage(
        size_t skinNumber,
        Player_sptr &&player,
        Node_sptr &&buyNode,
        Node_sptr &&yesNode,
        Node_sptr &&noNode,
        Label_sptr &&backgroundLabel,
        const Page_sptr &parent
    );

    static ValidationPage_sptr createInstance(
        size_t skinNumber,
        Player_sptr player,
        const Page_sptr &parent,
        float ratio
    );

    Page_sptr click(float mouseX, float mouseY) override;

    void resize(float ratio) override;

    Page_wptr parent() override;

    vecCstTextNode_uptr genTextNodes() const override;

    std::vector<std::string> shaderDefines() const override;

    std::string getVertexShaderName() const override;

    void update(const Mouse &mouse) override;

    vecCstLabel_sptr labels() const override;

private:

    const size_t _skinNumber;
    const Page_wptr _parent;
    Node_sptr _buyNode;
    Node_sptr _yesNode;
    Node_sptr _noNode;
    Label_sptr _backgroundLabel;

    static constexpr int yesLabelId = 1;
    static constexpr int noLabelId = 2;

    static vecNode_sptr createNodes(float ratio);
};

#endif // JUMPERBALLAPPLICATION_VALIDATIONPAGE_H

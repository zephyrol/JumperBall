//
// Created by Morgenthaler Sébastien on 15/08/22.
//

#ifndef JUMPERBALLAPPLICATION_TITLEPAGE_H
#define JUMPERBALLAPPLICATION_TITLEPAGE_H


#include "Page.h"

class TitlePage : public Page {
public:

    static Page_sptr createInstance(float ratio);

    void resize(float ratio) override;

    static vecNode_sptr createNodes(float ratio);

    TitlePage(
        const Node_sptr &jumperBallTitle,
        const Node_sptr &play,
        const Node_sptr &store,
        const Node_sptr &exit,
        const Node_sptr &author
    );

    NodeMessageAssociations nodeToMessage() const override;

private:
    Node_sptr _jumperBallTitle;
    Node_sptr _play;
    Node_sptr _store;
    Node_sptr _exit;
    Node_sptr _author;

};


#endif //JUMPERBALLAPPLICATION_TITLEPAGE_H

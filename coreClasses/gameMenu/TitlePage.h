//
// Created by Morgenthaler Sébastien on 15/08/22.
//

#ifndef JUMPERBALLAPPLICATION_TITLEPAGE_H
#define JUMPERBALLAPPLICATION_TITLEPAGE_H


#include "Page.h"

class TitlePage: public Page {
public:

    static Page_sptr createInstance(float ratio);

    void resize(float ratio) override;

    static vecNode_sptr createNodes(float ratio);

    TitlePage(
        const Node_sptr& jumperBallTitle
    );

    NodeMessageAssociations nodeToMessage() const override;

private:
    Node_sptr _jumperBallTitle;

};


#endif //JUMPERBALLAPPLICATION_TITLEPAGE_H

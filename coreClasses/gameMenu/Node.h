//
// Created by S.Morgenthaler on 13/07/22.
//

#ifndef JUMPERBALLAPPLICATION_NODE_H
#define JUMPERBALLAPPLICATION_NODE_H

#include "scene/SceneElement.h"

class Node;

using Node_sptr = std::shared_ptr<Node>;
using CstNode_sptr = std::shared_ptr<const Node>;
using vecCstNode_sptr = std::vector<CstNode_sptr>;
using vecNode_sptr = std::vector<Node_sptr>;

using CstNode_wptr = std::weak_ptr<const Node>;
using vecCstNode_wptr = std::vector<CstNode_wptr>;

class Node {
protected:
    /**
     * Local properties (in its parent node space)
     */
    struct LocalProperties {
        float _width = 1.f;
        float _height = 1.f;
        float _positionX = 0.f;
        float _positionY = 0.f;
    };

public:
    Node(CstNode_sptr &parent, const Node::LocalProperties &localProperties);

    void setChildren(vecCstNode_wptr &&children);

    const LocalProperties &getLocalProperties() const;

protected:
    const CstNode_sptr &getParent() const;

private:
    CstNode_sptr _parent;
    const LocalProperties _localProperties;
    vecCstNode_wptr _children;
};


#endif //JUMPERBALLAPPLICATION_NODE_H

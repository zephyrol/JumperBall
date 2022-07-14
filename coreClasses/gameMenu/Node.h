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

using Node_rptr = Node*;
using CstNode_rptr = const Node*;
using vecNode_rptr = std::vector<Node_rptr>;

class Node {
protected:

    /**
     * Node transform containing scale and position
     */
    struct Transform {

        /**
         * Width between 0.f and 1.f
         */
        float width;

        /**
         * Height between 0.f and 1.f
         */
        float height;

        /**
         * Horizontal position between -0.5f, and 0.5f
         */
        float positionX;

        /**
         * Vertical position between -0.5f, and 0.5f
         */
        float positionY;
    };

public:
    Node(const Node_sptr &parent, const Node::Transform &transform, float ratio);

    const Transform &getLocalTransform() const;

    CstNode_rptr getClickNode(float screenX, float screenY);

    float ratio() const;

    /**
     * Update screen space transform of the node and for each child.
     * @param parentTransform screen space parent transform
     */
    void updateScreenTransform(const Transform &parentTransform);

protected:
    /**
     * Get size of an inside node (child) in the outside node space (parent)
     * @param parentRatio Ratio of the outside node (parent)
     * @param childRatio Ratio of the inside node (child)
     * @return Width and height as vec2
     */
    static JBTypes::vec2f computeChildNodeSize(float parentRatio, float childRatio);

    static Transform getIdentityTransform();

    virtual std::unique_ptr<Node::Transform> getAdditionalLocalTransform();


private:

    CstNode_sptr _parent;

    /**
     * Local transform (in its parent node space)
     */
    const Transform _localTransform;

    const float _ratio;

    vecNode_rptr _children;

    /**
     * Node transformation in screen space. May be updated
     */
    std::unique_ptr<Transform> _screenTransform;

    void addChild(Node_rptr child);

};


#endif //JUMPERBALLAPPLICATION_NODE_H

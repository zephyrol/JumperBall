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

using Node_rptr = Node *;
using CstNode_rptr = const Node *;
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
    Node(const Node::Transform &transform, float ratio);

    virtual ~Node() = default;

    const Transform &getTransform() const;

    /**
     * Check if picking is on the node
     * @param mouseX X coordinate in screen space
     * @param mouseY Y coordinate in screen space
     * @return true if picking is on the node, else false
     */
    bool intersect(float mouseX, float mouseY) const;

    float ratio() const;
    float width() const;
    float height() const;
    float positionX() const;
    float positionY() const;

protected:
    /**
     * Get size of an inside node (child) in the outside node space (parent)
     * @param parentRatio Ratio of the outside node (parent)
     * @param childRatio Ratio of the inside node (child)
     * @return Width and height as vec2
     */
    static JBTypes::vec2f computeNodeSize(float parentRatio, float childRatio);

    /**
     * Get identity node transform.
     * @return Identity transform
     */
    static Transform getIdentityTransform();

    virtual std::unique_ptr<Node::Transform> getAdditionalLocalTransform() const;

private:

    /**
     * Transform (in screen space)
     */
    const Transform _transform;

    /**
     * Ratio (in screen space)
     */
    const float _ratio;
};


#endif //JUMPERBALLAPPLICATION_NODE_H

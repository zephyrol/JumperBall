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
    Node(const CstNode_sptr &parent, const Node::Transform &transform, float ratio);

    virtual ~Node() = default;

    const Transform &getLocalTransform() const;

    /**
     * Check if picking is on the node
     * @param screenX X coordinate in screen space
     * @param screenY Y coordinate in screen space
     * @return true if picking is on the node, else false
     */
    bool intersect(float screenX, float screenY);

    const Transform &getScreenTransform() const;

    float ratio() const;

    size_t height() const;

    float getScreenSpaceHeight() const;

    float getScreenSpaceWidth() const;

    /**
     * Update screen transform using its parent node one.
     * Make sure that its node parent was updated before
     */
    void updateScreenTransform();

    /**
     * Update screen transforms of several nodes considering their height in the tree in account.
     * @param nodes Vector of nodes to update
     */
    static void updateScreenTransforms(const vecNode_sptr &nodes);

protected:
    /**
     * Get size of an inside node (child) in the outside node space (parent)
     * @param parentRatio Ratio of the outside node (parent)
     * @param childRatio Ratio of the inside node (child)
     * @return Width and height as vec2
     */
    static JBTypes::vec2f computeChildNodeSize(float parentRatio, float childRatio);

    /**
     * Get identity node transform.
     * @return Identity transform
     */
    static Transform getIdentityTransform();

    virtual std::unique_ptr<Node::Transform> getAdditionalLocalTransform() const;

private:

    /**
     * Parent node in the tree, may be nullptr
     */
    const CstNode_sptr _parent;

    /**
     * Node height in the node tree
     */
    const size_t _height;

    /**
     * Local transform (in its parent node space)
     */
    const Transform _localTransform;

    /**
     * Node ratio (> 1 means horizontal)
     */
    const float _ratio;

    /**
     * Node transformation in screen space. May be updated.
     */
    std::unique_ptr<Transform> _screenTransform;

    /**
     * Compute screen space transform of the node.
     */
    std::unique_ptr<Transform> computeScreenTransform() const;
};


#endif //JUMPERBALLAPPLICATION_NODE_H

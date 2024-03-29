//
// Created by S.Morgenthaler on 13/07/22.
//

#ifndef JUMPERBALLAPPLICATION_NODE_H
#define JUMPERBALLAPPLICATION_NODE_H

#include "scene/Displayable.h"

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
     * Node transform containing scale and position
     */
    struct Transform {

        /**
         * Width (Screen space is between 0.f and 1.f)
         */
        float width;

        /**
         * Height (Screen space is between 0.f and 1.f)
         */
        float height;

        /**
         * Horizontal position (Screen space is between -0.5f, and 0.5f)
         */
        float positionX;

        /**
         * Vertical position (Screen space is between -0.5f, and 0.5f)
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
     * @return squared distance from center if picking is on the node, else nullptr
     */
    std::unique_ptr<float> intersect(float mouseX, float mouseY, float touchThreshold = 2.f) const;

    float ratio() const;
    float width() const;
    float height() const;
    float positionX() const;
    float positionY() const;

    static Node_sptr getNearest(vecNode_sptr nodes, float mouseX, float mouseY);

protected:
    /**
     * Get size of an inside node (child) in the outside node space (parent)
     * @param parentRatio Ratio of the outside node (parent)
     * @param childRatio Ratio of the inside node (child)
     * @return Width and height as vec2
     */
    static JBTypes::vec2f computeNodeSize(float parentRatio, float childRatio);

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

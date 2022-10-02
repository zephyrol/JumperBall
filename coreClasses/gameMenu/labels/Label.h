/*
 * File: Label.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#ifndef LABEL_H
#define LABEL_H

#include "gameMenu/Geometry.h"
#include "scene/SceneElement.h"
#include "gameMenu/nodes/Node.h"
#include <fstream>

class Label;
using Label_sptr = std::shared_ptr<Label>;
using CstLabel_sptr = std::shared_ptr<const Label>;
using vecCstLabel_sptr = std::vector<CstLabel_sptr>;
using vecLabel_sptr = std::vector<Label_sptr>;

class Label: public SceneElement {

public:
    explicit Label(
        Node_sptr node,
        bool isActivated = false
    );

    virtual vecGeometry genGeometries() const;

    bool isActivated() const;

    virtual std::string message() const;

    const Node_sptr& getNode();

    StaticValues <JBTypes::vec2f> getStaticVec2fValues() const override = 0;

    void activate();

    void deactivate();

    static void updateLabelsLevels(vecLabel_sptr &labels, size_t end);

    GlobalState getGlobalState() const override;

    virtual ~Label() = default;

protected:
    const Node_sptr _node;

    /**
     * Create a displayable triangle geometry respecting the node position and scale.
     *  @param localX X position in range [-0.5, 0.5]
     *  @param localY Y position in range [-0.5, 0.5]
     *  @param dir Triangle direction
     *  @param triangleWidth Triangle width. Width = 1 means that the triangle covers the entire node space.
     *  @return Geometry to display in menu
     */
    Geometry createDisplayableTriangle(
        float localX,
        float localY,
        const JBTypes::Direction &dir,
        float triangleWidth
    ) const;

    /**
     * Create a displayable quad geometry respecting the node position and scale.
     *  @param localX X position in range [-0.5, 0.5]
     *  @param localY Y position in range [-0.5, 0.5]
     *  @param quadScaleX Quad X scale. Scale = 1 means that the quad covers the entire node space.
     *  @param quadScaleY Quad X scale. Scale = 1 means that the quad covers the entire node space.
     *  @return Geometry to display in menu
     */
    Geometry createDisplayableQuad(
        float localX,
        float localY,
        float quadScaleX,
        float quadScaleY
    ) const;

private:
    bool _isActivated;

};

#endif /* LABEL_H */

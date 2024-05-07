/*
 * File: Label.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#ifndef LABEL_H
#define LABEL_H

#include "gameMenu/LabelGeometry.h"
#include "scene/Displayable.h"
#include "gameMenu/nodes/Node.h"
#include <fstream>

class Label;

using Label_sptr = std::shared_ptr<Label>;
using CstLabel_sptr = std::shared_ptr<const Label>;
using vecCstLabel_sptr = std::vector<CstLabel_sptr>;
using vecLabel_sptr = std::vector<Label_sptr>;

class Label : public Displayable {

public:
    Label(
        Node_sptr node,
        const JBTypes::Color &color,
        short id
    );

    virtual vecLabelGeometry genGeometries() const;

    virtual std::string message() const;

    const Node_sptr &getNode();

     short getId() const;

    const JBTypes::Color& color() const;

    std::vector<short> getStaticShortValues() const override;

    ~Label() override = default;

protected:
    const Node_sptr _node;
    const JBTypes::Color _color;
    const short _id;

    /**
     * Create a displayable triangle geometry respecting the node position and scale.
     *  @param localX X position in range [-0.5, 0.5]
     *  @param localY Y position in range [-0.5, 0.5]
     *  @param dir Triangle direction
     *  @param triangleScaleX Triangle width. Width = 1 means the triangle covers the entire node space.
     *  @param triangleScaleY Triangle height. Height = 1 means the triangle covers the entire node space.
     *  @return LabelGeometry to display in menu
     */
    LabelGeometry createDisplayableTriangle(
        float localX,
        float localY,
        float triangleScaleX,
        float triangleScaleY,
        const JBTypes::Direction &dir
    ) const;

    /**
     * Create a displayable quad geometry respecting the node position and scale.
     *  @param localX X position in range [-0.5, 0.5]
     *  @param localY Y position in range [-0.5, 0.5]
     *  @param quadScaleX Quad X scale. Scale = 1 means that the quad covers the entire node space.
     *  @param quadScaleY Quad X scale. Scale = 1 means that the quad covers the entire node space.
     *  @return LabelGeometry to display in menu
     */
    LabelGeometry createDisplayableQuad(
        float localX,
        float localY,
        float quadScaleX,
        float quadScaleY
    ) const;

};

#endif /* LABEL_H */

/*
 * File: Label.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 17:44
 */

#ifndef LABEL_H
#define LABEL_H

#include "system/Geometry.h"
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
        CstNode_sptr node,
        bool isActivated = false
    );

    virtual vecGeometry genGeometries() const;

    bool isActivated() const;

    virtual std::string message() const;

    const CstNode_sptr& getNode() const;

    StaticValues <JBTypes::vec2f> getStaticVec2fValues() const override = 0;

    void activate();

    void deactivate();

    static void updateLabelsLevels(vecLabel_sptr &labels, size_t end);

    GlobalState getGlobalState() const override;

    virtual ~Label() = default;

protected:
    const CstNode_sptr _node;

private:
    bool _isActivated;

};

#endif /* LABEL_H */

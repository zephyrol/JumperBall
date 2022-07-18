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
#include "Node.h"
#include <fstream>

class Label;
using Label_sptr = std::shared_ptr<Label>;
using CstLabel_sptr = std::shared_ptr<const Label>;
using vecCstLabel_sptr = std::vector<CstLabel_sptr>;
using vecLabel_sptr = std::vector<Label_sptr>;

class Label {
public:

    explicit Label(
        const CstNode_sptr &node,
        bool isActivated = false
    );

    virtual ~Label() = 0;

    virtual vecGeometry genGeometries() const;

    bool isFixed() const;

    bool isActivated() const;

    virtual std::string message() const;

    void activate();

    void deactivate();

    static void updateLabelsLevels(vecLabel_sptr &labels, size_t end);

protected:
    const CstNode_sptr _node;
private:
    bool _isActivated;

};

#endif /* LABEL_H */

/*
 * File: ArrowLabel.h
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:42
 */

#ifndef ARROWLABEL_H
#define ARROWLABEL_H

#include "Label.h"

class ArrowLabel;
using ArrowLabel_sptr = std::shared_ptr<ArrowLabel>;

class ArrowLabel : public Label {
public:
    ArrowLabel(
        Node_sptr node,
        const JBTypes::Color &color,
        short id
    );

    ~ArrowLabel() override = default;

    vecLabelGeometry genGeometries() const override;
};

#endif // ARROWLABEL_H

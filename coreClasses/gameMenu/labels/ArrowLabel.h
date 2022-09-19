/*
 * File: ArrowLabel.h
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:42
 */

#ifndef ARROWLABEL_H
#define ARROWLABEL_H

#include "Label.h"

class ArrowLabel : public Label {
public:
    ArrowLabel(
        CstNode_sptr node,
        bool isActivated = true
    );

    virtual ~ArrowLabel() = default;

    vecGeometry genGeometries() const override;

private:
};

#endif // ARROWLABEL_H

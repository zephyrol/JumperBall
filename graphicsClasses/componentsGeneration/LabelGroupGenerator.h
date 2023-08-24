//
// Created by S.Morgenthaler on 23/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_LABELGROUPGENERATOR_H
#define JUMPERBALLAPPLICATION_LABELGROUPGENERATOR_H


#include "RenderGroupGenerator.h"
#include "gameMenu/labels/Label.h"
#include "gameMenu/pages/Page.h"

class LabelGroupGenerator: public RenderGroupGenerator {
public:
    explicit LabelGroupGenerator(vecCstLabel_sptr &&labels, CstPage_sptr page);

    RenderGroup_sptr genRenderGroup() const override;

private:
    const vecCstLabel_sptr _labels;
    const CstPage_sptr _page;

    static vecCstGeometricShape_sptr genGeometricShapesFromLabel(const Label &label);
};


#endif //JUMPERBALLAPPLICATION_LABELGROUPGENERATOR_H

//
// Created by S.Morgenthaler on 23/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_SCREENGROUPGENERATOR_H
#define JUMPERBALLAPPLICATION_SCREENGROUPGENERATOR_H

#include "RenderGroupGenerator.h"


class ScreenGroupGenerator : public RenderGroupGenerator {
public:
    RenderGroup_sptr genRenderGroup() const override;
};


#endif //JUMPERBALLAPPLICATION_SCREENGROUPGENERATOR_H

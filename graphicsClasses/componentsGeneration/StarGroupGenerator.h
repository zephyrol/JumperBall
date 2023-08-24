//
// Created by S.Morgenthaler on 23/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_STARGROUPGENERATOR_H
#define JUMPERBALLAPPLICATION_STARGROUPGENERATOR_H

#include "RenderGroupGenerator.h"
#include "scene/Star.h"


class StarGroupGenerator : public RenderGroupGenerator {
public:

    StarGroupGenerator(
        std::shared_ptr<const Star> star,
        std::shared_ptr<const Star> star2
    );

    RenderGroup_sptr genRenderGroup() const override;

private:
    const std::shared_ptr<const Star> _star;
    const std::shared_ptr<const Star> _star2;
};


#endif //JUMPERBALLAPPLICATION_STARGROUPGENERATOR_H

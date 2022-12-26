/*
 * File: Clock.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:48
 */

#include "Clock.h"

Clock::Clock(const JBTypes::vec3ui &position, const JBTypes::Dir &direction, const Ball_sptr &ball) :
    InteractiveItem(position, direction, ball) {
}

vecCstShape_sptr Clock::getShapes() const {

    const auto createClockBase = [](JBTypes::Color color, float radius) {
        return std::make_shared<const Shape>(
            Shape::Aspect::Cylinder,
            JBTypes::Color::Orange,
            std::initializer_list<Transformation>(
                {
                    Transformation(Transformation::Type::Translation,
                                   {0.f, -0.5f, 0.f}),
                    Transformation(Transformation::Type::Scale,
                                   {0.4f, 0.05f, 0.4f}),
                    Transformation(Transformation::Type::Rotation,
                                   {static_cast<float>(M_PI / 2), 0.f, 0.f})
                }
            )
        );
    };

    const std::vector<std::pair<JBTypes::Color, float> > colorsRadii = {
        {JBTypes::Color::White, 0.4f},
        {JBTypes::Color::Blue,  0.5f}
    };
    std::vector<CstShape_sptr> shapes{};
    for (const auto &colorRadius: colorsRadii) {
        shapes.push_back(createClockBase(colorRadius.first, colorRadius.second));
    }

    shapes.push_back(std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Black,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {0.05f, 0.3f, 0.050f}),
                Transformation(Transformation::Type::Translation,
                               {0.f, -0.025f, 0.f})
            }
        )
    ));

    shapes.push_back(std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Yellow,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {0.1f, 0.05f, 0.05f}),
                Transformation(Transformation::Type::Translation,
                               {0.025f, -0.05f, 0.f})
            }
        )
    ));
    return shapes;
}

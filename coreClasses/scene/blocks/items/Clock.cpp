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

    std::vector<CstShape_sptr> shapes{};

    const auto clockHandsRotation = Transformation(
        Transformation::Type::Rotation,
        JBTypesMethods::rotationVectorUpToDir(direction())
    );

    constexpr auto clockHandsThickness = 0.03f;
    shapes.push_back(std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Black,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {0.03f, 0.2f, clockHandsThickness}),
                Transformation(Transformation::Type::Translation,
                               {0.f, 0.08f, 0.02f}),
                clockHandsRotation
            }
        )
    ));

    shapes.push_back(std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Black,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {0.15f, 0.04f, clockHandsThickness}),
                Transformation(Transformation::Type::Translation,
                               {0.08f, 0.f, 0.02f}),
                clockHandsRotation
            }
        )
    ));

    shapes.push_back(std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Black,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {0.03f, 0.2f, clockHandsThickness}),
                Transformation(Transformation::Type::Translation,
                               {0.f, 0.08f, -0.02f}),
                clockHandsRotation
            }
        )
    ));

    shapes.push_back(std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Black,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {0.15f, 0.04f, clockHandsThickness}),
                Transformation(Transformation::Type::Translation,
                               {-0.08f, 0.f, -0.02f}),
                clockHandsRotation
            }
        )
    ));


    const auto createClockBase = [this](
        JBTypes::Color color,
        float radius,
        float thickness
    ) {
        return std::make_shared<const Shape>(
            Shape::Aspect::Cylinder,
            color,
            std::initializer_list<Transformation>(
                {
                    Transformation(Transformation::Type::Translation,
                                   {0.f, -0.5f, 0.f}),
                    Transformation(Transformation::Type::Scale,
                                   {radius, thickness, radius}),
                    Shape::getVerticalCylinderRotation(direction())
                }
            )
        );
    };
    struct ColorRadiusThickness {
        JBTypes::Color color;
        float radius;
        float thickness;
    };
    const std::vector<ColorRadiusThickness> colorsRadiiThickness{
        {JBTypes::Color::White, 0.4f, 0.055f},
        {JBTypes::Color::Blue,  0.5f, 0.05f}
    };
    for (const auto &colorRadiusThickness: colorsRadiiThickness) {
        shapes.push_back(createClockBase(
            colorRadiusThickness.color,
            colorRadiusThickness.radius,
            colorRadiusThickness.thickness
        ));
    }

    return shapes;
}

void Clock::catchItem() {
    const auto ball = _ball.lock();
    ball->obtainClock();
}

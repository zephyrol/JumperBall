/*
 * File: JumpBlock.cpp
 * Author: S.Morgenthaler
 *
 * Created on 11 avril 2020, 19:42
 */

#include "JumpBlock.h"
#include "system/SoundOutput.h"

JumpBlock::JumpBlock(const JBTypes::vec3ui &position,
                     const vecItem_sptr &items,
                     const vecEnemy_sptr &enemies,
                     const vecSpecial_sptr &specials,
                     const Ball_sptr &ball,
                     const std::array<bool, 6> &facesJumpers) :
    InteractiveBlock(position, items, enemies, specials, ball),
    _facesJumpers(facesJumpers) {
}

std::array<bool, 6> JumpBlock::faceInfo() const {
    return _facesJumpers;
}

Block::Effect JumpBlock::detectionEvent() {
    const auto ball = _ball.lock();
    const unsigned int dir = JBTypesMethods::directionAsInteger(ball->currentSide());
    ball->addUpdateOutput(std::make_shared<SoundOutput>("touchedJumper"));
    return faceInfo().at(dir) ? Block::Effect::Jump : Block::Effect::Nothing;
}

vecCstShape_sptr JumpBlock::getExtraShapes() const {

    vecCstShape_sptr shapes{};
    for (size_t i = 0; i < _facesJumpers.size(); i++) {

        const bool isJumper = _facesJumpers.at(i);
        if (isJumper) {
            constexpr float sizeBlock = 1.f; // TODO specify it elsewhere
            constexpr float offset = sizeBlock / 2.f;

            const JBTypes::Dir direction = JBTypesMethods::integerAsDirection(
                static_cast <unsigned int>(i)
            );

            const JBTypes::vec3f localScale{0.7f, 0.05f, 0.7f};

            const JBTypes::vec3f translationOnBlock{
                0.f,
                offset,
                0.f
            };

            const JBTypes::vec3f translationPosition{
                static_cast<float>(_position.at(0)),
                static_cast<float>(_position.at(1)),
                static_cast<float>(_position.at(2))
            };

            const auto directionRotation = JBTypesMethods::rotationVectorUpToDir(direction);

            shapes.push_back(std::make_shared<const Shape>(
                Shape::Aspect::Cylinder,
                JBTypes::Color::Yellow,
                std::initializer_list<Transformation>(
                    {
                        Transformation(Transformation::Type::Scale, localScale),
                        Transformation(Transformation::Type::Translation, translationOnBlock),
                        Transformation(Transformation::Type::Rotation, directionRotation),
                        Transformation(Transformation::Type::Translation, translationPosition)
                    }
                )
            ));
        }
    }
    return shapes;
}


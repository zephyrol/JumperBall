/*
 * File: SharpBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:51
 */

#include "SharpBlock.h"

SharpBlock::SharpBlock(const JBTypes::vec3ui &position,
                       const vecItem_sptr &items,
                       const vecEnemy_sptr &enemies,
                       const vecSpecial_sptr &specials,
                       const Ball_sptr &ball,
                       const std::array<bool, 6> &facesSharps) :
    InteractiveBlock(position, items, enemies, specials, ball, true),
    _facesSharps((facesSharps)),
    _jumpingSharpBoundingBoxes(computeSharpBoundingBoxes(jumpingSharpSize)),
    _movingSharpBoundingBoxes(computeSharpBoundingBoxes(movingSharpSize)) {
}

Block::Effect SharpBlock::interaction() const {

    const auto isInSharpZone =
        [](const JBTypes::vec3f &position,
           float x_min, float x_max,
           float y_min, float y_max,
           float z_min, float z_max
        ) {
            return position.x > x_min && position.x < x_max &&
                   position.y > y_min && position.y < y_max &&
                   position.z > z_min && position.z < z_max;
        };

    const auto& state = _ball.lock()->state();
    const auto& sharpBoundingBoxes = state == Ball::State::Jumping || state == Ball::State::Falling
        ? _jumpingSharpBoundingBoxes
        : _movingSharpBoundingBoxes;
    for (const auto &boundingBox: sharpBoundingBoxes) {
        const auto ball = _ball.lock();
        if (isInSharpZone(
            ball->get3DPosition(),
            boundingBox.first.x,
            boundingBox.second.x,
            boundingBox.first.y,
            boundingBox.second.y,
            boundingBox.first.z,
            boundingBox.second.z
        )) {
            return Block::Effect::Burst;
        }
    }
    return InteractiveBlock::interaction();
}

std::array<bool, 6> SharpBlock::faceInfo() const {
    return _facesSharps;
}

vecCstShape_sptr SharpBlock::getExtraShapes() const {

    vecCstShape_sptr shapes{};
    constexpr size_t numberOfSpikes = 13;
    const std::array<JBTypes::vec2f, numberOfSpikes> translationFloorFactors{
        JBTypes::vec2f{0.f, 0.f},
        JBTypes::vec2f{0.4f, 0.4f},
        JBTypes::vec2f{-0.4f, 0.4f},
        JBTypes::vec2f{0.4f, -0.4f},
        JBTypes::vec2f{-0.4f, -0.4f},
        JBTypes::vec2f{0.f, 0.85f},
        JBTypes::vec2f{0.f, -0.85f},
        JBTypes::vec2f{0.85f, 0.f},
        JBTypes::vec2f{-0.85f, 0.f},
        JBTypes::vec2f{0.85f, 0.85f},
        JBTypes::vec2f{0.85f, -0.85f},
        JBTypes::vec2f{-0.85f, 0.85f},
        JBTypes::vec2f{-0.85f, -0.85f}
    };

    for (size_t i = 0; i < _facesSharps.size(); i++) {

        const bool isSharp = _facesSharps.at(i);
        if (isSharp) {
            constexpr float sizeBlock = 1.f;
            constexpr float offset = sizeBlock / 2.f;

            const JBTypes::Dir currentDir = JBTypesMethods::integerAsDirection(
                static_cast <unsigned int>(i)
            );

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


            for (auto translationFloorFactor : translationFloorFactors) {
                constexpr auto localScaleFactor = 0.15f;

                const JBTypes::vec3f localScale{localScaleFactor, localScaleFactor, localScaleFactor};

                const JBTypes::vec3f translationFloor{
                    offset * translationFloorFactor.x,
                    0.f,
                    offset * translationFloorFactor.y
                };

                const JBTypes::vec3f localTranslation = JBTypesMethods::add(
                    translationOnBlock,
                    translationFloor
                );

                const auto directionRotation = JBTypesMethods::rotationVectorUpToDir(currentDir);

                shapes.push_back(std::make_shared<const Shape>(
                    Shape::Aspect::Pyramid,
                    JBTypes::Color::None,
                    std::initializer_list<Transformation>(
                        {
                            Transformation(Transformation::Type::Scale,
                                           localScale),
                            Transformation(
                                Transformation::Type::Translation,
                                localTranslation),
                            Transformation(Transformation::Type::Rotation,
                                           directionRotation),
                            Transformation(
                                Transformation::Type::Translation,
                                translationPosition)
                        }
                    )
                ));
            }
        }
    }
    return shapes;
}

std::vector<std::pair<JBTypes::vec3f, JBTypes::vec3f> > SharpBlock::computeSharpBoundingBoxes(
    float sharpsSize
) const {

    std::vector<std::pair<JBTypes::vec3f, JBTypes::vec3f>> boundingBoxes;

    const auto blockfXPos = static_cast <float>(_position.at(0));
    const auto blockfYPos = static_cast <float>(_position.at(1));
    const auto blockfZPos = static_cast <float>(_position.at(2));
    for (size_t i = 0; i < _facesSharps.size(); ++i) {

        if (_facesSharps.at(i)) {
            constexpr float halfSizeBlock = 0.5f;
            float posBlockfXMin = blockfXPos - halfSizeBlock;
            float posBlockfXMax = blockfXPos + halfSizeBlock;
            float posBlockfYMin = blockfYPos - halfSizeBlock;
            float posBlockfYMax = blockfYPos + halfSizeBlock;
            float posBlockfZMin = blockfZPos - halfSizeBlock;
            float posBlockfZMax = blockfZPos + halfSizeBlock;

            JBTypes::Dir dir = JBTypesMethods::integerAsDirection(static_cast <unsigned int>(i));
            JBTypes::vec3f dirVec = JBTypesMethods::directionAsVector(dir);

            if (dirVec.x > EPSILON_F || dirVec.x < -EPSILON_F) {
                if (dirVec.x < 0) {
                    posBlockfXMin -= sharpsSize;
                } else if (dirVec.x > 0) {
                    posBlockfXMax += sharpsSize;
                }
            }
            if (dirVec.y > EPSILON_F || dirVec.y < -EPSILON_F) {
                if (dirVec.y < 0) {
                    posBlockfYMin -= sharpsSize;
                } else if (dirVec.y > 0) {
                    posBlockfYMax += sharpsSize;
                }
            }
            if (dirVec.z > EPSILON_F || dirVec.z < -EPSILON_F) {
                if (dirVec.z < 0) {
                    posBlockfZMin -= sharpsSize;
                } else if (dirVec.z > 0) {
                    posBlockfZMax += sharpsSize;
                }
            }
            boundingBoxes.push_back(
                {
                    {posBlockfXMin, posBlockfYMin, posBlockfZMin},
                    {posBlockfXMax, posBlockfYMax, posBlockfZMax}
                }
            );
        }
    }

    return boundingBoxes;
}

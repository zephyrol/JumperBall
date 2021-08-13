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
                       const Ball_sptr& ball,
                       const std::array<bool, 6> &facesSharps):
                       Block(position, ball, items, enemies, specials, true),
                       _facesSharps((facesSharps)){

}

Block::Effect SharpBlock::interaction (
    const JBTypes::timePointMs&
    ) {

    auto isInSharpZone = 
    []( const JBTypes::vec3f &position,
            float x_min, float x_max,
            float y_min, float y_max,
            float z_min, float z_max
    )
    {
        return position.x > x_min && position.x < x_max &&
               position.y > y_min && position.y < y_max &&
               position.z > z_min && position.z < z_max;
    };

    constexpr float sizeSharp = 0.51f;
    constexpr float offsetCenter = 0.19f;
    const JBTypes::vec3ui posBlock = position();
    const auto posBlockfX = static_cast <float>(posBlock.at(0));
    const auto posBlockfY = static_cast <float>(posBlock.at(1));
    const auto posBlockfZ = static_cast <float>(posBlock.at(2));
    for (size_t i = 0; i < _facesSharps.size(); ++i) {

        if (_facesSharps.at(i)) {
            float posBlockfXMin = posBlockfX;
            float posBlockfXMax = posBlockfX + 1;
            float posBlockfYMin = posBlockfY;
            float posBlockfYMax = posBlockfY + 1;
            float posBlockfZMin = posBlockfZ;
            float posBlockfZMax = posBlockfZ + 1;

            JBTypes::Dir dir =
                JBTypesMethods::integerAsDirection(
                    static_cast <unsigned int>(i));
            JBTypes::vec3f dirVec =
                JBTypesMethods::directionAsVector(dir);

            if (dirVec.x > EPSILON_F || dirVec.x < -EPSILON_F) {
                posBlockfYMin += offsetCenter;
                posBlockfYMax -= offsetCenter;
                posBlockfZMin += offsetCenter;
                posBlockfZMax -= offsetCenter;
                if (dirVec.x < 0) {
                    posBlockfXMin -= sizeSharp;
                } else if (dirVec.x > 0) {
                    posBlockfXMax += sizeSharp;
                }
            }
            if (dirVec.y > EPSILON_F || dirVec.y < -EPSILON_F) {
                posBlockfXMin += offsetCenter;
                posBlockfXMax -= offsetCenter;
                posBlockfZMin += offsetCenter;
                posBlockfZMax -= offsetCenter;
                if (dirVec.y < 0) {
                    posBlockfYMin -= sizeSharp;
                } else if (dirVec.y > 0) {
                    posBlockfYMax += sizeSharp;
                }
            }
            if (dirVec.z > EPSILON_F || dirVec.z < -EPSILON_F) {
                posBlockfXMin += offsetCenter;
                posBlockfXMax -= offsetCenter;
                posBlockfYMin += offsetCenter;
                posBlockfYMax -= offsetCenter;
                if (dirVec.y < 0) {
                    posBlockfZMin -= sizeSharp;
                } else if (dirVec.y > 0) {
                    posBlockfZMax += sizeSharp;
                }
            }

            if (isInSharpZone(
                    _ball->get3DPosition(),
                    posBlockfXMin,
                    posBlockfXMax,
                    posBlockfYMin,
                    posBlockfYMax,
                    posBlockfZMin,
                    posBlockfZMax
                )) {
                return Block::Effect::Burst;
            }
        }
    }
    return Block::Effect::Nothing;
}

std::array <bool, 6> SharpBlock::faceInfo() const {
    return _facesSharps;
}

std::vector<Shape> SharpBlock::getExtraShapes() const {


    std::vector<Shape> shapes {};
    const std::array <float, 7> scales { .2f, .1f, .05f, .1f, .075f, .15f, .175f };

    const std::array <JBTypes::vec2f, 7> translationFloorFactor {
        JBTypes::vec2f{0.f, 0.f}, JBTypes::vec2f{-0.6f, -0.4f}, JBTypes::vec2f{0.6f, -0.6f},
        JBTypes::vec2f{0.2f, 0.6f}, JBTypes::vec2f{-0.2f, -0.6f}, JBTypes::vec2f{0.6f, 0.6f},
        JBTypes::vec2f{-0.6f, 0.6f}
    };

    for (size_t i = 0; i < _facesSharps.size(); i++) {

        const bool isSharp = _facesSharps.at(i);
        if (isSharp) {
            constexpr float sizeBlock = 1.f; // TODO specify it elsewhere
            constexpr float offset = sizeBlock / 2.f;

            const JBTypes::Dir currentDir = JBTypesMethods::integerAsDirection(
                static_cast <unsigned int>(i)
            );
            const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(currentDir);

            const JBTypes::vec3f translationOffset = {-offset, 0, -offset};

            for (size_t j = 0; j < scales.size(); j++) {
                const JBTypes::vec3f scaleLocal { scales.at(j), 0.5f, scales.at(j) };

                const JBTypes::vec3f translationLocal {
                    static_cast<float>(_position.at(0)) + offset + vecDir.x * offset,
                    static_cast<float>(_position.at(1)) + offset + vecDir.y * offset,
                    static_cast<float>(_position.at(2)) + offset + vecDir.z * offset
                };

                const JBTypes::vec3f translationFloor {
                    offset * translationFloorFactor.at(j).x,
                    0.f,
                    offset * translationFloorFactor.at(j).y
                };

                shapes.emplace_back(
                    Shape::Aspect::Pyramid,
                    JBTypes::Color::None,
                    currentDir,
                    JBTypesMethods::add(
                        JBTypesMethods::add(translationOffset, translationLocal),
                        translationFloor
                    ),
                    scaleLocal
                );
            }
        }
    }
    return shapes;
}

/*
 * File: JumpBlock.cpp
 * Author: seb
 *
 * Created on 11 avril 2020, 19:42
 */

#include "JumpBlock.h"

JumpBlock::JumpBlock(const JBTypes::vec3ui &position,
                     const vecItem_sptr &items,
                     const vecEnemy_sptr &enemies,
                     const vecSpecial_sptr &specials,
                     const Ball_sptr& ball,
                     const std::array<bool, 6> &facesJumpers):
                     Block(position, ball, items, enemies, specials),
                     _facesJumpers(facesJumpers) {
}

std::array <bool, 6> JumpBlock::faceInfo() const {
    return _facesJumpers;
}

Block::Effect JumpBlock::detectionEvent (const JBTypes::Dir& direction,
                                         const JBTypes::timePointMs&) {
    const unsigned int dir = JBTypesMethods::directionAsInteger(direction);
    return faceInfo().at(dir) ? Block::Effect::Jump : Block::Effect::Nothing;
}

std::vector<Shape> JumpBlock::getExtraShapes() const {

    std::vector<Shape> shapes {};
    for (size_t i = 0; i < _facesJumpers.size(); i++) {

        const bool isJumper = _facesJumpers.at(i);
        if (isJumper) {
            constexpr float sizeBlock = 1.f; // TODO specify it elsewhere
            constexpr float offset = sizeBlock / 2.f;

            const JBTypes::Dir currentDir = JBTypesMethods::integerAsDirection(
                static_cast <unsigned int>(i)
            );
            const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(currentDir);

            const JBTypes::vec3f scaleLocal { 0.7f, 0.05f, 0.7f };

            const JBTypes::vec3f translationLocal {
                static_cast<float>(_position.at(0)) + vecDir.x * offset,
                static_cast<float>(_position.at(1)) + vecDir.y * offset,
                static_cast<float>(_position.at(2)) + vecDir.z * offset
            };

            shapes.emplace_back(
                Shape::Aspect::Cylinder,
                JBTypes::Color::Yellow,
                currentDir,
                translationLocal,
                scaleLocal
            );
        }
    }
    return shapes;
}


/*
 * File: Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#include <iostream>
#include <algorithm>
#include <functional>
#include <cmath>
#include <cctype>
#include <future>
#include "Map.h"

Map::Map(Map::MapInfo&& mapInfo):
    _blocks(std::move(mapInfo.blocks)),
    _blocksWithInteraction(createBlocksWithInteraction()),
    _blocksWithItems(createBlocksWithItems()),
    _blocksWithSpecials(createBlocksWithSpecials()),
    _width(mapInfo.width),
    _height(mapInfo.height),
    _depth(mapInfo.depth),
    _beginX(mapInfo.beginX),
    _beginY(mapInfo.beginY),
    _beginZ(mapInfo.beginZ),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _blocksInteractions([this] (size_t blockNumber) {
                            return _blocksWithInteraction.at(blockNumber)->interaction(
                                _dirBallInteractions,
                                _timeInteractions,
                                _posBallInteractions
                                );
                        }, _blocksWithInteraction.size()),
    _itemsInteractions([this] (size_t blockNumber) {
                            _blocksWithItems.at(blockNumber)->catchItem(_posBallInteractions, _radiusInteractions);
                       }, _blocksWithItems.size()),
    _enemiesInteractions([this] (size_t blockNumber) {
                            return _blocksWithEnemies.at(blockNumber)->updateEnemies(
                                _posBallInteractions,
                                _radiusInteractions
                            );
                         }, _blocksWithEnemies.size()),
    _dirBallInteractions(JBTypes::Dir::North),
    _posBallInteractions({ 0.F, 0.F, 0.F }),
    _radiusInteractions(0.F),
    _timeInteractions() {
}

Block_sptr Map::getBlock (int x, int y, int z) {
    CstBlock_sptr constBlock = static_cast <const Map&>(*this).getBlock(x, y, z);

    return std::const_pointer_cast <Block>(constBlock);
}

std::string Map::positionToString(const JBTypes::vec3ui& position) {
    return std::to_string(position.at(0)) + "," + 
        std::to_string(position.at(1)) + "," +
        std::to_string(position.at(2));
}

JBTypes::vec3ui Map::stringToPosition(const std::string& stringPosition) {
    const auto& firstComma = std::find(stringPosition.begin(), stringPosition.end(), ',');
    const auto& secondComma = std::find(firstComma + 1, stringPosition.end(), ',');
    return {
        static_cast<unsigned int>(std::stoi(std::string(stringPosition.begin(), firstComma))), 
        static_cast<unsigned int>(std::stoi(std::string(firstComma + 1, secondComma))), 
        static_cast<unsigned int>(std::stoi(std::string(secondComma + 1, stringPosition.end())))
    };
}

std::map<std::string,Block_sptr> Map::createBlockPositions() const {
    std::map<std::string,Block_sptr> positions;
    for (const auto& block: _blocks) {
        const auto position = block->position();
        positions[positionToString(position)] = block;
    }
    return positions;
}

std::vector<Block_sptr> Map::createBlocksWithInteraction() const {
    std::vector<Block_sptr>  blocksWithInteraction;
    for (const auto& block: _blocks) {
        if (block->hasInteraction()) {
            blocksWithInteraction.push_back(block);
        }
    }
    return blocksWithInteraction;
}

std::vector<Block_sptr> Map::createBlocksWithItems() const {
    std::vector<Block_sptr>  blocksWithItems;
    for (const auto& block: _blocks) {
        if (!block->getItems().empty()) {
            blocksWithItems.push_back(block);
        }
    }
    return blocksWithItems;
}

std::vector<Block_sptr> Map::createBlocksWithEnemies() const {
    std::vector<Block_sptr> blocksWithEnemies;
    for (const auto& block: _blocks) {
        if (!block->getEnemies().empty()) {
            blocksWithEnemies.push_back(block);
        }
    }
    return blocksWithEnemies;
}

std::vector<Block_sptr> Map::createBlocksWithSpecials() const {
    std::vector<Block_sptr>  blocksWithSpecials;
    for (const auto& block: _blocks) {
        if (!block->getSpecials().empty()) {
            blocksWithSpecials.push_back(block);
        }
    }
    return blocksWithSpecials;
}

// TODO: move to special class
std::map <JBTypes::Color, bool> Map::createSpecialStates() const {
    constexpr bool defaultStateValue = true;
    return {
        {
            { JBTypes::Color::Red, defaultStateValue },
            { JBTypes::Color::Green, defaultStateValue },
            { JBTypes::Color::Blue, defaultStateValue },
            { JBTypes::Color::Yellow, defaultStateValue }
        }
    };
}

CstBlock_sptr Map::getBlock (int x, int y, int z) const {
    return _blocksPositions.at(positionToString({
        static_cast<unsigned int>(x),
        static_cast<unsigned int>(y),
        static_cast<unsigned int>(z)
    }));
}

unsigned int Map::width() const {
    return _width;
}

unsigned int Map::height() const {
    return _height;
}

unsigned int Map::depth() const {
    return _depth;
}

JBTypes::vec3f Map::getCenterMap() const {
    return {
        static_cast <const float>(_width) / 2.F,
        static_cast <const float>(_height) / 2.F,
        static_cast <const float>(_depth) / 2.F
    };
}

float Map::getLargestSize() const {
    const auto fWidth = static_cast <const float>(_width);
    const auto fHeight = static_cast <const float>(_height);
    const auto fDepth = static_cast <const float>(_depth);
    float boundingBoxMax = fWidth;
    if (boundingBoxMax < fHeight) {
        boundingBoxMax = fHeight;
    }
    if (boundingBoxMax < fDepth) {
        boundingBoxMax = fDepth;
    }
    return boundingBoxMax;
}

float Map::getTimeSinceCreation() const {
    return JBTypesMethods::getTimeSecondsSinceTimePoint(_creationTime);
}

unsigned int Map::beginX() const {
    return _beginX;
}

unsigned int Map::beginY() const {
    return _beginY;
}

unsigned int Map::beginZ() const {
    return _beginZ;
}

Map::Effect Map::interaction (
    const JBTypes::Dir& direction,
    const JBTypes::vec3f& boundingSpherePosition,
    float radius
) {
    _dirBallInteractions = direction;
    _posBallInteractions = boundingSpherePosition;
    _radiusInteractions = radius;
    _timeInteractions = JBTypesMethods::getTimePointMSNow();


    _blocksInteractions.runTasks();
    _itemsInteractions.runTasks();
    _enemiesInteractions.runTasks();

    std::shared_ptr <std::vector <Block::Effect> > blocksEffects = _blocksInteractions.waitTasks();
    Block::Effect finalBlockEffect = Block::Effect::Nothing;
    for (Block::Effect& blockEffect : *blocksEffects) {
        if (blockEffect != Block::Effect::Nothing) {
            finalBlockEffect = blockEffect;
        }
    }

    std::shared_ptr <std::vector <Block::Effect> > enemiesEffects = _enemiesInteractions.waitTasks();
    Block::Effect finalEnemyEffect = Block::Effect::Nothing;
    for (Block::Effect& enemyEffect : *enemiesEffects) {
        if (enemyEffect != Block::Effect::Nothing) {
            finalEnemyEffect = enemyEffect;
        }
    }
    _itemsInteractions.waitTasks();

    if (finalBlockEffect == Block::Effect::Burst || finalEnemyEffect == Block::Effect::Burst) {
        return Map::Effect::Burst;
    } else if (finalBlockEffect == Block::Effect::Jump) {
        return Map::Effect::Jump;
    } else if (finalBlockEffect == Block::Effect::Slide) {
        return Map::Effect::Slide;
    } else if (finalBlockEffect == Block::Effect::Burn) {
        return Map::Effect::Burn;
    } else if (
        finalBlockEffect == Block::Effect::Nothing ||
        finalEnemyEffect == Block::Effect::Nothing
        ) {
        return Map::Effect::Nothing;
    }
    return Map::Effect::Nothing;
}

void Map::switchColor (const JBTypes::Color& color) {
    // TODO update it
    // if (_specialsState.find(color) == _specialsState.end()) {
    //     _specialsState[color] = false;
    // } else {
    //     _specialsState.at(color) = !_specialsState.at(color);
    // }

    // for (SpecialInfo& specialInfo : _specials) {
    //     const std::shared_ptr <Special> special = specialInfo.special;
    //     if (special && special->getColor() == color) {
    //         special->switchOnOff();
    //     }
    // }

    // for (EnemyInfo& enemyInfo : _enemies) {
    //     const std::shared_ptr <Enemy> enemy = enemyInfo.enemy;
    //     if (enemy && enemy->getColor() == color) {
    //         enemy->switchOnOff();
    //     }
    // }
}

JBTypes::vec3ui Map::getBlockCoords (size_t index,
                                     unsigned int width,
                                     unsigned int depth) {
    const auto uIntIndex = static_cast <const unsigned int>(index);
    const unsigned int widthDepth = width * depth;
    return { uIntIndex % width, uIntIndex / widthDepth, (uIntIndex % widthDepth) / width };
}

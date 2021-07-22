/*
 * File: Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#include <iostream>
#include <algorithm>
#include <functional>
#include <math.h>
#include <cctype>
#include <future>
#include "Map.h"

Map::Map(Map::MapInfo&& mapInfo):
    _blocks(std::move(mapInfo.blocks)),
    _blocksInfo(std::move(mapInfo.blocksInfo)),
    _blocksWithInteractionInfo(getBlocksWithInteraction()),
    _blocksWithItemsIndices(getBlocksWithItems()),
    _enemies(std::move(mapInfo.enemiesInfo)),
    _specials(std::move(mapInfo.specialInfo)),
    _blocksTeleporters(createBlocksTeleporters()),
    _specialsState(createSpecialStates()),
    _width(std::move(mapInfo.width)),
    _height(std::move(mapInfo.height)),
    _depth(std::move(mapInfo.depth)),
    _beginX(std::move(mapInfo.beginX)),
    _beginY(std::move(mapInfo.beginY)),
    _beginZ(std::move(mapInfo.beginZ)),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _blocksInteractions([this] (size_t blockNumber) {
                            const Block_sptr& block =
                                getBlock(_blocksWithInteractionInfo.at(blockNumber).index);
                            return block->interaction(
                                _dirBallInteractions,
                                _timeInteractions,
                                _posBallInteractions
                                );
                        }, _blocksWithInteractionInfo.size()),
    _itemsInteractions([this] (size_t blockNumber) {
                           const Block_sptr& block =
                               getBlock(_blocksWithItemsIndices.at(blockNumber));
                           if (block->hasItems()) {
                               block->catchItem(_posBallInteractions, _radiusInteractions);
                           }
                       }, _blocksWithItemsIndices.size()),
    _enemiesInteractions([this] (size_t enemyNumber) {
                             const Map::EnemyInfo enemyInfo = _enemies.at(enemyNumber);
                             const std::shared_ptr <Enemy> enemy = enemyInfo.enemy;
                             return enemy->update(_posBallInteractions, _radiusInteractions);
                         }, _enemies.size()),
    _dirBallInteractions(JBTypes::Dir::North),
    _posBallInteractions({ 0.f, 0.f, 0.f }),
    _radiusInteractions(0.f),
    _timeInteractions() {
}

Block_sptr Map::getBlock (int x, int y, int z) {
    CstBlock_sptr constBlock =
        static_cast <const Map&>(*this).getBlock(x, y, z);

    return std::const_pointer_cast <Block>(constBlock);
}

Block_sptr Map::getBlock (size_t index) {
    const CstBlock_sptr& constBlock =
        static_cast <const Map&>(*this).getBlock(index);

    return std::const_pointer_cast <Block>(constBlock);
}

std::vector <Map::BlockInfo> Map::getBlocksWithInteraction() const {
    std::vector <Map::BlockInfo> blocksWithInteraction;
    for (unsigned int i = 0; i < _blocksInfo.size(); ++i) {
        const CstBlock_sptr& block =
            getBlock(_blocksInfo.at(i).index);
        if (block->hasInteraction()) {
            blocksWithInteraction.push_back(_blocksInfo.at(i));
        }
    }
    return blocksWithInteraction;
}

std::vector <size_t> Map::getBlocksWithItems() const {
    std::vector <size_t> blocksWithItemsIndices;
    for (unsigned int i = 0; i < _blocksInfo.size(); ++i) {
        const size_t index = _blocksInfo.at(i).index;
        const CstBlock_sptr& block =
            getBlock(index);
        if (block->hasItems()) {
            blocksWithItemsIndices.push_back(index);
        }
    }
    return blocksWithItemsIndices;
}

const std::map <JBTypes::Color, Map::TeleportersInfo>&
Map::getBlocksTeleporters() const {
    return _blocksTeleporters;
}

std::map <JBTypes::Color, Map::TeleportersInfo> Map::createBlocksTeleporters() const {
    std::map <JBTypes::Color, Map::TeleportersInfo> blocksTeleporters;
    for (
        unsigned int colorInt = static_cast <unsigned int>(JBTypes::Color::Red);
        colorInt < static_cast <unsigned int>(JBTypes::Color::Yellow);
        ++colorInt
        ) {
        const JBTypes::Color color = static_cast <JBTypes::Color>(colorInt);
        size_t counterTeleporter = 0;
        size_t indexFirstTeleporter;
        size_t indexSecondTeleporter;
        JBTypes::Dir directionFirstTeleporter;
        JBTypes::Dir directionSecondTeleporter;
        for (const Map::SpecialInfo& specialInfo : _specials) {
            if (
                specialInfo.type == Map::SpecialTypes::Teleporter &&
                specialInfo.special->getColor() == color
                ) {
                if (counterTeleporter == 0) {
                    indexFirstTeleporter = specialInfo.index;
                    directionFirstTeleporter = specialInfo.special->direction();
                } else {
                    indexSecondTeleporter = specialInfo.index;
                    directionSecondTeleporter = specialInfo.special->direction();
                }
                ++counterTeleporter;
            }
        }
        if (counterTeleporter == 2) {
            Map::TeleportersInfo teleporterInfo;
            teleporterInfo.coupleIndices =
            { indexFirstTeleporter, indexSecondTeleporter };
            teleporterInfo.coupleDirections =
            { directionFirstTeleporter, directionSecondTeleporter };
            blocksTeleporters[color] = teleporterInfo;
        } else if (counterTeleporter != 0) {
            std::cerr << "Error: The map contains an invalid number of " <<
                " teleporters for a specific color : " << counterTeleporter
                      << std::endl;
        }
    }
    return blocksTeleporters;
}

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

const std::map <JBTypes::Color, bool>& Map::getSpecialStates() const {
    return _specialsState;
}

CstBlock_sptr Map::getBlock (int x, int y, int z) const {
    CstBlock_sptr block;
    if (
        x >= static_cast <int>(_width) ||
        y >= static_cast <int>(_height) ||
        z >= static_cast <int>(_depth) ||
        x < 0 || y < 0 || z < 0
        )
        block = nullptr;
    else {
        size_t index = _width * (z + y * _depth) + x;
        block = _blocks.at(index);
    }
    return block;
}

JBTypes::vec3ui Map::getBlockCoords (size_t index) const {
    return getBlockCoords(index, _width, _depth);
}

size_t Map::getIndex (const JBTypes::vec3ui& coords) const {
    return _width * (coords.at(2) + coords.at(1) * _depth) + coords.at(0);
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
        static_cast <float>(_width) / 2.f,
        static_cast <float>(_height) / 2.f,
        static_cast <float>(_depth) / 2.f
    };
}

float Map::getLargestSize() const {
    float fWidth = static_cast <float>(_width);
    float fHeight = static_cast <float>(_height);
    float fDepth = static_cast <float>(_depth);
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

Map::Effect Map::interaction (const JBTypes::Dir& ballDir, const JBTypes::vec3f& posBall, float radius) {

    _dirBallInteractions = ballDir;
    _posBallInteractions = posBall;
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

    std::shared_ptr <std::vector <Enemy::Effect> > enemiesEffects = _enemiesInteractions.waitTasks();
    Enemy::Effect finalEnemyEffect = Enemy::Effect::Nothing;
    for (Enemy::Effect& enemyEffect : *enemiesEffects) {
        if (enemyEffect != Enemy::Effect::Nothing) {
            finalEnemyEffect = enemyEffect;
        }
    }
    _itemsInteractions.waitTasks();

    if (finalBlockEffect == Block::Effect::Burst || finalEnemyEffect == Enemy::Effect::Burst) {
        return Map::Effect::Burst;
    } else if (finalBlockEffect == Block::Effect::Jump) {
        return Map::Effect::Jump;
    } else if (finalBlockEffect == Block::Effect::Slide) {
        return Map::Effect::Slide;
    } else if (finalBlockEffect == Block::Effect::Burn) {
        return Map::Effect::Burn;
    } else if (
        finalBlockEffect == Block::Effect::Nothing ||
        finalEnemyEffect == Enemy::Effect::Nothing
        ) {
        return Map::Effect::Nothing;
    }
    return Map::Effect::Nothing;
}

void Map::switchColor (const JBTypes::Color& color) {
    if (_specialsState.find(color) == _specialsState.end()) {
        _specialsState[color] = false;
    } else {
        _specialsState.at(color) = !_specialsState.at(color);
    }

    for (SpecialInfo& specialInfo : _specials) {
        const std::shared_ptr <Special> special = specialInfo.special;
        if (special && special->getColor() == color) {
            special->switchOnOff();
        }
    }

    for (EnemyInfo& enemyInfo : _enemies) {
        const std::shared_ptr <Enemy> enemy = enemyInfo.enemy;
        if (enemy && enemy->getColor() == color) {
            enemy->switchOnOff();
        }
    }
}

JBTypes::vec3ui Map::getBlockCoords (size_t index,
                                     unsigned int width,
                                     unsigned int depth) {
    unsigned int uIntIndex = static_cast <unsigned int>(index);
    const unsigned int widthDepth = width * depth;
    return { uIntIndex % width, uIntIndex / widthDepth, (uIntIndex % widthDepth) / width };
}

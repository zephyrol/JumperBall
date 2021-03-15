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
    _blocksWithObjectsIndices(getBlocksWithObjects()),
    _enemies(std::move(mapInfo.enemiesInfo)),
    _specials(std::move(mapInfo.specialInfo)),
    _blocksTeleporters(createBlocksTeleporters()),
    _specialsState(createSpecialStates()),
    _width(std::move(mapInfo.width)),
    _height(std::move(mapInfo.height)),
    _deep(std::move(mapInfo.deep)),
    _beginX(std::move(mapInfo.beginX)),
    _beginY(std::move(mapInfo.beginY)),
    _beginZ(std::move(mapInfo.beginZ)),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _blocksInteractions([this] (size_t blockNumber) {
                            const std::shared_ptr <Block>& block =
                                getBlock(_blocksWithInteractionInfo.at(blockNumber).index);
                            return block->interaction(
                                _dirBallInteractions,
                                _timeInteractions,
                                _posBallInteractions,
                                getBlockCoords
                                    (_blocksWithInteractionInfo.at(blockNumber).index));
                        }, _blocksWithInteractionInfo.size()),
    _objectsInteractions([this] (size_t blockNumber) {
                             const std::shared_ptr <Block>& block =
                                 getBlock(_blocksWithObjectsIndices.at(blockNumber));
                             if (block->hasObjects()) {
                                 block->catchObject(
                                     getBlockCoords(_blocksWithObjectsIndices.at(blockNumber)),
                                     _posBallInteractions, _radiusInteractions);
                             }
                         }, _blocksWithObjectsIndices.size()),
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

std::shared_ptr <Block> Map::getBlock (int x, int y, int z) {
    std::shared_ptr <const Block> constBlock =
        static_cast <const Map&>(*this).getBlock(x, y, z);

    return std::const_pointer_cast <Block>(constBlock);
}

std::shared_ptr <Block> Map::getBlock (size_t index) {
    const std::shared_ptr <const Block>& constBlock =
        static_cast <const Map&>(*this).getBlock(index);

    return std::const_pointer_cast <Block>(constBlock);
}

std::vector <Map::BlockInfo> Map::getBlocksWithInteraction() const {
    std::vector <Map::BlockInfo> blocksWithInteraction;
    for (unsigned int i = 0; i < _blocksInfo.size(); ++i) {
        const std::shared_ptr <const Block>& block =
            getBlock(_blocksInfo.at(i).index);
        if (block->hasInteraction()) {
            blocksWithInteraction.push_back(_blocksInfo.at(i));
        }
    }
    return blocksWithInteraction;
}

std::vector <size_t> Map::getBlocksWithObjects() const {
    std::vector <size_t> blocksWithObjectsIndices;
    for (unsigned int i = 0; i < _blocksInfo.size(); ++i) {
        const size_t index = _blocksInfo.at(i).index;
        const std::shared_ptr <const Block>& block =
            getBlock(index);
        if (block->hasObjects()) {
            blocksWithObjectsIndices.push_back(index);
        }
    }
    return blocksWithObjectsIndices;
}

const std::map <JBTypes::Color, Map::TeleportersInfo>&
Map::getBlocksTeleporters() const {
    return _blocksTeleporters;
}

std::map <JBTypes::Color, Map::TeleportersInfo> Map::createBlocksTeleporters()
const {
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

std::shared_ptr <const Block> Map::getBlock (int x, int y, int z) const {
    std::shared_ptr <const Block> block;
    if (
        x >= static_cast <int>(_width) ||
        y >= static_cast <int>(_height) ||
        z >= static_cast <int>(_deep) ||
        x < 0 || y < 0 || z < 0
        )
        block = nullptr;
    else {
        size_t index = _width * (z + y * _deep) + x;
        block = _blocks.at(index);
    }
    return block;
}

std::shared_ptr <const Block> Map::getBlock (size_t index) const {
    return _blocks.at(index);
}

JBTypes::vec3ui Map::getBlockCoords (size_t index) const {
    return getBlockCoords(index, _width, _deep);
}

size_t Map::getIndex (const JBTypes::vec3ui& coords) const {
    return _width * (coords.at(2) + coords.at(1) * _deep) + coords.at(0);
}

Map::BlockTypes Map::getType (const JBTypes::vec3ui& position) const {
    const size_t index = getIndex(position);
    for (const Map::BlockInfo& blockInfo : _blocksInfo) {
        if (blockInfo.index == index)
            return blockInfo.type;
    }
    return Map::BlockTypes::None;
}

const std::vector <Map::BlockInfo>& Map::blocksInfo() const {
    return _blocksInfo;
}

unsigned int Map::width() const {
    return _width;
}

unsigned int Map::height() const {
    return _height;
}

unsigned int Map::deep() const {
    return _deep;
}

JBTypes::vec3f Map::getCenterMap() const {
    return {
        static_cast <float>(_width / 2.f),
        static_cast <float>(_height / 2.f),
        static_cast <float>(_deep / 2.f)
    };
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

Map::Effect Map::interaction (const JBTypes::Dir& ballDir,
                              const JBTypes::vec3f& posBall,
                              float radius) {

    _dirBallInteractions = ballDir;
    _posBallInteractions = posBall;
    _radiusInteractions = radius;
    _timeInteractions = JBTypesMethods::getTimePointMSNow();

    _blocksInteractions.runTasks();
    _objectsInteractions.runTasks();
    _enemiesInteractions.runTasks();

    std::shared_ptr <std::vector <Block::Effect> > blocksEffects =
        _blocksInteractions.waitTasks();
    Block::Effect finalBlockEffect = Block::Effect::Nothing;
    for (Block::Effect& blockEffect : *blocksEffects) {
        if (blockEffect != Block::Effect::Nothing) {
            finalBlockEffect = blockEffect;
        }
    }

    std::shared_ptr <std::vector <Enemy::Effect> > enemiesEffects =
        _enemiesInteractions.waitTasks();
    Enemy::Effect finalEnemyEffect = Enemy::Effect::Nothing;
    for (Enemy::Effect& enemyEffect : *enemiesEffects) {
        if (enemyEffect != Enemy::Effect::Nothing) {
            finalEnemyEffect = enemyEffect;
        }
    }
    _objectsInteractions.waitTasks();

    if (
        finalBlockEffect == Block::Effect::Burst ||
        finalEnemyEffect == Enemy::Effect::Burst
        ) {
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
                                     unsigned int deep) {
    unsigned int uIntIndex = static_cast <unsigned int>(index);
    const unsigned int widthDeep = width * deep;
    return { uIntIndex % width,
             uIntIndex / widthDeep,
             (uIntIndex % widthDeep) / width };
}

const std::vector <Map::EnemyInfo>& Map::getEnemiesInfo() const {
    return _enemies;
}

const std::vector <Map::SpecialInfo>& Map::getSpecialInfo() const {
    return _specials;
}

unsigned int Map::nbMaps = 0;

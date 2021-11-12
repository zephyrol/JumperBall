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

Map::Map(Map::MapInfo &&mapInfo):
    _blocks(std::move(mapInfo.blocks)),
    _blocksPositions(std::make_shared<const std::map<std::string,Block_sptr> >(createBlockPositions())),
    _blocksToUpdate(std::make_shared<const vecBlock_sptr>(getBlocksToUpdate())),
    _blocksTeleportations(
        std::make_shared<const std::map<BlockDir , BlockDir> >(createBlocksTeleportations())
    ),
    _ball(std::move(mapInfo.ball)),
    _width(mapInfo.width),
    _height(mapInfo.height),
    _depth(mapInfo.depth),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _blocksUpdating([this] (size_t blockNumber) {
                            _blocksToUpdate->at(blockNumber)->update(
                                    _updatingTime
                                );
                        }, _blocksToUpdate->size()),
    _updatingTime()
{
    _ball->setBlockPositions(_blocksPositions);
    _ball->setBlockWithInteractions(_blocksToUpdate);
    _ball->setBlockTeleportations(_blocksTeleportations);
    _ball->updateMovements();
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
        positions[Block::positionToString(position)] = block;
    }
    return positions;
}

std::vector<Block_sptr> Map::getBlocksToUpdate() const {
    std::vector<Block_sptr>  blocksWithInteraction;
    for (const auto& block: _blocks) {
        if (block->hasInteraction()) {
            blocksWithInteraction.push_back(block);
        }
    }
    return blocksWithInteraction;
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
        static_cast <float>(_width) / 2.F,
        static_cast <float>(_height) / 2.F,
        static_cast <float>(_depth) / 2.F
    };
}

float Map::getLargestSize() const {
    const auto fWidth = static_cast <float>(_width);
    const auto fHeight = static_cast <float>(_height);
    const auto fDepth = static_cast <float>(_depth);
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

void Map::update(const JBTypes::timePointMs& updatingTime, const Ball::ActionRequest& action) {

    _updatingTime = updatingTime;
    _ball->update(updatingTime, action);

    _blocksUpdating.runTasks();
    _blocksUpdating.waitTasks();

    if (ballIsOut()) {
        _ball->die();
    }
}

void Map::switchColor (const JBTypes::Color&) {
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
    const auto uIntIndex = static_cast <unsigned int>(index);
    const unsigned int widthDepth = width * depth;
    return { uIntIndex % width, uIntIndex / widthDepth, (uIntIndex % widthDepth) / width };
}

bool Map::ballIsOut() const {

    const auto ballPosition = _ball->get3DPosition();
    constexpr float thresholdOut = 5.f;
    // TODO: get ball 3D position
    return (
        ballPosition.x < -thresholdOut || ballPosition.x > (static_cast <float>(_width) + thresholdOut) ||
        ballPosition.y < -thresholdOut || ballPosition.y > (static_cast <float>(_height) + thresholdOut) ||
        ballPosition.z < -thresholdOut || ballPosition.z > (static_cast <float>(_depth) + thresholdOut)
    );
}

JBTypes::vec3f Map::getNextLook() const {
    return _ball->getNextLook();
}

CstBlock_sptr Map::getBlock(const JBTypes::vec3ui &pos) const {
    const std::string strPos = Block::positionToString(pos);
    return _blocksPositions->find(strPos) != _blocksPositions->end()
           ? _blocksPositions->at(strPos)
           : nullptr;
}

vecCstBlock_sptr Map::getBlocks() const {
    vecCstBlock_sptr cstBlocks;
    for (const auto& block : _blocks) {
        cstBlocks.push_back(block) ;
    }
    return cstBlocks;
}

CstBall_sptr Map::getBall() const {
    return _ball;
}

std::map<BlockDir , BlockDir> Map::createBlocksTeleportations() const {
    std::map<BlockDir , BlockDir> teleportationBlocks;
    std::map<JBTypes::Color, BlockDir> foundColors;
    for (const auto& block: _blocks) {
        for(const auto& special: block->getSpecials()) {
            const auto& color = special->getColor();
            if (foundColors.find(color) == foundColors.end() ){
                foundColors[color] = { block, special->direction() };
            } else {
                teleportationBlocks[foundColors[color]] = { block, special->direction() };
            }
        }
    }

    std::map<BlockDir , BlockDir> invertedTeleportationBlocks;
    for(const auto& origDest: teleportationBlocks) {
        invertedTeleportationBlocks[origDest.second] = origDest.first;
    }
    teleportationBlocks.insert(invertedTeleportationBlocks.begin(), invertedTeleportationBlocks.end());

    return teleportationBlocks;
}

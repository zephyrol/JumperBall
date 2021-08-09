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
    _blocksPositions(createBlockPositions()),
    _blocksToUpdate(getBlocksToUpdate()),
    _ball(std::move(mapInfo.ball)),
    _width(mapInfo.width),
    _height(mapInfo.height),
    _depth(mapInfo.depth),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _blocksUpdating([this] (size_t blockNumber) {
                            _blocksToUpdate.at(blockNumber)->update(
                                    _timeInteractions
                                );
                        }, _blocksToUpdate.size()),
    _timeInteractions(),
    _nextBlockGetter(),
    _turnBackMovement()
{
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

void Map::interaction() {
    _timeInteractions = JBTypesMethods::getTimePointMSNow();
    _blocksUpdating.runTasks();
    _blocksUpdating.waitTasks();


    /*const Map::Effect effect = _map.interaction(_currentSide, get3DPosition(), getRadius());
    if (effect == Map::Effect::Burst) {
        if (_stateOfLife != StateOfLife::Bursting) {
            _stateOfLife = StateOfLife::Bursting;
            setTimeLifeNow();
        }
    }*/
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

Map::nextBlockInformation Map::getNextBlockInfo() const {

    const JBTypes::Dir lookTowards = _ball->lookTowards();
    const JBTypes::Dir currentSide = _ball->currentSide();
    const std::array <int, 12> offsetsNextBlocks = _nextBlockGetter.evaluate(
        {currentSide, lookTowards}
    );

    const auto& position = _ball->getPosition();

    const auto& getNeighbour = [&position, offsetsNextBlocks](size_t n) -> JBTypes::vec3ui  {
        const size_t offset = 3 * n;
        return {
            position.at(0) + offsetsNextBlocks.at(offset),
            position.at(1) + offsetsNextBlocks.at(offset + 1),
            position.at(2) + offsetsNextBlocks.at(offset + 2)
        };
    };

    const auto inFrontOf = getNeighbour(0);
    const auto left = getNeighbour(1);
    const auto right = getNeighbour(2);
    const auto above = getNeighbour(3);

    const CstBlock_sptr& blockAbove = getBlock(above);
    const CstBlock_sptr& blockInFrontOf = getBlock(inFrontOf);
    const CstBlock_sptr& blockLeft = getBlock(left);
    const CstBlock_sptr& blockRight = getBlock(right);


    Map::nextBlockInformation nextBlock{};

    if (blockAbove && blockAbove->isExists()) {
        nextBlock.pos = above;
        nextBlock.nextLocal = NextBlockLocal::Above;

        const JBTypes::Dir lookTowardsBeforeMovement = lookTowards;
        nextBlock.nextLook = currentSide;
        nextBlock.nextSide = _turnBackMovement.evaluate({ lookTowardsBeforeMovement });
    } else if (blockInFrontOf && blockInFrontOf->isExists()) {
        nextBlock.pos = inFrontOf;
        nextBlock.nextLocal = NextBlockLocal::InFrontOf;
        nextBlock.nextLook = lookTowards;
        nextBlock.nextSide = currentSide;
    } else if (
            (!blockLeft || !blockLeft->isExists()) &&
            (!blockRight || !blockRight->isExists())
            ) {
        nextBlock.pos = position;
        nextBlock.nextLocal = NextBlockLocal::Same;
        const JBTypes::Dir sideBeforeMovement = currentSide;
        nextBlock.nextSide = lookTowards;
        nextBlock.nextLook = _turnBackMovement.evaluate({ sideBeforeMovement });
    } else if (_ball->stateOfLife() == Ball::StateOfLife::Sliding) {
        nextBlock.pos = inFrontOf;
        nextBlock.nextLocal = NextBlockLocal::InFrontOf;
        nextBlock.nextLook = lookTowards;
        nextBlock.nextSide = currentSide;
    } else {
        nextBlock.pos = position;
        nextBlock.nextLocal = NextBlockLocal::None;
        nextBlock.nextSide = currentSide;;
        nextBlock.nextLook = lookTowards;
    }
    return nextBlock;
}

JBTypes::vec3f Map::getNextLook() const {
    return JBTypesMethods::directionAsVector(getNextBlockInfo().nextLook);
}

JBTypes::vec3ui Map::getBlockCoords (size_t index,
                                     unsigned int width,
                                     unsigned int depth) {
    const auto uIntIndex = static_cast <const unsigned int>(index);
    const unsigned int widthDepth = width * depth;
    return { uIntIndex % width, uIntIndex / widthDepth, (uIntIndex % widthDepth) / width };
}

std::shared_ptr <const std::vector <unsigned int> > Map::intersectBlock (float x, float y, float z) const {
    const JBTypes::vec3f sideVec = _ball->currentSideAsVector();

    const float offsetBlockPosition = _ball->getRadius();
    const float xIntersectionUnder = x - sideVec.x * offsetBlockPosition;
    const float yIntersectionUnder = y - sideVec.y * offsetBlockPosition;
    const float zIntersectionUnder = z - sideVec.z * offsetBlockPosition;

    const auto xInteger = static_cast <unsigned int>(xIntersectionUnder);
    const auto yInteger = static_cast <unsigned int>(yIntersectionUnder);
    const auto zInteger = static_cast <unsigned int>(zIntersectionUnder);

    const CstBlock_sptr& block = getBlock({ xInteger, yInteger, zInteger });

    return (block && block->isExists())
           ? std::make_shared <const std::vector <unsigned int> >(
               std::initializer_list <unsigned int>({ xInteger, yInteger, zInteger })
           )
           : nullptr;
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

CstBlock_sptr Map::getBlock(const JBTypes::vec3ui &pos) const {
    const std::string strPos = positionToString(pos);
    return _blocksPositions.find(strPos) != _blocksPositions.end()
           ? _blocksPositions.at(strPos)
           : nullptr;
}

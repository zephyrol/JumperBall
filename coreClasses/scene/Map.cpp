/*
 * File: Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#include "Map.h"

Map::Map(Map::MapInfo &&mapInfo) :
    _blocks(std::move(mapInfo.blocks)),
    _blocksPositions(
        std::make_shared<const std::unordered_map<std::string, Block_sptr> >(createBlockPositions())
    ),
    _blocksToUpdate(std::make_shared<const vecBlock_sptr>(getBlocksToUpdate())),
    _blocksTeleportations(
        std::make_shared<const std::map<BlockDir, BlockDir> >(createBlocksTeleportations())
    ),
    _ball(std::move(mapInfo.ball)),
    _width(mapInfo.width),
    _height(mapInfo.height),
    _depth(mapInfo.depth),
    _timeToFinish(static_cast<float>(mapInfo.timeToFinish)),
    _remainingTime(_timeToFinish),
    _nbOfKeys(mapInfo.nbOfKeys),
    _isExitUnlocked(false) {
    _ball->setBlockPositions(_blocksPositions);
    _ball->setBlockWithInteractions(_blocksToUpdate);
    _ball->setBlockTeleportations(_blocksTeleportations);
    _ball->updateMovements();
}

JBTypes::vec3ui Map::stringToPosition(const std::string &stringPosition) {
    const auto &firstComma = std::find(stringPosition.begin(), stringPosition.end(), ',');
    const auto &secondComma = std::find(firstComma + 1, stringPosition.end(), ',');
    return {
        static_cast<unsigned int>(std::stoi(std::string(stringPosition.begin(), firstComma))),
        static_cast<unsigned int>(std::stoi(std::string(firstComma + 1, secondComma))),
        static_cast<unsigned int>(std::stoi(std::string(secondComma + 1, stringPosition.end())))
    };
}

std::unordered_map<std::string, Block_sptr> Map::createBlockPositions() const {
    std::unordered_map<std::string, Block_sptr> positions;
    for (const auto &block: _blocks) {
        const auto position = block->position();
        positions[Block::positionToString(position)] = block;
    }
    return positions;
}

std::vector<Block_sptr> Map::getBlocksToUpdate() const {
    std::vector<Block_sptr> blocksWithInteraction;
    for (const auto &block: _blocks) {
        if (block->hasInteraction()) {
            blocksWithInteraction.push_back(block);
        }
    }
    return blocksWithInteraction;
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

std::string Map::update(const Ball::ActionRequest &action) {

    _ball->update(action);

    for (const auto &block: *_blocksToUpdate) {
        block->update();
    }

    constexpr decltype(_ball->numberOfClocks()) clockTimeBonus = 30;
    _remainingTime = _timeToFinish + static_cast<float>(
        _ball->numberOfClocks() * clockTimeBonus
    ) - _ball->getInGameChronometer()->getTime();

    if (isBallsOut() || _remainingTime < 0.f) {
        _ball->die();
    }

    if (!_isExitUnlocked && _ball->numberOfKeys() >= _nbOfKeys) {
        for (const auto &block: _blocks) {
            block->unlockExit();
        }
        _isExitUnlocked = true;
    }

    return UpdateOutput::combineUpdateOutputs(_ball->retrieveUpdateOutput());
}

JBTypes::vec3ui Map::getBlockCoords(size_t index, unsigned int width, unsigned int depth) {
    const auto uIntIndex = static_cast <unsigned int>(index);
    const unsigned int widthDepth = width * depth;
    return {uIntIndex % width, uIntIndex / widthDepth, (uIntIndex % widthDepth) / width};
}

bool Map::isBallsOut() const {

    const auto ballPosition = _ball->get3DPosition();
    constexpr float thresholdOut = 5.f;
    // TODO: get ball 3D position
    return (
        ballPosition.x<-thresholdOut || ballPosition.x>(static_cast <float>(_width) + thresholdOut) ||
        ballPosition.y<-thresholdOut || ballPosition.y>(static_cast <float>(_height) + thresholdOut) ||
        ballPosition.z<-thresholdOut || ballPosition.z>(static_cast <float>(_depth) + thresholdOut)
    );
}

JBTypes::vec3f Map::getNextLook() const {
    return _ball->getNextLook();
}

CstBlock_sptr Map::getBlock(const JBTypes::vec3ui &pos) const {
    const std::string strPos = Block::positionToString(pos);
    const auto blockIterator = _blocksPositions->find(strPos);
    return blockIterator != _blocksPositions->end()
           ? blockIterator->second
           : nullptr;
}

vecCstBlock_sptr Map::getBlocks() const {
    vecCstBlock_sptr cstBlocks;
    for (const auto &block: _blocks) {
        cstBlocks.push_back(block);
    }
    return cstBlocks;
}

CstBall_sptr Map::getBall() const {
    return _ball;
}

std::map<BlockDir, BlockDir> Map::createBlocksTeleportations() const {
    std::map<BlockDir, BlockDir> teleportationBlocks{};
    std::map<JBTypes::Color, BlockDir> foundColors{};
    for (const auto &block: _blocks) {
        for (const auto &special: block->getSpecials()) {
            const auto &color = special->getColor();
            if (foundColors.find(color) == foundColors.end()) {
                foundColors[color] = {block, special->direction()};
            } else {
                teleportationBlocks[foundColors[color]] = {block, special->direction()};
            }
        }
    }

    std::map<BlockDir, BlockDir> invertedTeleportationBlocks;
    for (const auto &origDest: teleportationBlocks) {
        invertedTeleportationBlocks[origDest.second] = origDest.first;
    }
    teleportationBlocks.insert(invertedTeleportationBlocks.begin(), invertedTeleportationBlocks.end());

    return teleportationBlocks;
}

bool Map::gameIsLost() const {
    return _ball->stateOfLife() == Ball::StateOfLife::Dead;
}

bool Map::gameIsWon() const {
    return _ball->stateOfLife() == Ball::StateOfLife::Winner;
}

float Map::remainingTime() const {
    return _remainingTime;
}

unsigned int Map::getCurrentNumberOfKeys() const {
    return _ball->numberOfKeys();
}

unsigned int Map::getMaxNumberOfKeys() const {
    return _nbOfKeys;
}

unsigned int Map::getCurrentNumberOfCoins() const {
    return _ball->numberOfCoins();
}

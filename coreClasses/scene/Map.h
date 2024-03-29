/*
 * File: Map.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef MAP_H
#define MAP_H

#include "Ball.h"
#include "ItemsContainer.h"
#include <fstream>

class Map;

using Map_sptr = std::shared_ptr<Map>;
using CstMap_sptr = std::shared_ptr<const Map>;
using vecCstMap_sptr = std::vector<CstMap_sptr>;
using vecMap_sptr = std::vector<Map_sptr>;

class Map: public ItemsContainer {
public:

    struct MapInfo {
        unsigned int width;
        unsigned int height;
        unsigned int depth;
        unsigned int nbOfKeys;
        unsigned int nbOfCoins;
        unsigned int timeToFinish;
        Ball_sptr ball;
        vecBlock_sptr blocks;
    };

    explicit Map(MapInfo &&mapInfo);

    unsigned int width() const;

    unsigned int height() const;

    unsigned int depth() const;

    JBTypes::vec3f getCenterMap() const;

    float getLargestSize() const;

    std::string update(const Ball::ActionRequest &action);

    CstBall_sptr getBall() const;

    CstBlock_sptr getBlock(const JBTypes::vec3ui &pos) const;

    static JBTypes::vec3ui getBlockCoords(size_t index, unsigned int width, unsigned int depth);

    JBTypes::vec3f getNextLook() const;

    bool gameIsLost() const;

    bool gameIsWon() const;

    float remainingTime() const;

    unsigned int getCurrentNumberOfKeys() const override;

    unsigned int getMaxNumberOfKeys() const override;

    unsigned int getCurrentNumberOfCoins() const override;

private:

    static JBTypes::vec3ui stringToPosition(const std::string &stringPosition);

    bool isBallsOut() const;

    std::unordered_map<std::string, Block_sptr> createBlockPositions() const;

    std::vector<Block_sptr> getBlocksToUpdate() const;

    std::map<BlockDir, BlockDir> createBlocksTeleportations() const;

    const vecBlock_sptr _blocks;
    const std::shared_ptr<const std::unordered_map<std::string, Block_sptr> > _blocksPositions;
    const std::shared_ptr<const vecBlock_sptr> _blocksToUpdate;
    const std::shared_ptr<const std::map<BlockDir, BlockDir> > _blocksTeleportations;

    const Ball_sptr _ball;

    const unsigned int _width;
    const unsigned int _height;
    const unsigned int _depth;
    const float _timeToFinish;
    float _remainingTime;

    const unsigned int _nbOfKeys;

    bool _isExitUnlocked;


public :
    vecCstBlock_sptr getBlocks() const;
};

#endif /* MAP_H */

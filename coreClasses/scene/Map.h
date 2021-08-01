/*
 * File: Map.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef MAP_H
#define MAP_H
#include "blocks/Block.h"
#include "system/ParallelTask.h"
#include <fstream>

class Map {
public:

// ----------- TYPES -----------//

enum class Effect { Nothing, Burst, Burn, Slide, Jump };

struct MapInfo { 
    unsigned int width;
    unsigned int height;
    unsigned int depth;
    unsigned int beginX;
    unsigned int beginY;
    unsigned int beginZ;
    std::vector <Block_sptr> blocks;
};

explicit Map(MapInfo&& mapInfo);

unsigned int beginX() const;
unsigned int beginY() const;
unsigned int beginZ() const;

CstBlock_sptr getBlock(int x, int y, int z) const;

unsigned int width() const;
unsigned int height() const;
unsigned int depth() const;
JBTypes::vec3f getCenterMap() const;
float getLargestSize() const;

float getTimeSinceCreation() const;

// TODO: move to special
std::map <JBTypes::Color, bool> createSpecialStates() const;

Map::Effect interaction(const JBTypes::Dir& direction, const JBTypes::vec3f& boundingSpherePosition, float radius);

void switchColor(const JBTypes::Color& color);
Block_sptr getBlock(int x, int y, int z);

static JBTypes::vec3ui getBlockCoords(size_t index, unsigned int width, unsigned int depth);

private:

static std::string positionToString(const JBTypes::vec3ui& position);
static JBTypes::vec3ui stringToPosition(const std::string& stringPosition);

std::map<std::string,Block_sptr> createBlockPositions() const;
std::vector<Block_sptr> createBlocksWithInteraction() const;
std::vector<Block_sptr> createBlocksWithItems() const;
std::vector<Block_sptr> createBlocksWithEnemies() const;
std::vector<Block_sptr> createBlocksWithSpecials() const;

const vecBlock_sptr _blocks;
const std::map<std::string,Block_sptr> _blocksPositions;
const vecBlock_sptr _blocksWithInteraction;
const vecBlock_sptr _blocksWithItems;
const vecBlock_sptr _blocksWithEnemies;
const vecBlock_sptr _blocksWithSpecials;

const unsigned int _width;
const unsigned int _height;
const unsigned int _depth;
const unsigned int _beginX;
const unsigned int _beginY;
const unsigned int _beginZ;
const JBTypes::timePointMs _creationTime;

ParallelTask <Block::Effect> _blocksInteractions;
ParallelTask <void> _itemsInteractions;
ParallelTask <Block::Effect> _enemiesInteractions;

JBTypes::Dir _dirBallInteractions;
JBTypes::vec3f _posBallInteractions;
float _radiusInteractions;
JBTypes::timePointMs _timeInteractions;

};

#endif /* MAP_H */

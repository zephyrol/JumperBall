/*
 * File: Map.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef MAP_H
#define MAP_H
#include "blocks/Block.h"
#include "blocks/BaseBlock.h"
#include "blocks/FireBlock.h"
#include "blocks/IceBlock.h"
#include "blocks/SharpBlock.h"
#include "blocks/BrittleBlock.h"
#include "blocks/JumpBlock.h"
#include "blocks/GhostBlock.h"
#include "items/Key.h"
#include "items/Coin.h"
#include "items/Clock.h"
#include "enemies/DarkBall.h"
#include "enemies/ThornBall.h"
#include "enemies/Laser.h"
#include "special/SwitchButton.h"
#include "special/Teleporter.h"
#include "system/ParallelTask.h"
#include <fstream>

class Map {
public:

// ----------- TYPES -----------//

enum class Effect { Nothing, Burst, Burn, Slide, Jump };

struct MapInfo { unsigned int width;
                 unsigned int height;
                 unsigned int depth;
                 unsigned int beginX;
                 unsigned int beginY;
                 unsigned int beginZ;
                 std::vector <Block_sptr> blocks;
};

Map(MapInfo&& mapInfo);

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


JBTypes::vec3ui getBlockCoords(size_t index) const;
size_t getIndex(const JBTypes::vec3ui& coords) const;
std::vector <size_t> getBlocksWithItems() const;
const std::map <JBTypes::Color, Map::TeleportersInfo>& getBlocksTeleporters() const;

std::map <JBTypes::Color, bool> createSpecialStates() const;
const std::map <JBTypes::Color, bool>& getSpecialStates() const;

Map::Effect interaction(const JBTypes::Dir& ballDir, const JBTypes::vec3f& posBall, float radius);

void switchColor(const JBTypes::Color& color);
Block_sptr getBlock(int x, int y, int z);
Block_sptr getBlock(size_t index);

static JBTypes::vec3ui getBlockCoords(size_t index, unsigned int width, unsigned int depth);

private:
std::map <JBTypes::Color, Map::TeleportersInfo> createBlocksTeleporters() const;

const std::vector <Block_sptr> _blocks;
const std::map<std::string,Block_sptr> blocksPositions;
const std::vector <Block_sptr> _blocksWithInteraction;
std::vector <size_t> _blocksWithItemsIndices;
std::map <JBTypes::Color, bool> _specialsState;

const unsigned int _width;
const unsigned int _height;
const unsigned int _depth;
const unsigned int _beginX;
const unsigned int _beginY;
const unsigned int _beginZ;
const JBTypes::timePointMs _creationTime;

ParallelTask <Block::Effect> _blocksInteractions;
ParallelTask <void> _itemsInteractions;
ParallelTask <Enemy::Effect> _enemiesInteractions;

JBTypes::Dir _dirBallInteractions;
JBTypes::vec3f _posBallInteractions;
float _radiusInteractions;
JBTypes::timePointMs _timeInteractions;

};

#endif /* MAP_H */

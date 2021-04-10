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
#include "objects/Key.h"
#include "objects/Coin.h"
#include "objects/Clock.h"
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
enum class BlockTypes { None, Base, Fire, Ice, Sharp,
                        Brittle, Jump, Ghost };

enum class EnemyTypes { Laser, ThornBall, DarkBall };

enum class SpecialTypes { SwitchButton, Teleporter };

enum class Effect { Nothing, Burst, Burn, Slide, Jump };

struct BlockInfo { size_t index;
                   BlockTypes type; };

struct EnemyInfo {
    size_t index = 0;
    std::shared_ptr <Enemy> enemy = nullptr;
    EnemyTypes type = EnemyTypes::Laser;
};

struct SpecialInfo {
    size_t index = 0;
    std::shared_ptr <Special> special = nullptr;
    SpecialTypes type = SpecialTypes::SwitchButton;
};

struct TeleportersInfo {
    std::pair <size_t, size_t> coupleIndices;
    std::pair <JBTypes::Dir, JBTypes::Dir> coupleDirections;
};

struct MapInfo { unsigned int width;
                 unsigned int height;
                 unsigned int deep;
                 unsigned int beginX;
                 unsigned int beginY;
                 unsigned int beginZ;
                 std::vector <std::shared_ptr <Block> > blocks;
                 std::vector <BlockInfo> blocksInfo;
                 std::vector <EnemyInfo> enemiesInfo;
                 std::vector <SpecialInfo> specialInfo; };

Map(MapInfo&& mapInfo);

unsigned int beginX() const;
unsigned int beginY() const;
unsigned int beginZ() const;

std::shared_ptr <const Block> getBlock(int x, int y, int z) const;
std::shared_ptr <const Block> getBlock(size_t index) const;

unsigned int width() const;
unsigned int height() const;
unsigned int deep() const;
JBTypes::vec3f getCenterMap() const;
float getLargestSize() const;

float getTimeSinceCreation() const;


JBTypes::vec3ui getBlockCoords(size_t index) const;
size_t getIndex(const JBTypes::vec3ui& coords) const;
BlockTypes getType(const JBTypes::vec3ui& position) const;
const std::vector <BlockInfo>& blocksInfo() const;

std::vector <BlockInfo> getBlocksWithInteraction() const;
std::vector <size_t> getBlocksWithObjects() const;
const std::map <JBTypes::Color, Map::TeleportersInfo>& getBlocksTeleporters() const;

std::map <JBTypes::Color, bool> createSpecialStates() const;
const std::map <JBTypes::Color, bool>& getSpecialStates() const;


const std::vector <EnemyInfo>& getEnemiesInfo() const;
const std::vector <SpecialInfo>& getSpecialInfo() const;

Map::Effect interaction(const JBTypes::Dir& ballDir, const JBTypes::vec3f& posBall, float radius);

void switchColor(const JBTypes::Color& color);
std::shared_ptr <Block> getBlock(int x, int y, int z);
std::shared_ptr <Block> getBlock(size_t index);

static JBTypes::vec3ui getBlockCoords(size_t index, unsigned int width, unsigned int deep);

private:
std::map <JBTypes::Color, Map::TeleportersInfo> createBlocksTeleporters() const;

// --------ATTRIBUTES-----------//
std::vector <std::shared_ptr <Block> > _blocks;
std::vector <BlockInfo> _blocksInfo;
std::vector <BlockInfo> _blocksWithInteractionInfo;
std::vector <size_t> _blocksWithObjectsIndices;

std::vector <EnemyInfo> _enemies;

std::vector <SpecialInfo> _specials;
std::map <JBTypes::Color, TeleportersInfo>
_blocksTeleporters;
std::map <JBTypes::Color, bool> _specialsState;

unsigned int _width;
unsigned int _height;
unsigned int _deep;
unsigned int _beginX;
unsigned int _beginY;
unsigned int _beginZ;
const JBTypes::timePointMs _creationTime;

// Multithreading
ParallelTask <Block::Effect> _blocksInteractions;
ParallelTask <void> _objectsInteractions;
ParallelTask <Enemy::Effect> _enemiesInteractions;

JBTypes::Dir _dirBallInteractions;
JBTypes::vec3f _posBallInteractions;
float _radiusInteractions;
JBTypes::timePointMs _timeInteractions;
// --------------

static unsigned int nbMaps;


};

#endif /* MAP_H */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.cpp
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
    _blocksWithObjectsInfo(getBlocksWithObjects()),
    _enemies(std::move(mapInfo.enemiesInfo)),
    _width (std::move(mapInfo.width)),
    _height (std::move(mapInfo.height)),
    _deep (std::move(mapInfo.deep)),
    _beginX (std::move(mapInfo.beginX)),
    _beginY (std::move(mapInfo.beginY)),
    _beginZ (std::move(mapInfo.beginZ)),
    _timeCreation(std::chrono::system_clock::now()),
    _blocksInteractions([this](size_t blockNumber) {
         const std::shared_ptr<Block>& block =
            getBlock(_blocksWithInteractionInfo.at(blockNumber).index);
         return block->interaction(
                     _dirBallInteractions,
                     _timeInteractions,
                     _posBallInteractions,
                     getBlockCoords
                     (_blocksWithInteractionInfo.at(blockNumber).index));
    },_blocksWithInteractionInfo.size()),
    _objectsInteractions([this](size_t blockNumber) {
         const std::shared_ptr<Block>& block =
            getBlock(_blocksWithObjectsInfo.at(blockNumber).index);
         if (block->hasObjects()) {
            block->catchObject(
                getBlockCoords(_blocksWithObjectsInfo.at(blockNumber).index),
                _posBallInteractions,_radiusInteractions );
         }
    },_blocksWithObjectsInfo.size()),
    _dirBallInteractions(JBTypes::Dir::North),
    _posBallInteractions({0.f,0.f,0.f}),
    _radiusInteractions(0.f),
    _timeInteractions() {
}


std::shared_ptr<Block> Map::getBlock(int x, int y, int z){
    std::shared_ptr<const Block> constBlock =
            static_cast<const Map&>(*this).getBlock(x,y,z);

    return std::const_pointer_cast<Block> (constBlock);
}

std::shared_ptr<Block> Map::getBlock(size_t index)
{
    const std::shared_ptr<const Block>& constBlock =
            static_cast<const Map&>(*this).getBlock(index);

    return std::const_pointer_cast<Block> (constBlock);
}

std::vector<Map::BlockInfo> Map::getBlocksWithInteraction() const
{
    std::vector<Map::BlockInfo> blocksWithInteraction;
    for (unsigned int i = 0 ; i < _blocksInfo.size(); ++i) {
        const std::shared_ptr<const Block>& block =
                getBlock(_blocksInfo.at(i).index);
        if (block->hasInteraction()) {
            blocksWithInteraction.push_back(_blocksInfo.at(i));
        }
    }
    return blocksWithInteraction;
}

std::vector<Map::BlockInfo> Map::getBlocksWithObjects() const
{
    std::vector<Map::BlockInfo> blocksWithObjects;
    for (unsigned int i = 0 ; i < _blocksInfo.size(); ++i) {
        const std::shared_ptr<const Block>& block =
                getBlock(_blocksInfo.at(i).index);
        if (block->hasObjects()) {
            blocksWithObjects.push_back(_blocksInfo.at(i));
        }
    }
    return blocksWithObjects;
}

std::shared_ptr<const Block> Map::getBlock(int x, int y, int z) const {
    std::shared_ptr<const Block> block;
    if (x >= static_cast<int>(_width) ||
            y >= static_cast<int>(_height) ||
            z >= static_cast<int>(_deep) ||
            x < 0 || y < 0 || z < 0 )
        block = nullptr;
    else {
        size_t index = _width * (z + y * _deep) + x;
        block = _blocks.at(index);
    }
    return block;
}

std::shared_ptr<const Block> Map::getBlock(size_t index) const {
    return _blocks.at(index);
}

std::array<unsigned int, 3> Map::getBlockCoords(size_t index) const {
    return getBlockCoords(index, _width, _deep);
    /*unsigned int uIntIndex = static_cast<unsigned int> (index);
    const unsigned int widthDeep = _width * _deep;
    return { uIntIndex % _width,
             uIntIndex / widthDeep,
             (uIntIndex % widthDeep) / _width };*/
}

size_t Map::getIndex(const std::array<unsigned int, 3>& coords) const {
    return _width * (coords.at(2) + coords.at(1)* _deep) + coords .at(0);
}

Map::BlockTypes Map::getType(const std::array<unsigned int, 3>& position) const{
    const size_t index = getIndex(position);
    for (const Map::BlockInfo& blockInfo: _blocksInfo) {
        if (blockInfo.index == index )
        return blockInfo.type;
    }
    return Map::BlockTypes::None;
}

const std::vector<Map::BlockInfo>& Map::blocksInfo() const {
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

unsigned int Map::beginX() const {
    return _beginX;
}

unsigned int Map::beginY() const {
    return _beginY;
}

unsigned int Map::beginZ() const {
    return _beginZ;
}

std::chrono::time_point<std::chrono::system_clock> Map::timeCreation() const {
    return _timeCreation;
}

Block::Effect Map::interaction( const JBTypes::Dir& ballDir,
                                const JBTypes::vec3f& posBall,
                                float radius) {
    
    _dirBallInteractions = ballDir;
    _posBallInteractions = posBall;
    _radiusInteractions = radius;
    _timeInteractions = JBTypesMethods::getTimePointMSNow();

    _blocksInteractions.runTasks();
    _objectsInteractions.runTasks();

    std::shared_ptr<std::vector<Block::Effect> > effects =
        _blocksInteractions.waitTasks();
    Block::Effect finalEffect = Block::Effect::Nothing;
    for (Block::Effect& effect : *effects) {
        if (effect != Block::Effect::Nothing){
            finalEffect = effect;
        }
    }
    _objectsInteractions.waitTasks();
    return finalEffect;
}


std::array<unsigned int, 3> Map::getBlockCoords(size_t index,
                                                unsigned int width,
                                                unsigned int deep) 
{
    unsigned int uIntIndex = static_cast<unsigned int> (index);
    const unsigned int widthDeep = width * deep;
    return { uIntIndex % width,
             uIntIndex / widthDeep,
             (uIntIndex % widthDeep) / width };
}


const std::vector<Map::EnemyInfo>& Map::getEnemiesInfo() const {
    return _enemies;
}

unsigned int Map::nbMaps = 0;

/*
 * File:   MapState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */

#include "MapState.h"

MapState::MapState(const Map &map):
    _map(map),
    _width(map.width()),
    _height(map.height()),
    _deep(map.deep()),
    _blockStates(genBlockStates()),
    _enemiesStates(genEnemiesStates()),
    _specialsStates(genSpecialsStates())
{
}

void MapState::update()
{
    for(const std::shared_ptr<BlockState>& blockState : _blockStates){
        if(blockState) {
            blockState->update();
        }
    }
    for(const std::shared_ptr<EnemyState>& enemyState: _enemiesStates){
        if(enemyState) {
            enemyState->update();
        }
    }
    for(const std::shared_ptr<SpecialState>& specialState: _specialsStates){
        if(specialState) {
            specialState->update();
        }
    }
}

unsigned int MapState::width() const
{
    return _width;
}

unsigned int MapState::height() const
{
    return _height;
}

unsigned int MapState::deep() const
{
    return _deep;
}

Map::BlockTypes MapState::getType(const std::array<unsigned int, 3>& position)
const {
    return _map.getType(position);
}

const std::vector<Map::BlockInfo>& MapState::blocksInfo() const {
    return _map.blocksInfo();
}

std::array<unsigned int, 3> MapState::getBlockCoords(size_t index) const {
    return _map.getBlockCoords(index);
}

const std::vector<std::shared_ptr<BlockState> > &MapState::blockStates()
const
{
   return _blockStates;
}

const std::vector<std::shared_ptr<EnemyState> > &MapState::enemiesStates()
const {
    return _enemiesStates;
}

std::vector<std::shared_ptr<BlockState> > MapState::genBlockStates() const
{
   const unsigned int nbBlocks =_map.width() * _map.height() * _map.deep();
   std::vector<std::shared_ptr<BlockState> >BlockStates {nbBlocks, nullptr};

    for (size_t i = 0 ; i < _map.blocksInfo().size(); ++i) {
        std::shared_ptr<const Block> block =
                _map.getBlock(_map.blocksInfo().at(i).index);
        if ( block ) {
            BlockStates.at(_map.blocksInfo().at(i).index) =
                    std::make_shared<BlockState>(*block);
        }
    }
   return BlockStates;
}

std::vector<std::shared_ptr<EnemyState> > MapState::genEnemiesStates()
const
{
    std::vector<std::shared_ptr<EnemyState> > enemiesStates;
    for (const Map::EnemyInfo& enemyInfo : _map.getEnemiesInfo()) {
        enemiesStates.push_back(
            std::make_shared<EnemyState>(*enemyInfo.enemy, enemyInfo.type)
        );
    }
    return enemiesStates;
}

std::vector<std::shared_ptr<SpecialState> > MapState::genSpecialsStates() 
const {
    std::vector<std::shared_ptr<SpecialState> > specialsStates;
    for (const Map::SpecialInfo& specialInfo: _map.getSpecialInfo()) {
        specialsStates.push_back(
            std::make_shared<SpecialState>(*specialInfo.special, specialInfo.type)
        );
    }
    return specialsStates;
}

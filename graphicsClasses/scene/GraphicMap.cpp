/*
 * File:   GraphicMap.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */

#include "GraphicMap.h"

GraphicMap::GraphicMap(const Map &map):
    _map(map),
    _width(map.width()),
    _height(map.height()),
    _deep(map.deep()),
    _graphicBlocks(genGraphicBlocks()),
    _graphicEnemies(genGraphicEnemies())
{
}


void GraphicMap::update()
{
    _width = _map.width();
    _height = _map.height();
    _deep = _map.deep();
    for(const std::shared_ptr<GraphicBlock>& graphicBlock : _graphicBlocks){
        if(graphicBlock) {
            graphicBlock->update();
        }
    }
}

unsigned int GraphicMap::width() const
{
    return _width;
}

unsigned int GraphicMap::height() const
{
    return _height;
}

unsigned int GraphicMap::deep() const
{
    return _deep;
}


Map::BlockTypes GraphicMap::getType(
                                    const std::array<unsigned int, 3>& position) const {
    return _map.getType(position);
}


const std::vector<Map::BlockInfo>& GraphicMap::blocksInfo() const {
    return _map.blocksInfo();
}

std::array<unsigned int, 3> GraphicMap::getBlockCoords(size_t index) const {
    return _map.getBlockCoords(index);
}

const std::vector<std::shared_ptr<GraphicBlock> > &GraphicMap::graphicBlocks()
const
{
   return _graphicBlocks;
}

const std::vector<std::shared_ptr<GraphicEnemy> > &GraphicMap::graphicEnemies()
const {
    return _graphicEnemies;
}

std::vector<std::shared_ptr<GraphicBlock> > GraphicMap::genGraphicBlocks() const
{
   const unsigned int nbBlocks =_map.width() * _map.height() * _map.deep();
   std::vector<std::shared_ptr<GraphicBlock> >graphicBlocks {nbBlocks, nullptr};

    for (size_t i = 0 ; i < _map.blocksInfo().size(); ++i) {
        std::shared_ptr<const Block> block =
                _map.getBlock(_map.blocksInfo().at(i).index);
        if ( block ) {
            graphicBlocks.at(_map.blocksInfo().at(i).index) =
                    std::make_shared<GraphicBlock>(*block);
        }
    }
   return graphicBlocks;
}

std::vector<std::shared_ptr<GraphicEnemy> > GraphicMap::genGraphicEnemies()
const
{
    std::vector<std::shared_ptr<GraphicEnemy> > graphicEnemies;
    for (const Map::EnemyInfo& enemyInfo : _map.getEnemiesInfo()) {
        graphicEnemies.push_back(
            std::make_shared<GraphicEnemy>(*enemyInfo.enemy)
        );
    }
    return graphicEnemies;
}

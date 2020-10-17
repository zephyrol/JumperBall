/*
 * File:   StarState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef MAPSTATE_H
#define MAPSTATE_H
#include "scene/Map.h"
#include "scene/blocks/BlockState.h"
#include "scene/enemies/EnemyState.h"

class MapState
{
public:
    MapState (const Map& map);
    void update();

    unsigned int width() const;
    unsigned int height() const;
    unsigned int deep() const;
    Map::BlockTypes getType( const std::array<unsigned int, 3>& position
                            ) const;
    const std::vector<Map::BlockInfo>& blocksInfo() const;
    std::array<unsigned int, 3> getBlockCoords(size_t index) const;
    const std::vector<std::shared_ptr<BlockState> > &BlockStates() const;
    const std::vector<std::shared_ptr<EnemyState> > &enemiesStates() const;

private:
    const Map&                            _map;
    unsigned int                          _width;
    unsigned int                          _height;
    unsigned int                          _deep;
    std::vector<std::shared_ptr<BlockState> >
                                          _BlockStates;
    std::vector<std::shared_ptr<EnemyState> >
                                          _enemiesStates;

    std::vector<std::shared_ptr<BlockState> >   genBlockStates() const;
    std::vector<std::shared_ptr<EnemyState> >  genenemiesStates() const;
};

#endif // MAPSTATE

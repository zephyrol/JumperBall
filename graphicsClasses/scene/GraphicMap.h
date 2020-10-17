/*
 * File:   GraphicStar.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef GRAPHICMAP_H
#define GRAPHICMAP_H
#include <scene/Map.h>
#include "GraphicBlock.h"
#include "GraphicEnemy.h"

class GraphicMap
{
public:
    GraphicMap(const Map& map);
    void update();

    unsigned int width() const;
    unsigned int height() const;
    unsigned int deep() const;
    Map::BlockTypes getType( const std::array<unsigned int, 3>& position
                            ) const;
    const std::vector<Map::BlockInfo>& blocksInfo() const;
    std::array<unsigned int, 3> getBlockCoords(size_t index) const;
    const std::vector<std::shared_ptr<GraphicBlock> > &graphicBlocks() const;
    const std::vector<std::shared_ptr<GraphicEnemy> > &graphicEnemies() const;

private:
    const Map&                            _map;
    unsigned int                          _width;
    unsigned int                          _height;
    unsigned int                          _deep;
    std::vector<std::shared_ptr<GraphicBlock> >
                                          _graphicBlocks;
    std::vector<std::shared_ptr<GraphicEnemy> >
                                          _graphicEnemies;

    std::vector<std::shared_ptr<GraphicBlock> >   genGraphicBlocks() const;
    std::vector<std::shared_ptr<GraphicEnemy> >   genGraphicEnemies() const;
};

#endif // GRAPHICMAP_H

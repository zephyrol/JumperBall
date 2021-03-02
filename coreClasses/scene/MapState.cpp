/*
 * File: MapState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */

#include "MapState.h"

MapState::MapState(const Map& map):
    _map(map),
    _width(map.width()),
    _height(map.height()),
    _deep(map.deep()) {
}

void MapState::update() {
}

unsigned int MapState::width() const {
    return _width;
}

unsigned int MapState::height() const {
    return _height;
}

unsigned int MapState::deep() const {
    return _deep;
}

Map::BlockTypes MapState::getType (const JBTypes::vec3ui& position) const {
    return _map.getType(position);
}

const std::vector <Map::BlockInfo>& MapState::blocksInfo() const {
    return _map.blocksInfo();
}

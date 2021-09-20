/*
 * File: Block.cpp
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#include "Block.h"
#include <utility>

Block::Block(
    const JBTypes::vec3ui& position,
    const vecItem_sptr& items,
    const vecEnemy_sptr& enemies,
    const vecSpecial_sptr& specials,
    bool alwaysHasInteractions,
    bool isFixed):
    _position(position),
    _items(items),
    _cstItems(getCstItems()),
    _enemies(enemies),
    _cstEnemies(getCstEnemies()),
    _specials(specials),
    _cstSpecials(getCstSpecials()),
    _hasInteraction(alwaysHasInteractions || !items.empty() || !enemies.empty() || !specials.empty()),
    _isFixed(isFixed),
    _localScale{1.f, 1.f, 1.f },
    _localTranslation{0.f, 0.f, 0.f }
{
}

std::array <bool, 6> Block::faceInfo() const {
    return { false, false, false, false, false, false };
}

bool Block::isExists() const {
    return true;
}

const JBTypes::vec3f& Block::localScale() const {
    return _localScale;
}

const JBTypes::vec3f& Block::localTranslation() const {
    return _localTranslation;
}

const vecCstItem_sptr& Block::getItems() const {
    return _cstItems; 
}

const vecCstEnemy_sptr& Block::getEnemies() const {
    return _cstEnemies;
}

const vecCstSpecial_sptr& Block::getSpecials() const {
    return _cstSpecials; 
}

Block::Effect Block::interaction (const JBTypes::timePointMs& ) {
    return Block::Effect::Nothing;
}

bool Block::hasInteraction() const {
    return _hasInteraction;
}

void Block::update(const JBTypes::timePointMs& currentTime) {
    interaction(currentTime);
}

const bool& Block::isFixed() const {
    return _isFixed;
}

const JBTypes::vec3ui& Block::position() const {
    return _position;
}

SceneElement::StaticValues <JBTypes::vec3f> Block::getStaticVec3fValues() const {
    constexpr float offset = 0.5f;
    return {{
        static_cast <float>(_position.at(0)) + offset,
        static_cast <float>(_position.at(1)) + offset,
        static_cast <float>(_position.at(2)) + offset
    }};
}

SceneElement::DynamicValues <JBTypes::vec3f> Block::getDynamicVec3fs() const {
    return {{ "translation", _localTranslation }, { "scale", _localScale }};
}

SceneElement::GlobalState Block::getGlobalState() const {
    return isExists() 
           ? SceneElement::GlobalState::United
           : SceneElement::GlobalState::Separate;
}

vecCstItem_sptr Block::getCstItems() const {
    vecCstItem_sptr cstItems;
    for(const auto& item: _items){
        cstItems.push_back(item);
    }
    return cstItems;
}

vecCstEnemy_sptr Block::getCstEnemies() const{
    vecCstEnemy_sptr cstEnemies;
    for(const auto& enemy: _enemies){
        cstEnemies.push_back(enemy);
    }
    return cstEnemies;
}

vecCstSpecial_sptr Block::getCstSpecials() const {
    vecCstSpecial_sptr cstSpecials;
    for(const auto& special: _specials){
        cstSpecials.push_back(special);
    }
    return cstSpecials;
}

bool Block::mayDisappear() const {
    return false;
}

JBTypes::Color Block::getColor() const {
    return JBTypes::Color::None;
}

vecCstShape_sptr Block::getExtraShapes() const {
    return {};
}

std::map<CstItem_sptr ,vecCstShape_sptr > Block::getItemShapes() const {
    std::map<CstItem_sptr, vecCstShape_sptr > itemShapes {};
    for(const auto& item: _items) {
        itemShapes[item] = item->getShapes();
    }
    return itemShapes;
}


std::string Block::positionToString(const JBTypes::vec3ui& position) {
    return std::to_string(position.at(0)) + "," +
           std::to_string(position.at(1)) + "," +
           std::to_string(position.at(2));
}

Block::Effect Block::detectionEvent() {
}


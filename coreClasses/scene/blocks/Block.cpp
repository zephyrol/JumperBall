/*
 * File: Block.cpp
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#include "Block.h"

Block::Block(
    const JBTypes::vec3ui& position,
    const vecItem_sptr& items, 
    const vecEnemy_sptr& enemies,
    const vecSpecial_sptr& specials,
    bool alwaysHasInteractions ,
    bool isFixed):
    _position(position),
    _items(items),
    _cstItems(_items),
    _enemies(enemies),
    _cstEnemies(_enemies),
    _specials(specials),
    _cstSpecials(_specials),
    _hasInteraction(alwaysHasInteractions || items.size() > 0 || enemies.size() > 0 || specials.size() > 0),
    _isFixed(isFixed),
    _localScale{1.f, 1.f, 1.f},
    _localTranslation{0.f, 0.f, 0.f}
{
}

std::array <bool, 6> Block::faceInfo() const {
    return { false, false, false, false, false, false };
}

bool Block::isExists() const {
    return true;
}

Block::Effect Block::detectionEvent (const JBTypes::Dir&, const JBTypes::timePointMs&) {
    return Block::Effect::Nothing;
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

Block::Effect Block::interaction (
    const JBTypes::Dir&,
    const JBTypes::timePointMs&,
    const JBTypes::vec3f&
) {
    return Block::Effect::Nothing;
}

std::string Block::getBlockOptions() const {
    return "";
}

bool Block::hasInteraction() const {
    return _hasInteraction;
}

Block::Effect Block::update(
    const JBTypes::Dir& direction,
    const JBTypes::timePointMs& currentTime,
    const JBTypes::vec3f& boundingSpherePosition,
    float boundingSphereRadius
) {
    const auto interactionEffect = interaction(direction, currentTime, boundingSpherePosition);
}

void Block::update() {
}

void Block::catchItem (const JBTypes::vec3f& boundingSphereCenter, float boundingSphereRadius) {
    for (Item_sptr item : _items) {
        if (!item->isGotten()) {
            item->catchingTest(boundingSphereCenter, boundingSphereRadius);
        }
    }
}

Block::Effect Block::updateEnemies(const JBTypes::vec3f& boundingSphereCenter, float boundingSphereRadius) {
    Block::Effect effect;
    for (Enemy_sptr enemy: _enemies) {
        const auto enemyEffect = enemy->update(boundingSphereCenter, boundingSphereRadius);
        if(enemyEffect == Enemy::Effect::Burst) {
            effect = Block::Effect::Burst;
        }
    }
    return effect;
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
        static_cast <float>(_position.at(0) + offset),
        static_cast <float>(_position.at(1) + offset),
        static_cast <float>(_position.at(2) + offset)
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

/*
 * File: Block.cpp
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#include "Block.h"

Block::Block(const JBTypes::vec3ui& position, bool hasInteraction, bool isFixed):
    _position(position),
    _hasInteraction(hasInteraction),
    _isFixed(isFixed),
    _localScale{1.f, 1.f, 1.f},
    _localTranslation{0.f, 0.f, 0.f},
    _items{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    _hasItems(false) {
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

Block::Effect Block::interaction (const JBTypes::Dir&,
                                  const JBTypes::timePointMs&,
                                  const JBTypes::vec3f&
                                  ) {
    return Block::Effect::Nothing;
}

// TODO : delete it, items ptr should be add in the constructor
void Block::createItem (Item::CategoryOfItems category,
                        JBTypes::Dir dir) {
    _hasItems = true;
    switch (category) {
    case Item::CategoryOfItems::Clock:
        _items.at(JBTypesMethods::directionAsInteger(dir)) = std::make_shared <Clock>(_position, dir);
        break;
    case Item::CategoryOfItems::Coin:
        _items.at(JBTypesMethods::directionAsInteger(dir)) = std::make_shared <Coin>(_position, dir);
        break;
    case Item::CategoryOfItems::Key:
        _items.at(JBTypesMethods::directionAsInteger(dir)) = std::make_shared <Key>(_position, dir);
        break;
    default:
        break;
    }
}

std::string Block::getBlockOptions() const {
    return "";
}

const std::shared_ptr <const Item> Block::item (size_t number) const {
    return _items.at(number);
}

bool Block::hasInteraction() const {
    return _hasInteraction;
}

bool Block::hasItems() const {
    return _hasItems;
}

void Block::catchItem (const JBTypes::vec3f& entityPosition,
                       float radiusEntity) {
    for (size_t i = 0; i < _items.size(); ++i) {
        const std::shared_ptr <Item> item = _items.at(i);
        if (item && !item->isGotten()) {
            const JBTypes::vec3f itemPos = itemPosition(_position, static_cast <unsigned int>(i));
            item->catchingTest(itemPos, entityPosition, radiusEntity);
        }
    }
}

const bool& Block::isFixed() const {
    return _isFixed;
}

// TODO : should be in Item class
JBTypes::vec3f Block::itemPosition (const JBTypes::vec3ui& pos, unsigned int dirUint) {

    constexpr float offsetPosition = 1.f;
    float x = static_cast <float>(pos.at(0) + 0.5f);
    float y = static_cast <float>(pos.at(1) + 0.5f);
    float z = static_cast <float>(pos.at(2) + 0.5f);
    const JBTypes::Dir direction = JBTypesMethods::integerAsDirection(dirUint);

    switch (direction) {
    case JBTypes::Dir::North:
        z -= offsetPosition;
        break;
    case JBTypes::Dir::South:
        z += offsetPosition;
        break;
    case JBTypes::Dir::East:
        x += offsetPosition;
        break;
    case JBTypes::Dir::West:
        x -= offsetPosition;
        break;
    case JBTypes::Dir::Up:
        y += offsetPosition;
        break;
    case JBTypes::Dir::Down:
        y -= offsetPosition;
        break;
    default:
        break;
    }

    return JBTypes::vec3f { x, y, z };
}

const std::array <std::shared_ptr <Item>, 6>& Block::items() const {
    return _items;
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

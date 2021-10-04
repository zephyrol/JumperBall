/*
 * File: Block.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <scene/blocks/enemies/Enemy.h>
#include <scene/blocks/special/Special.h>
#include <scene/blocks/items/Item.h>
#include "system/Types.h"
#include "scene/SceneElement.h"

class Block;
using Block_sptr = std::shared_ptr <Block>;
using CstBlock_sptr = std::shared_ptr <const Block>;
using vecCstBlock_sptr = std::vector <CstBlock_sptr>;
using vecBlock_sptr = std::vector <Block_sptr>;

class Block: public SceneElement {
public:

Block(
    const JBTypes::vec3ui& position,
    const vecItem_sptr& items,
    const vecEnemy_sptr& enemies,
    const vecSpecial_sptr& specials,
    bool alwaysHasInteractions,
    bool isFixed
);

virtual ~Block() = default;
enum class Effect { Nothing, Burst, Burn, Slide, Jump };

virtual Block::Effect detectionEvent();

static constexpr size_t itemsNumber = 6;

virtual std::array <bool, 6> faceInfo() const;
virtual bool isExists() const;
virtual JBTypes::Color getColor() const;

const JBTypes::vec3f& localScale() const;
const JBTypes::vec3f& localTranslation() const;

const vecCstItem_sptr& getItems() const;
const vecCstEnemy_sptr& getEnemies() const;
const vecCstSpecial_sptr& getSpecials() const;

bool hasInteraction() const;

virtual void update(const JBTypes::timePointMs &updatingTime);
virtual Effect interaction();

std::map<CstItem_sptr ,vecCstShape_sptr> getItemShapes() const;
virtual vecCstShape_sptr getExtraShapes() const;

virtual bool mayDisappear() const;

virtual const bool& isFixed() const;
const JBTypes::vec3ui& position() const;

SceneElement::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;
SceneElement::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;
SceneElement::GlobalState getGlobalState() const override;

static std::string positionToString(const JBTypes::vec3ui& position);

private: 


vecCstItem_sptr getCstItems() const;
vecCstEnemy_sptr getCstEnemies() const;
vecCstSpecial_sptr getCstSpecials() const;

protected:
const JBTypes::vec3ui _position;
const vecItem_sptr _items;
const vecCstItem_sptr _cstItems;
const vecEnemy_sptr _enemies;
const vecCstEnemy_sptr _cstEnemies;
const vecSpecial_sptr _specials;
const vecCstSpecial_sptr _cstSpecials;
const bool _hasInteraction;
const bool _isFixed;
JBTypes::timePointMs _updatingTime;
JBTypes::vec3f _localScale;
JBTypes::vec3f _localTranslation;
};

#endif /* BLOCK_H */

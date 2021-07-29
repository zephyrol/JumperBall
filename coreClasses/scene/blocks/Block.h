/*
 * File: Block.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef BLOCK_H
#define BLOCK_H
#include "system/Types.h"
#include "scene/enemies/Enemy.h"
#include "scene/items/Item.h"
#include "scene/special/Special.h"
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
    const vecCstEnemy_sptr& enemies,
    const vecSpecial_sptr& specials,
    bool hasInteraction = false,
    bool isFixed = true
);

virtual ~Block() = default;
enum class Effect { Nothing, Burst, Burn, Slide, Jump };

virtual Effect interaction(
    const JBTypes::Dir& ballDir,
    const JBTypes::timePointMs& currentTime,
    const JBTypes::vec3f& posBall
    );
virtual Effect detectionEvent(const JBTypes::Dir& ballDir, const JBTypes::timePointMs& currentTime);

virtual unsigned char getBlockSymbol() const = 0;
virtual std::string getBlockOptions() const;

static constexpr size_t itemsNumber = 6;

virtual std::array <bool, 6> faceInfo() const;
virtual bool isExists() const;

const JBTypes::vec3f& localScale() const;
const JBTypes::vec3f& localTranslation() const;

const vecCstItem_sptr& getItems() const;
const vecCstEnemy_sptr& getEnemies() const;
const vecCstSpecial_sptr& getSpecials() const;

bool hasInteraction() const;

void catchItem(const JBTypes::vec3f& boundingSphereCenter, float boundingSphereRadius);
Block::Effect updateEnemies(const JBTypes::vec3f& boundingSphereCenter, float boundingSphereRadius);

virtual const bool& isFixed() const;
const JBTypes::vec3ui& position() const;

SceneElement::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;
SceneElement::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;
SceneElement::GlobalState getGlobalState() const override;

static JBTypes::vec3f itemPosition(const JBTypes::vec3ui& pos, unsigned int dirUint);


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
JBTypes::vec3f _localScale;
JBTypes::vec3f _localTranslation;
};

#endif /* BLOCK_H */

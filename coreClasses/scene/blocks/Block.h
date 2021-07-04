/*
 * File: Block.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef BLOCK_H
#define BLOCK_H
#include "system/Types.h"
#include "scene/items/Clock.h"
#include "scene/items/Coin.h"
#include "scene/items/Key.h"
#include "scene/SceneElement.h"

class Block;
using Block_sptr = std::shared_ptr <Block>;
using CstBlock_sptr = std::shared_ptr <const Block>;
using vecCstBlock_sptr = std::vector <CstBlock_sptr>;
using vecBlock_sptr = std::vector <Block_sptr>;

class Block: public SceneElement {
public:

// --CONSTRUCTORS & DESTRUCTORS--//
Block(const JBTypes::vec3ui& position,
      bool hasInteraction = false,
      bool isFixed = true);
virtual ~Block() = default;

// ------------TYPES------------//
enum class Effect { Nothing, Burst, Burn, Slide, Jump };

// ----------METHODS-------------//
virtual Effect interaction(
    const JBTypes::Dir& ballDir,
    const JBTypes::timePointMs& currentTime,
    const JBTypes::vec3f& posBall
    );
virtual Effect detectionEvent(const JBTypes::Dir& ballDir, const JBTypes::timePointMs& currentTime);
virtual void createItem(Item::CategoryOfItems category, JBTypes::Dir dir);

static constexpr size_t itemsNumber = 6;

virtual std::array <bool, 6> faceInfo() const;
virtual bool isExists() const;

const JBTypes::vec3f& localScale() const;
const JBTypes::vec3f& localTranslation() const;

const std::shared_ptr <const Item> item(size_t number) const;
bool hasInteraction() const;
bool hasItems() const;
void catchItem(
    const JBTypes::vec3f& entityPosition,
    float radiusEntity);
virtual const bool& isFixed() const;
const std::array <std::shared_ptr <Item>, 6>& items() const;
const JBTypes::vec3ui& position() const;

SceneElement::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;
SceneElement::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;
SceneElement::GlobalState getGlobalState() const override;

static JBTypes::vec3f itemPosition(const JBTypes::vec3ui& pos, unsigned int dirUint);


protected:
// --------ATTRIBUTES-----------//
const JBTypes::vec3ui _position;
const bool _hasInteraction;
const bool _isFixed;
JBTypes::vec3f _localScale;
JBTypes::vec3f _localTranslation;
std::array <std::shared_ptr <Item>, 6> _items;
bool _hasItems;
};

#endif /* BLOCK_H */

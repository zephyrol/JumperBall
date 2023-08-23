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

using Block_sptr = std::shared_ptr<Block>;
using CstBlock_sptr = std::shared_ptr<const Block>;
using vecCstBlock_sptr = std::vector<CstBlock_sptr>;
using vecBlock_sptr = std::vector<Block_sptr>;
using BlockDir = std::pair<Block_sptr, JBTypes::Dir>;

class Block : public SceneElement {
public:

    Block(
        const JBTypes::vec3ui &position,
        const vecItem_sptr &items,
        const vecEnemy_sptr &enemies,
        const vecSpecial_sptr &specials,
        bool alwaysHasInteractions
    );

    ~Block() override = default;

    enum class Effect {
        Nothing, Burst, Burn, Slide, Jump, FinishLevel
    };

    virtual Block::Effect detectionEvent() = 0;

    static constexpr size_t itemsNumber = 6;

    virtual std::array<bool, 6> faceInfo() const;

    virtual bool isExists() const;

    virtual JBTypes::Color getColor() const;

    const vecCstItem_sptr &getItems() const;

    const vecCstEnemy_sptr &getEnemies() const;

    const vecCstSpecial_sptr &getSpecials() const;

    bool hasInteraction() const;

    virtual void update();

    virtual void unlockExit();

    virtual Effect interaction() const = 0;

    virtual vecCstShape_sptr getExtraShapes() const;

    virtual bool mayDisappear() const;

    const JBTypes::vec3ui &position() const;

    Displayable::DynamicValues<JBTypes::vec3f> getDynamicVec3fValues() const override;

    DynamicValues<float> getDynamicFloatValues() const override;

    DynamicValues <JBTypes::Quaternion> getDynamicQuaternionValues() const override;

    static std::string positionToString(const JBTypes::vec3ui &position);

    vecCstItem_sptr getCstItems() const;

    vecCstEnemy_sptr getCstEnemies() const;

    vecCstSpecial_sptr getCstSpecials() const;

    vecItem_sptr getItems();

    vecEnemy_sptr getEnemies();

    vecSpecial_sptr getSpecials();

    std::string getDynamicGroupHash() const override;

protected:
    const JBTypes::vec3ui _position;
    const vecItem_sptr _items;
    const vecCstItem_sptr _cstItems;
    const vecEnemy_sptr _enemies;
    const vecCstEnemy_sptr _cstEnemies;
    const vecSpecial_sptr _specials;
    const vecCstSpecial_sptr _cstSpecials;
    const bool _hasInteraction;
};

#endif /* BLOCK_H */

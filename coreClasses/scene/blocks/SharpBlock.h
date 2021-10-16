/*
 * File: SharpBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:51
 */

#ifndef SHARPBLOCK_H
#define SHARPBLOCK_H

#include "Block.h"
#include "InteractiveBlock.h"

class SharpBlock:public InteractiveBlock {
public:

    SharpBlock(
        const JBTypes::vec3ui& position,
        const vecItem_sptr& items,
        const vecEnemy_sptr& enemies,
        const vecSpecial_sptr& specials,
        const Ball_sptr& ball,
        const std::array <bool, 6>& facesSharps
    );

    std::array <bool, 6> faceInfo() const override;

Block::Effect interaction() const override;

vecCstShape_sptr getExtraShapes() const override;
std::vector<std::pair<JBTypes::vec3f, JBTypes::vec3f> > computeSharpBoundingBoxes() const;

private:
const std::array <bool, 6> _facesSharps;
const std::vector<std::pair<JBTypes::vec3f, JBTypes::vec3f> > _sharpBoundingBoxes;
};

#endif /* SHARPBLOCK_H */

/*
 * File: SharpBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:51
 */

#ifndef SPICYBLOCK_H
#define SPICYBLOCK_H
#include "Block.h"

class SharpBlock:public Block {
public:
SharpBlock(const JBTypes::vec3ui& position, const std::array <bool, 6>& facesSharps);

std::array <bool, 6> faceInfo() const override;

virtual Block::Effect interaction(
    const JBTypes::Dir& ballDir,
    const JBTypes::timePointMs& currentTime,
    const JBTypes::vec3f& posBall
) override;

virtual unsigned char getBlockSymbol() const override;
virtual std::string getBlockOptions() const override;

private:
const std::array <bool, 6> _facesSharps;
};

#endif /* SPICYBLOCK_H */

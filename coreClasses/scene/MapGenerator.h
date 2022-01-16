/*
 * File: MapGenerator.h
 * Author: Morgenthaler S
 *
 * Created on 2 novembre 2020, 22:38
 */

#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include "Map.h"

namespace MapGenerator {


static constexpr unsigned char baseUsuableAsciiCharacters = 92;
static constexpr unsigned char firstUsuableAsciiCharacter = 34;

// ---------CONSTANTS------------//

// Blocks compression
static constexpr unsigned char firstKindOfBlock = 48;
static constexpr unsigned char firstNumberOfBlock = 58;
static constexpr unsigned int nbOfCharactersUsedForNumbers = 69;
static constexpr unsigned int firstNumberParams = 33;

// Items and special compression
static constexpr unsigned char nextBlockAction = 33;
static constexpr unsigned char firstNumberType = 34;
static constexpr unsigned char firstNumberSide = 42;
static constexpr unsigned char firstNumberWithoutAnyItems = 48;
static constexpr unsigned int nbOfCharactersWithoutItems = 79;

// Enemies compression
static constexpr unsigned char firstNumberLength = 33;
static constexpr unsigned char firstNumberColor = 33;

bool verifyBlockType(unsigned char blockType);
bool verifyBlockOption(unsigned char blockOption);
bool blockHasAnyProperties(unsigned char blockType);

void compress(std::ifstream& input);

std::string compressString(const std::string& asciiString);
std::string uncompressString(const std::string& compressedString);

void compressNew(std::ifstream& input);

std::shared_ptr <Map> loadMap(size_t mapNumber);

Map::MapInfo uncompressMap(std::ifstream& file);

std::string convertToBase(unsigned long long int number, unsigned char base);
unsigned long long int convertToBase10(const std::string& s, unsigned int base);
std::string applyOffset(const std::string& s, int offset);
std::string substractOffset(const std::string& s, int offset);

Block_sptr createBlockWithoutProperties(
    unsigned char blockType,
    const vecItem_sptr& items,
    const vecEnemy_sptr& enemies,
    const vecSpecial_sptr& specials
);

// Reading functions
unsigned int readUnsignedInt(std::ifstream& input);
std::string readingString(std::ifstream& input);

// Writing functions
void writeSeparator(std::ofstream& output);
void writeEndLine(std::ofstream& output);
void writeUnsignedInt(std::ofstream& output, unsigned int unsignedInt);
void writeString(std::ofstream& output, const std::string& string);

void verificationMap(std::ifstream& input, const Map& map);

    vecBlock_sptr createBlocks(
        unsigned char blockType,
        const JBTypes::vec3ui &position,
        const vecItem_sptr &items,
        const vecEnemy_sptr &enemies,
        const vecSpecial_sptr &specials,
        const Ball_sptr &ball,
        const std::string& properties = ""
    );

}

#endif

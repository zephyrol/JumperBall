/*
 * File: MapGenerator.cpp
 * Author: Morgenthaler S
 *
 * Created on 2 novembre 2020, 22:38
 */
#include "MapGenerator.h"
#include <algorithm>
#include <cmath>
#include <future>
#include <functional>
#include <scene/blocks/special/SwitchButton.h>
#include <scene/blocks/items/Clock.h>
#include <scene/blocks/items/Key.h>
#include <scene/blocks/items/Coin.h>
#include <scene/blocks/enemies/Laser.h>
#include <scene/blocks/enemies/ThornBall.h>
#include <scene/blocks/enemies/DarkBall.h>
#include <scene/blocks/special/Teleporter.h>
#include <scene/blocks/BaseBlock.h>
#include <scene/blocks/FireBlock.h>
#include <scene/blocks/BrittleBlock.h>
#include <scene/blocks/IceBlock.h>
#include <scene/blocks/SharpBlock.h>
#include <scene/blocks/JumpBlock.h>
#include <scene/blocks/GhostBlock.h>


std::shared_ptr <Map> MapGenerator::loadMap (size_t mapNumber) {
    std::shared_ptr <Map> map = nullptr;

    const std::string mapFileToOpenV1 = "maps/map" + std::to_string(mapNumber) + ".txt";
    std::string mapFileToOpenV2 = "bin/maps/map" + std::to_string(mapNumber) + ".txt";
    const std::vector <std::string> fileNames {
        mapFileToOpenV1, std::move(mapFileToOpenV2) };

    bool foundFile = false;
    for (size_t i = 0; i < fileNames.size() && !foundFile; ++i) {
        std::ifstream mapFile;
        mapFile.open(fileNames.at(i)); // Opening file to read
        if (mapFile) {
            foundFile = true;
            map = std::make_shared <Map>(uncompressMap(mapFile));
            mapFile.close();
        }
    }

    if (!foundFile) {
        std::cerr << "ERROR: Opening " << mapFileToOpenV1 << " impossible .." << std::endl;
        JBTypesMethods::displayInstallError();
        exit(EXIT_FAILURE);
    }
    std::cout << "Map " << mapNumber << " loaded" << std::endl;

    return map;
}


Map::MapInfo MapGenerator::uncompressMap(std::ifstream& file) {

    Map::MapInfo mapInfo {};

    const auto getUncompressedDimension = [](const std::string& compressedDimension) {
        const std::string baseAsciiDimension = substractOffset(
            compressedDimension,
            firstUsuableAsciiCharacter
        );
        return convertToBase10(baseAsciiDimension, baseUsuableAsciiCharacters);
    };

    mapInfo.width = getUncompressedDimension(readingString(file));
    mapInfo.depth = getUncompressedDimension(readingString(file));
    mapInfo.height = getUncompressedDimension(readingString(file));

    const auto beginX = getUncompressedDimension(readingString(file));
    const auto beginZ = getUncompressedDimension(readingString(file));
    const auto beginY = getUncompressedDimension(readingString(file));
    mapInfo.ball = std::make_shared<Ball>(beginX, beginY, beginZ);


    const auto getTypeOptions = [](
        const std::string& blockAdditionalInfo,
        const std::function<bool(unsigned char)>& isAType
    ) {

        std::map<unsigned char, std::string> typeOptions;

        unsigned char currentType;
        for (const char info : blockAdditionalInfo) {
            if (isAType(info)) {
                currentType = info;
                typeOptions[currentType] = {};
            } else {
                typeOptions[currentType].push_back(info);
            }
        }
        return typeOptions;
    };

    size_t blockIndexCursor= 0;

    const auto isAnItemTypeChar = [](unsigned char info) {
        return info == 'I' || info == 'E' || info == 'S';
    };
    const auto createItems = 
    [&getTypeOptions, &isAnItemTypeChar, &mapInfo, &blockIndexCursor](const std::string& itemsInfo) {
        const auto typeOptions = getTypeOptions(itemsInfo, isAnItemTypeChar);

        const auto createItem = 
        [&mapInfo, &blockIndexCursor](unsigned char itemType, unsigned char direction) -> Item_sptr {
            const JBTypes::Dir dir = JBTypesMethods::charAsDirection(direction);
            const auto blockCoords = Map::getBlockCoords(blockIndexCursor, mapInfo.width, mapInfo.depth);
            if (itemType == 'I') {
                return std::make_shared<Coin>(blockCoords, dir);
            }
            if (itemType  == 'K') {
                return std::make_shared<Key>(blockCoords, dir);
            } 
            //Clock
            return std::make_shared<Clock>(blockCoords, dir);
        };

        vecItem_sptr items;
        for (const auto& typeOption: typeOptions) {
            const unsigned char itemType = typeOption.first;
            const std::string& itemOptions = typeOption.second;
            for (const unsigned char direction : itemOptions) {
                items.push_back(createItem(itemType, direction));
            }
        }
        return items;
    };

    const auto isAnEnemyTypeChar = [](unsigned char info) {
        return info == 'L' || info == 'T' || info == 'D';
    };
    const auto createEnemies = 
    [&getTypeOptions, &isAnEnemyTypeChar, &mapInfo, &blockIndexCursor](const std::string& enemiesInfo) {
        const auto typeOptions = getTypeOptions(enemiesInfo, isAnEnemyTypeChar);

        const auto createEnemy = 
        [&mapInfo, &blockIndexCursor](
            unsigned char enemyType,
            const std::string& options
            ) -> Enemy_sptr {
            const auto blockCoords = Map::getBlockCoords(blockIndexCursor, mapInfo.width, mapInfo.depth);
            const JBTypes::Dir direction = JBTypesMethods::charAsDirection(options.at(0));
            if (enemyType == 'L') {
                const JBTypes::Color color = JBTypesMethods::charAsColor(options.at(1));
                const size_t length = static_cast<size_t>(
                    stoi(std::string(options.begin() + 2, options.end()))
                );
                return std::make_shared<Laser>(color, blockCoords, direction, length, mapInfo.ball);
            }
            if (enemyType  == 'T') {
                const JBTypes::Dir movementDir = JBTypesMethods::charAsDirection(options.at(1));
                const size_t movementLength = static_cast<size_t>(
                    stoi(std::string(options.begin() + 2, options.end()))
                );
                return std::make_shared<ThornBall>(
                    blockCoords,
                    direction,
                    movementDir,
                    movementLength,
                    mapInfo.ball
                );
            }
            // Dark Ball
            const JBTypes::Dir movementDir = JBTypesMethods::charAsDirection(options.at(1));
            const size_t numberOfJumps = static_cast<size_t>(
                stoi(std::string(options.begin() + 2, options.end())));
            return std::make_shared<DarkBall>(
                blockCoords,
                direction,
                movementDir,
                numberOfJumps,
                mapInfo.ball
            );
        };

        vecEnemy_sptr enemies;
        for (const auto& typeOption: typeOptions) {
            const unsigned char enemyType = typeOption.first;
            const std::string& enemyOptions = typeOption.second;
            enemies.push_back(createEnemy(enemyType, enemyOptions));
        }
        return enemies;
    };

    const auto isAnSpecialTypeChar = [](unsigned char info) {
        return info == 'S' || info == 'T';
    };
    const auto createSpecials = 
    [&getTypeOptions, &isAnSpecialTypeChar](
        const std::string& specialsInfo,
        const JBTypes::vec3ui& blockCoords
        ) {
        const auto typeOptions = getTypeOptions(specialsInfo, isAnSpecialTypeChar);

        const auto createSpecial = 
        [&blockCoords] (
            unsigned char specialType,
            const std::string& options
            ) -> Special_sptr {
            const JBTypes::Color color = JBTypesMethods::charAsColor(options.at(0));
            const JBTypes::Dir direction = JBTypesMethods::charAsDirection(options.at(1));
            if (specialType == 'S') {
                return std::make_shared<SwitchButton>(color, direction, blockCoords);
            } else {
                return std::make_shared<Teleporter>(color, direction, blockCoords);
            }
        };

        vecSpecial_sptr specials;
        for (const auto& typeOption: typeOptions) {
            const unsigned char specialType = typeOption.first;
            const std::string& specialOptions = typeOption.second;
            specials.push_back(createSpecial(specialType, specialOptions));
        }
        return specials;
    };

    std::vector<Block_sptr> blocks;
    for (std::string infoMap = readingString(file); !infoMap.empty(); infoMap = readingString(file)) {
        if (infoMap.front() == '!') {
            const std::string compressedNoBlocksCounter (infoMap.begin() + 1, infoMap.end());
            const std::string asciiBasedNoBlocksCounter = substractOffset(
                compressedNoBlocksCounter,
                firstUsuableAsciiCharacter
            );
            const unsigned int noBlocksCounter = convertToBase10(
                asciiBasedNoBlocksCounter,
                baseUsuableAsciiCharacters
            );
            blockIndexCursor += noBlocksCounter;
        } else {
            const std::string uncompressedInfoMap = uncompressString(infoMap);

            // Block type
            const unsigned char blockType = uncompressedInfoMap.front();
            verifyBlockType(blockType);

            // Block options
            const std::string blockOptions (uncompressedInfoMap.begin() + 1, uncompressedInfoMap.end());
            for (unsigned char blockOption: blockOptions) {
                verifyBlockOption(blockOption);
            }
            
            vecItem_sptr items;
            vecEnemy_sptr enemies;
            vecSpecial_sptr specials;

            const auto position = Map::getBlockCoords(blockIndexCursor, mapInfo.width, mapInfo.depth);
            for (unsigned char optionType : blockOptions) {
                const std::string uncompressedString = uncompressString(readingString(file));
                if (optionType == 'I') {
                    items = createItems(uncompressedString);
                } else if (optionType == 'E') {
                    enemies = createEnemies(uncompressedString);
                } else if (optionType == 'S') {
                    specials = createSpecials(uncompressedString, position);
                } else {
                    std::cerr << "Unknown block option: " << optionType << std::endl;
                }
            }
            const std::string properties =  blockHasAnyProperties(blockType)
                ? uncompressString(readingString(file))
                : "";

            mapInfo.blocks.push_back(
                createBlock(blockType, position, items, enemies, specials, mapInfo.ball, properties)
            );
            ++blockIndexCursor;
        }
    }

    return mapInfo;
}

std::string MapGenerator::convertToBase (unsigned long long int number, unsigned char base) {
    std::string convertedNumber;

    while (number > 0) {
        const unsigned int remainder = number % base;
        convertedNumber.insert(convertedNumber.begin(), static_cast<char>(remainder));

        number = number / base;
    }

    return convertedNumber;
}

unsigned long long int MapGenerator::convertToBase10 (const std::string& s, unsigned int base) {
    unsigned long long int value = 0;
    std::string copyS = s;
    while (copyS.length() > 0) {
        const auto number = static_cast <unsigned int>(copyS.front());
        value += number * static_cast <unsigned int>(pow(base, copyS.length() - 1));
        copyS.erase(copyS.begin());
    }
    return value;
}

std::string MapGenerator::substractOffset (const std::string& s, int offset) {
    std::string offsetString = s;
    for (char& c : offsetString) {
        c -= offset;
    }
    return offsetString;
}

std::string MapGenerator::applyOffset (const std::string& s, int offset) {
    std::string offsetString = s;
    for (char& c : offsetString) {
        c += offset;
    }
    return offsetString;
}

void MapGenerator::compressNew(std::ifstream& input) {
    std::ofstream output("outMapNew.txt");

    // Compress dimensions
    const auto getCompressedDimension = [](unsigned int uncompressedDimension) {
        const std::string baseAsciiDimension = convertToBase(
            uncompressedDimension,
            baseUsuableAsciiCharacters
        );
        return applyOffset(baseAsciiDimension, firstUsuableAsciiCharacter);
    };

    const unsigned int width = readUnsignedInt(input);
    const unsigned int depth = readUnsignedInt(input);
    const unsigned int height = readUnsignedInt(input);
    const std::string compressedWidth = getCompressedDimension(width);
    const std::string compressedDepth = getCompressedDimension(depth);
    const std::string compressedHeight = getCompressedDimension(height);
    const std::string beginX = getCompressedDimension(readUnsignedInt(input));
    const std::string beginZ = getCompressedDimension(readUnsignedInt(input));
    const std::string beginY = getCompressedDimension(readUnsignedInt(input));

    for (const std::string &dimension : { 
        compressedWidth, compressedDepth, compressedHeight, beginX, beginY, beginZ 
    }) {
        output << dimension;
        writeSeparator(output);
    }

    // Compress blocks
    enum BlockExistence { Exist, NotExist };

    const auto getBlockExistence = [](const std::string& word) -> BlockExistence {
        if (word.size() != 1 || (word.front() != '.' && word.front() != 'X')) {
            std::cerr << "Block existence " << word << " is invalid, should be . or X" << std::endl;
            return BlockExistence::NotExist;
        }
        const unsigned char blockExistenceChar = word.front();
        return blockExistenceChar == 'X' ? BlockExistence::Exist : BlockExistence::NotExist;
    };

    const auto compressAndWrite = [](const std::string& uncompressedString, std::ofstream& output) {
        const std::string compressedString = compressString(uncompressedString);
        output << compressedString; 
        writeSeparator(output);
    };


    const auto writeBlock =
        [&output, &input, &compressAndWrite]() 
    {
        const std::string blockInfo = readingString(input);

        const unsigned char blockType = blockInfo.front();
        verifyBlockType(blockType);

        const std::string blockOptions(blockInfo.begin() + 1, blockInfo.end());
        for (unsigned char blockOption : blockOptions) {
            verifyBlockOption(blockOption);
        }

        compressAndWrite(blockInfo, output);

        const bool hasProperties = blockHasAnyProperties(blockType);
        const size_t numberOfWordsToRead = hasProperties 
            ? blockOptions.size() + 1
            : blockOptions.size();

        for (unsigned int i = 0; i < numberOfWordsToRead; ++i) {
            const std::string uncompressedOptionOrProperty = readingString(input);
            compressAndWrite(uncompressedOptionOrProperty, output);
        }
    };

    unsigned int noBlockCounter = 0;
    const auto writeNoBlocks = [&noBlockCounter,&output]() {
        if(noBlockCounter == 0) {
            return;
        }
        const std::string counterAscii = convertToBase(noBlockCounter, baseUsuableAsciiCharacters);
        output << "!" <<  applyOffset(counterAscii, firstUsuableAsciiCharacter);
        writeSeparator(output);
        noBlockCounter = 0;
    };

    for (unsigned int i = 0; i < depth * height; ++i) {
        std::cout << "line " << i << std::endl;
        std::vector<BlockExistence> blocksExistencesRow;
        for (unsigned int j = 0; j < width; ++j) {
            blocksExistencesRow.push_back(getBlockExistence(readingString(input)));
        }

        for(const BlockExistence& blockExistence: blocksExistencesRow) {
            if (blockExistence == BlockExistence::NotExist) {
                ++noBlockCounter;
            } else {
                writeNoBlocks();
                writeBlock();
            }
        }
    }
    writeNoBlocks();
    output.close();
}

unsigned int MapGenerator::readUnsignedInt (std::ifstream& input) {
    unsigned int readValue;
    input >> readValue;
    return readValue;
}

std::string MapGenerator::readingString (std::ifstream& input) {
    std::string readValue;
    input >> readValue;
    return readValue;
}

void MapGenerator::writeSeparator (std::ofstream& output) {
    output << " ";
}

void MapGenerator::writeEndLine (std::ofstream& output) {
    output << std::endl;
}

std::string MapGenerator::compressString(const std::string &asciiString) {

    const std::map<unsigned char, unsigned char> asciiToLetterDigitBaseCorrespondences{
        {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, 
        {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, {'Q', 16},
        {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23}, {'Y', 24},
        {'Z', 25}, {'0', 26}, {'1', 27}, {'2', 28}, {'3', 29}, {'4', 30}, {'5', 31}, {'6', 32},
        {'7', 33}, {'8', 34}, {'9', 35}
    };

    const auto asciiToLetterDigitBase = [&asciiToLetterDigitBaseCorrespondences](unsigned char asciiChar) {

        if (asciiToLetterDigitBaseCorrespondences.find(asciiChar) ==
            asciiToLetterDigitBaseCorrespondences.end()) {
            std::cerr << "Error: Unknown character : " << asciiChar << std::endl;
            return static_cast<unsigned char>(0);
        }
        return asciiToLetterDigitBaseCorrespondences.at(asciiChar);
    };

    const auto asciiStringToLetterDigitBase = [&asciiToLetterDigitBase](const std::string &asciiString) {
        std::string letterDigitBaseString;
        for (unsigned char c : asciiString) {
            letterDigitBaseString.push_back(static_cast<char>(asciiToLetterDigitBase(c)));
        }
        return letterDigitBaseString;
    };

    const std::string letterDigitBaseString = asciiStringToLetterDigitBase(asciiString);
    const unsigned long long int base10StringRepresentation = convertToBase10(
        letterDigitBaseString,
        asciiToLetterDigitBaseCorrespondences.size()
    );
    const std::string baseUsuableAsciiCharactersBaseString = convertToBase(
        base10StringRepresentation,
        baseUsuableAsciiCharacters
    );
    return applyOffset(baseUsuableAsciiCharactersBaseString ,firstUsuableAsciiCharacter);
}

std::string MapGenerator::uncompressString(const std::string &compressedString) {

    const std::map<unsigned char, unsigned char> letterDigitBaseToAsciiCorrespondences{
        {0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'}, {5, 'F'}, {6, 'G'}, {7, 'H'}, {8, 'I'}, 
        {9, 'J'}, {10, 'K'}, {11, 'L'}, {12, 'M'}, {13, 'N'}, {14, 'O'}, {15, 'P'}, {16, 'Q'},
        {17, 'R'}, {18, 'S'}, {19, 'T'}, {20, 'U'}, {21, 'V'}, {22, 'W'}, {23, 'X'}, {24, 'Y'},
        {25, 'Z'}, {26, '0'}, {27, '1'}, {28, '2'}, {29, '3'}, {30, '4'}, {31, '5'}, {32, '6'},
        {33, '7'}, {34, '8'}, {35, '9'}
    };

    const auto letterDigitToAsciiBase = [&letterDigitBaseToAsciiCorrespondences](unsigned char letterDigit) {

        if (letterDigitBaseToAsciiCorrespondences.find(letterDigit) ==
            letterDigitBaseToAsciiCorrespondences.end()) {
            std::cerr << "Error: Unknown character : " << letterDigit << std::endl;
            return static_cast<unsigned char>(0);
        }
        return letterDigitBaseToAsciiCorrespondences.at(letterDigit);
    };

    const auto letterDigitStringToAsciiBase = [&letterDigitToAsciiBase](const std::string &letterDigitStr) {
        std::string asciiBaseString;
        for (unsigned char c : letterDigitStr) {
            asciiBaseString.push_back(static_cast<char>(letterDigitToAsciiBase(c)));
        }
        return asciiBaseString;
    };

    const std::string baseUsuableAsciiCharactersBaseString = substractOffset(
        compressedString, 
        firstUsuableAsciiCharacter
    );
    const unsigned long long int base10StringRepresentation = convertToBase10(
        baseUsuableAsciiCharactersBaseString,
        baseUsuableAsciiCharacters
    );
    const std::string letterDigitBaseStr = convertToBase(
        base10StringRepresentation,
        letterDigitBaseToAsciiCorrespondences.size()
    );

    return letterDigitStringToAsciiBase(letterDigitBaseStr);
}

bool MapGenerator::verifyBlockType(unsigned char blockType) {
    const std::vector<unsigned char> blocksList = { 'N', 'F', 'I', 'B', 'G', 'S', 'J' };
    if (std::find(blocksList.begin(), blocksList.end(), blockType) == blocksList.end()) {
        std::cerr << "Error: unknown block type: " << blockType << std::endl;
        return false;
    }
    return true;
}

bool MapGenerator::verifyBlockOption(unsigned char blockOption) {
    const std::vector<unsigned char> optionsList = { 'I', 'E', 'S' };
    if (std::find(optionsList.begin(), optionsList.end(), blockOption) == optionsList.end()) {
        std::cerr << "Error: unknown block option: " << blockOption << std::endl;
        return false;
    }
    return true;
}

bool MapGenerator::blockHasAnyProperties(unsigned char blockType) {
    const std::vector<unsigned char> blocksWithProperties = { 'G', 'S', 'J' };
    return std::find(
               blocksWithProperties.begin(),
               blocksWithProperties.end(),
               blockType) != blocksWithProperties.end();
}

Block_sptr MapGenerator::createBlock(
    unsigned char blockType,
    const JBTypes::vec3ui& position,
    const vecItem_sptr &items,
    const vecEnemy_sptr &enemies,
    const vecSpecial_sptr &specials,
    const Ball_sptr &ball,
    const std::string& properties
    ) {
    switch (blockType) {
        case 'N': return std::make_shared<BaseBlock>(position, items, enemies, specials, ball);
        case 'F': return std::make_shared<FireBlock>(position, items, enemies, specials, ball);
        case 'I': return std::make_shared<IceBlock>(position, items, enemies, specials, ball);
        case 'B': return std::make_shared<BrittleBlock>(position, items, enemies, specials, ball);
        case 'G': return std::make_shared<GhostBlock>(
            position,
            items,
            enemies,
            specials,
            ball,
            std::stof(properties)
        );
        case 'S': return std::make_shared<SharpBlock>(
            position,
            items,
            enemies,
            specials,
            ball,
            JBTypesMethods::strDirAsArray(properties)
        );
        case 'J': return std::make_shared<JumpBlock>(
            position,
            items,
            enemies,
            specials,
            ball,
            JBTypesMethods::strDirAsArray(properties)
        );
        default: return nullptr;
    }
}


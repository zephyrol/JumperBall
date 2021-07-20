/*
 * File: MapGenerator.cpp
 * Author: Morgenthaler S
 *
 * Created on 2 novembre 2020, 22:38
 */
#include "MapGenerator.h"
#include <algorithm>
#include <math.h>
#include <cctype>
#include <future>
#include <functional>


std::shared_ptr <Map> MapGenerator::loadMap (size_t mapNumber) {
    std::shared_ptr <Map> map = nullptr;

    std::string mapFileToOpenV1 = "maps/map" +
                                  std::to_string(mapNumber) + ".txt";
    std::string mapFileToOpenV2 = "bin/maps/map" +
                                  std::to_string(mapNumber) + ".txt";
    const std::vector <std::string> fileNames {
        std::move(mapFileToOpenV1), std::move(mapFileToOpenV2) };

    bool foundFile = false;
    for (size_t i = 0; i < fileNames.size() && !foundFile; ++i) {
        std::ifstream mapFile;
        mapFile.open(fileNames.at(i)); // Opening file to read
        if (mapFile) {
            foundFile = true;
            map = std::make_shared <Map>(createMapInfo(mapFile));
            mapFile.close();
        }
    }

    if (!foundFile) {
        std::cerr << "ERROR: Opening " << mapFileToOpenV1 << " impossible .."
                  << std::endl;
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
        return convertToBase10( baseAsciiDimension, baseUsuableAsciiCharacters);
    };

    mapInfo.width = getUncompressedDimension(readingString(file));
    mapInfo.depth = getUncompressedDimension(readingString(file));
    mapInfo.height = getUncompressedDimension(readingString(file));
    mapInfo.beginX = getUncompressedDimension(readingString(file));
    mapInfo.beginZ = getUncompressedDimension(readingString(file));
    mapInfo.beginY = getUncompressedDimension(readingString(file));


    const auto getTypeOptions = [](
        const std::string& blockAdditionalInfo,
        const std::function<bool(unsigned char)>& isAType
    ) {

        std::map<unsigned char, std::string> typeOptions;

        unsigned char currentType;
        for (const unsigned char info : blockAdditionalInfo) {
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
                return std::make_shared<Laser>(color, blockCoords, direction, length);
            }
            if (enemyType  == 'T') {
                const JBTypes::Dir movementDir = JBTypesMethods::charAsDirection(options.at(1));
                const size_t movementLength = static_cast<size_t>(
                    stoi(std::string(options.begin() + 2, options.end()))
                );
                return std::make_shared<ThornBall>(blockCoords, direction, movementDir, movementLength );
            }
            // Dark Ball
            const JBTypes::Dir movementDir = JBTypesMethods::charAsDirection(options.at(1));
            const size_t numberOfJumps = static_cast<size_t>(
                stoi(std::string(options.begin() + 2, options.end())));
            return std::make_shared<DarkBall>(blockCoords, direction, movementDir, numberOfJumps);
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
    [&getTypeOptions, &isAnSpecialTypeChar, &mapInfo, &blockIndexCursor](const std::string& specialsInfo) {
        const auto typeOptions = getTypeOptions(specialsInfo, isAnSpecialTypeChar);

        const auto createSpecial = 
        [&mapInfo, &blockIndexCursor](
            unsigned char specialType,
            const std::string& options
            ) -> Special_sptr {
            const auto blockCoords = Map::getBlockCoords(blockIndexCursor, mapInfo.width, mapInfo.depth);
            const JBTypes::Color color = JBTypesMethods::charAsColor(options.at(0));
            const JBTypes::Dir direction = JBTypesMethods::charAsDirection(options.at(1));
            if (specialType == 'S') {
                return std::make_shared<SwitchButton>(color, direction, blockCoords);
            }
            // Teleporter
            return std::make_shared<Teleporter>(color, direction, blockCoords);
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
            
            const bool hasProperties = blockHasAnyProperties(blockType);

            vecItem_sptr items;
            vecEnemy_sptr enemies;
            vecSpecial_sptr specials;
            for (size_t i = 0; i < blockOptions.size(); ++i) {
                const unsigned char optionType = blockOptions.at(i);
                const std::string uncompressedString = uncompressString(readingString(file));
                if (optionType == 'I') {
                    items = createItems(uncompressedString);
                } else if (optionType == 'E') {
                    enemies = createEnemies(uncompressedString);
                } else if (optionType == 'S') {
                    specials = createSpecials(uncompressedString);
                } else {
                    std::cerr << "Unknown block option: " << optionType << std::endl;
                }

            }
        }
    }

    return mapInfo;
}

Map::MapInfo MapGenerator::createMapInfo (std::ifstream& file) {
    Map::MapInfo mapInfo {};

    unsigned int width;
    unsigned int depth;
    unsigned int height;

    file >> width;
    file >> depth;
    file >> height;

    mapInfo.width = width;
    mapInfo.height = height;
    mapInfo.depth = depth;

    file >> mapInfo.beginX;
    file >> mapInfo.beginZ;
    file >> mapInfo.beginY;

    // BLOCKS PART
    std::string infoMap;
    std::string counterBuffer;

    std::cout << "decompress blocks" << std::endl;
    char readValue;
    file >> infoMap;
    while (!infoMap.empty()) {

        readValue = infoMap.front();
        infoMap.erase(infoMap.begin());
        if (readValue >= firstNumberOfBlock) {
            counterBuffer.push_back(readValue);
        } else {
            unsigned int nbBlocksTowrite;
            if (counterBuffer.empty()) {
                nbBlocksTowrite = 1;
            } else {
                counterBuffer =
                    substractOffset(counterBuffer, firstNumberOfBlock);
                nbBlocksTowrite = convertToBase10(counterBuffer,
                                                  nbOfCharactersUsedForNumbers);
            }
            const unsigned int typeOfBlock = readValue - firstKindOfBlock;
            for (unsigned int i = 0; i < nbBlocksTowrite; ++i) {
                Block_sptr block = nullptr;
                const std::vector <unsigned int> blockWithSpecialParams { 4, 6 };
                std::array <bool, 6> specialParams
                { false, false, false, false, false, false };
                if (
                    std::find(blockWithSpecialParams.begin(),
                              blockWithSpecialParams.end(), typeOfBlock) !=
                    blockWithSpecialParams.end()
                    ) {
                    const char readParam = infoMap.front() - firstNumberParams;
                    infoMap.erase(infoMap.begin());
                    const std::string sParam =
                        convertToBase(static_cast <unsigned int>(readParam), 2);
                    for (unsigned int i = 0; i < specialParams.size(); ++i) {
                        const size_t diffSize =
                            specialParams.size() - sParam.size();
                        if (i >= diffSize) {
                            if (sParam.at(i - diffSize) == 0) {
                                specialParams.at(i) = false;
                            } else {
                                specialParams.at(i) = true;
                            }
                        } else {
                            specialParams.at(i) = false;
                        }
                    }
                }
                const auto blockPos = Map::getBlockCoords(mapInfo.blocks.size(), mapInfo.width, mapInfo.depth);
                switch (typeOfBlock) {
                case 0:
                    break;
                case 1:
                    block = std::make_shared <BaseBlock>(blockPos); break;
                case 2:
                    block = std::make_shared <FireBlock>(blockPos); break;
                case 3:
                    block = std::make_shared <IceBlock>(blockPos); break;
                case 4:
                    block = std::make_shared <SharpBlock>(blockPos, specialParams); break;
                case 5:
                    block = std::make_shared <BrittleBlock>(blockPos); break;
                case 6:
                    block = std::make_shared <JumpBlock>(blockPos, specialParams); break;
                case 7:
                    block = std::make_shared <GhostBlock>(blockPos); break;
                default:
                    break;
                }
                if (block) {
                    mapInfo.blocksInfo.push_back({ mapInfo.blocks.size(), uintToBlockType(typeOfBlock) });
                }
                mapInfo.blocks.push_back(block);
                counterBuffer.clear();
            }
        }
    }

    // OBJECTS PART
    std::cout << "decompress items" << std::endl;
    std::string infoItems;
    std::string counterWithoutItemsBuffer;
    unsigned int currentIndex = 0;
    char previousReadValue = 0;

    file >> infoItems;
    while (!infoItems.empty()) {

        readValue = infoItems.front();
        if (readValue == nextBlockAction) {
            infoItems.erase(infoItems.begin());
            ++currentIndex; // We increment the cursor to the next block
        } else if (readValue >= firstNumberWithoutAnyItems) {
            counterWithoutItemsBuffer.push_back(readValue);
            infoItems.erase(infoItems.begin());

            if (
                previousReadValue < firstNumberWithoutAnyItems &&
                previousReadValue >= firstNumberType
                ) {
                ++currentIndex; // We increment the cursor to the next block
            }
        } else {
            if (!counterWithoutItemsBuffer.empty()) {
                counterWithoutItemsBuffer =
                    substractOffset(counterWithoutItemsBuffer,
                                    firstNumberWithoutAnyItems);
                currentIndex += convertToBase10(counterWithoutItemsBuffer,
                                                nbOfCharactersWithoutItems);
            }

            const unsigned int typeOfItem = readValue - firstNumberType;

            infoItems.erase(infoItems.begin());
            readValue = infoItems.front();

            const unsigned int side = readValue - firstNumberSide;
            const JBTypes::Dir dir =
                JBTypesMethods::integerAsDirection(side);

            Item::CategoryOfItems category;
            switch (typeOfItem) {
            case 0:
                category = Item::CategoryOfItems::Key;
                break;
            case 1:
                category = Item::CategoryOfItems::Coin;
                break;
            case 2:
                category = Item::CategoryOfItems::Clock;
                break;
            default:
                category = Item::CategoryOfItems::Coin;
                break;
            }

            mapInfo.blocks.at(currentIndex)->createItem(category, dir);
            counterWithoutItemsBuffer.clear();
            infoItems.erase(infoItems.begin(), infoItems.begin() + 1);
        }
        previousReadValue = readValue;
    }

    std::cout << "decompress enemies" << std::endl;
    // ENEMIES PART
    std::string infoEnemies;
    std::string counterWithoutEnemiesBuffer;
    currentIndex = 0;
    previousReadValue = 0;

    file >> infoEnemies;
    while (!infoEnemies.empty()) {

        readValue = infoEnemies.front();
        if (readValue == nextBlockAction) {
            infoEnemies.erase(infoEnemies.begin());
            ++currentIndex; // We increment the cursor to the next block
        } else if (readValue >= firstNumberWithoutAnyItems) {
            counterWithoutEnemiesBuffer.push_back(readValue);
            infoEnemies.erase(infoEnemies.begin());

            if (
                previousReadValue < firstNumberWithoutAnyItems &&
                previousReadValue >= firstNumberType
                ) {
                ++currentIndex; // We increment the cursor to the next block
            }
        } else {
            if (!counterWithoutEnemiesBuffer.empty()) {
                counterWithoutEnemiesBuffer = substractOffset(
                    counterWithoutEnemiesBuffer,
                    firstNumberWithoutAnyItems);

                currentIndex += convertToBase10(counterWithoutEnemiesBuffer,
                                                nbOfCharactersWithoutItems);
            }
            // direction
            const unsigned int side = readValue - firstNumberSide;
            const JBTypes::Dir dir =
                JBTypesMethods::integerAsDirection(side);

            // enemy type
            infoEnemies.erase(infoEnemies.begin());
            readValue = infoEnemies.front();
            const unsigned int typeOfEnemy = readValue - firstNumberType;

            // enemy movement dir
            infoEnemies.erase(infoEnemies.begin());
            readValue = infoEnemies.front();

            const unsigned int movementSide = readValue - firstNumberSide;
            const JBTypes::Dir movementDir =
                JBTypesMethods::integerAsDirection(movementSide);

            // Length
            infoEnemies.erase(infoEnemies.begin());
            readValue = infoEnemies.front();
            const size_t length = static_cast <size_t>(readValue) -
                                  static_cast <size_t>(firstNumberLength);
            // Color
            infoEnemies.erase(infoEnemies.begin());
            readValue = infoEnemies.front();
            const unsigned int charColor = readValue - firstNumberColor;
            const JBTypes::Color color =
                static_cast <JBTypes::Color>(charColor);
            infoEnemies.erase(infoEnemies.begin());

            std::shared_ptr <Enemy> enemy;
            switch (typeOfEnemy) {
            case 0:
                enemy = std::make_shared <Laser>(
                    color,
                    Map::getBlockCoords(currentIndex, width, depth),
                    dir,
                    length
                    );
                break;
            case 1:
                enemy = std::make_shared <ThornBall>(
                    Map::getBlockCoords(currentIndex, width, depth),
                    dir,
                    movementDir,
                    length
                    );
                break;
            case 2:
                enemy = std::make_shared <DarkBall>(
                    Map::getBlockCoords(currentIndex, width, depth),
                    dir,
                    movementDir,
                    length
                    );
                break;
            default:
                enemy = std::make_shared <Laser>(
                    color,
                    Map::getBlockCoords(currentIndex, width, depth),
                    dir,
                    length
                    );
                break;
            }
            Map::EnemyInfo enemyInfo;
            enemyInfo.enemy = enemy;
            enemyInfo.index = static_cast <size_t>(currentIndex);
            enemyInfo.type = static_cast <Map::EnemyTypes>(typeOfEnemy);
            mapInfo.enemiesInfo.push_back(enemyInfo);
            counterWithoutEnemiesBuffer.clear();
            ++currentIndex; // hack
        }
        previousReadValue = readValue;
    }

    // SPECIAL PART
    std::cout << "decompress special" << std::endl;
    std::string infoSpecial;
    std::string counterWithoutSpecialBuffer;
    currentIndex = 0;
    previousReadValue = 0;

    file >> infoSpecial;
    while (!infoSpecial.empty()) {

        readValue = infoSpecial.front();
        if (readValue == nextBlockAction) {
            infoSpecial.erase(infoSpecial.begin());
            ++currentIndex; // We increment the cursor to the next block
        } else if (readValue >= firstNumberWithoutAnyItems) {
            counterWithoutSpecialBuffer.push_back(readValue);
            infoSpecial.erase(infoSpecial.begin());

            if (
                previousReadValue < firstNumberWithoutAnyItems &&
                previousReadValue >= firstNumberType
                ) {
                ++currentIndex; // We increment the cursor to the next block
            }
        } else {
            if (!counterWithoutSpecialBuffer.empty()) {
                counterWithoutSpecialBuffer =
                    substractOffset(counterWithoutSpecialBuffer,
                                    firstNumberWithoutAnyItems);
                currentIndex += convertToBase10(counterWithoutSpecialBuffer,
                                                nbOfCharactersWithoutItems);
            }
            // direction
            const unsigned int side = readValue - firstNumberSide;
            const JBTypes::Dir dir =
                JBTypesMethods::integerAsDirection(side);

            // special type
            infoSpecial.erase(infoSpecial.begin());
            readValue = infoSpecial.front();
            const unsigned int typeOfSpecial = readValue - firstNumberType;
            infoSpecial.erase(infoSpecial.begin());
            std::shared_ptr <Special> special;
            const auto& blockPtr = mapInfo.blocks.at(currentIndex);
            Map::SpecialTypes specialType = Map::SpecialTypes::SwitchButton;
            switch (typeOfSpecial) {
            case 0:
                special = std::make_shared <SwitchButton>(
                    JBTypes::Color::Red,
                    dir,
                    Map::getBlockCoords(currentIndex, width, depth)
                    );
                specialType = Map::SpecialTypes::SwitchButton;
                break;
            case 1:
                special = std::make_shared <SwitchButton>(
                    JBTypes::Color::Green,
                    dir,
                    Map::getBlockCoords(currentIndex, width, depth)
                    );
                specialType = Map::SpecialTypes::SwitchButton;
                break;
            case 2:
                special = std::make_shared <SwitchButton>(
                    JBTypes::Color::Blue,
                    dir,
                    Map::getBlockCoords(currentIndex, width, depth)
                    );
                specialType = Map::SpecialTypes::SwitchButton;
                break;
            case 3:
                special = std::make_shared <SwitchButton>(
                    JBTypes::Color::Yellow,
                    dir,
                    Map::getBlockCoords(currentIndex, width, depth)
                    );
                specialType = Map::SpecialTypes::SwitchButton;
                break;
            case 4:
                special = std::make_shared <Teleporter>(
                    JBTypes::Color::Red,
                    dir,
                    Map::getBlockCoords(currentIndex, width, depth)
                    );
                specialType = Map::SpecialTypes::Teleporter;
                break;
            case 5:
                special = std::make_shared <Teleporter>(
                    JBTypes::Color::Green,
                    dir,
                    Map::getBlockCoords(currentIndex, width, depth)
                    );
                specialType = Map::SpecialTypes::Teleporter;
                break;
            case 6:
                special = std::make_shared <Teleporter>(
                    JBTypes::Color::Blue,
                    dir,
                    Map::getBlockCoords(currentIndex, width, depth)
                    );
                specialType = Map::SpecialTypes::Teleporter;
                break;
            case 7:
                special = std::make_shared <Teleporter>(
                    JBTypes::Color::Yellow,
                    dir,
                    Map::getBlockCoords(currentIndex, width, depth)
                    );
                specialType = Map::SpecialTypes::Teleporter;
                break;
            default:
                break;
            }
            Map::SpecialInfo specialInfo;
            specialInfo.special = special;
            specialInfo.index = static_cast <size_t>(currentIndex);
            specialInfo.type = specialType;
            mapInfo.specialInfo.push_back(specialInfo);
            counterWithoutSpecialBuffer.clear();
        }
        previousReadValue = readValue;
    }

    return mapInfo;
}

std::string MapGenerator::convertToBase (unsigned long long int number, unsigned char base) {
    std::string convertedNumber;

    while (number > 0) {
        const unsigned int remainder = number % base;
        convertedNumber.insert(convertedNumber.begin(), remainder);

        number = number / base;
    }

    return convertedNumber;
}

unsigned long long int MapGenerator::convertToBase10 (const std::string& s, unsigned int base) {
    unsigned long long int value = 0;
    std::string copyS = s;
    while (copyS.length() > 0) {
        const unsigned int number = static_cast <unsigned int>(copyS.front());
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
    return;
}


void MapGenerator::compress (std::ifstream& input) {
    std::ofstream output("outMap.txt");

    const unsigned int width = readUnsignedInt(input);
    const unsigned int depth = readUnsignedInt(input);
    const unsigned int height = readUnsignedInt(input);
    const unsigned int beginX = readUnsignedInt(input);
    const unsigned int beginZ = readUnsignedInt(input);
    const unsigned int beginY = readUnsignedInt(input);

    writeUnsignedInt(output, width);
    writeSeparator(output);

    writeUnsignedInt(output, depth);
    writeSeparator(output);

    writeUnsignedInt(output, height);
    writeSeparator(output);

    writeEndLine(output);

    writeUnsignedInt(output, beginX);
    writeSeparator(output);

    writeUnsignedInt(output, beginZ);
    writeSeparator(output);

    writeUnsignedInt(output, beginY);
    writeSeparator(output);

    writeEndLine(output);

    std::string trash;

    const auto getBlocksString =
        [&input] (unsigned int width,
                  unsigned int height,
                  unsigned int depth)->std::string {
            std::cout << "compress blocks" << std::endl;
            readingString(input);

            unsigned int currentType = readUnsignedInt(input);
            unsigned int counter = 1;
            std::string blocksString;
            const std::vector <unsigned int> blockWithSpecialParams { 4, 6 };
            for (unsigned int i = 1; i < width * depth * height; ++i) {
                const unsigned int readValue = readUnsignedInt(input);

                if (readValue != currentType) {
                    if (counter > 1) {
                        std::string stringToWrite = convertToBase(
                            counter, nbOfCharactersUsedForNumbers);
                        stringToWrite =
                            applyOffset(stringToWrite, firstNumberOfBlock);
                        blocksString.append(stringToWrite);
                    }
                    if (counter > 0) {
                        blocksString.append(std::to_string(currentType));
                    }
                    currentType = readValue;
                    counter = 0;
                }
                counter++;
                if (
                    std::find(blockWithSpecialParams.begin(),
                              blockWithSpecialParams.end(), readValue) !=
                    blockWithSpecialParams.end()
                    ) {
                    currentType = readValue;
                    blocksString.append(std::to_string(currentType));
                    const std::string blockParams = readingString(input);
                    const std::string blockParamsWithOffset = substractOffset(
                        blockParams, static_cast <unsigned int>('a'));
                    const unsigned int numberBase10 =
                        convertToBase10(blockParamsWithOffset, 2);
                    std::string numberBase64 =
                        convertToBase(numberBase10, 64);
                    numberBase64 = applyOffset(numberBase64, firstNumberParams);
                    blocksString.append(numberBase64);
                    counter = 0;
                }
            }
            if (counter > 1) {
                std::string stringToWrite = convertToBase(
                    counter, nbOfCharactersUsedForNumbers);
                stringToWrite = applyOffset(stringToWrite, firstNumberOfBlock);
                blocksString.append(stringToWrite);
            }
            blocksString.append(std::to_string(currentType));
            return blocksString;
        };

    // BLOCKS PART
    std::cout << "compress blocks" << std::endl;
    const std::string blocksString = getBlocksString(width, height, depth);
    writeString(output, blocksString);
    writeEndLine(output);

    // OBJECTS PART
    std::cout << "compress items" << std::endl;
    input >> trash;
    std::string readString;
    unsigned int counterWithoutItems = 0;

    for (unsigned int i = 0; i < width * depth * height; ++i) {
        input >> readString;
        if (std::isdigit(readString.at(0)) != 0) {
            ++counterWithoutItems;
        } else {
            if (counterWithoutItems > 0) {
                std::string stringToWrite = convertToBase(
                    counterWithoutItems, nbOfCharactersWithoutItems);
                stringToWrite =
                    applyOffset(stringToWrite, firstNumberWithoutAnyItems);
                output << stringToWrite;
                counterWithoutItems = 0;
            } else if (i > 0) {
                // we need to differentiate this block and the future block
                // if two blocks with items are side-by-side
                // and we do not apply this separator character if it's the
                // the FIRST block
                output << nextBlockAction;
            }

            for (unsigned char c : readString) {
                std::cout << c << " is read" << std::endl;
                unsigned char charToWrite;
                switch (c) {
                case 'N': charToWrite = firstNumberSide; break;
                case 'S': charToWrite = firstNumberSide + 1; break;
                case 'E': charToWrite = firstNumberSide + 2; break;
                case 'W': charToWrite = firstNumberSide + 3; break;
                case 'U': charToWrite = firstNumberSide + 4; break;
                case 'D': charToWrite = firstNumberSide + 5; break;
                case 'K': charToWrite = firstNumberType; break;
                case 'I': charToWrite = firstNumberType + 1; break;
                case 'C': charToWrite = firstNumberType + 2; break;
                default: std::cerr << "Unknown item character: "
                                   << c << std::endl;
                    charToWrite = 0; break;
                }
                output << charToWrite;
            }
        }
    }
    if (counterWithoutItems > 0) {
        std::string stringToWrite = convertToBase(
            counterWithoutItems, nbOfCharactersWithoutItems);
        stringToWrite = applyOffset(stringToWrite, firstNumberWithoutAnyItems);
        output << stringToWrite;
        counterWithoutItems = 0;
    }

    output << std::endl;
    // ENEMIES PART
    std::cout << "compress enemies" << std::endl;
    input >> trash;
    readString.clear();

    for (unsigned int i = 0; i < width * depth * height; ++i) {
        input >> readString;
        if (std::isdigit(readString.at(0)) != 0) {
            ++counterWithoutItems;
        } else {
            if (counterWithoutItems > 0) {
                std::string stringToWrite = convertToBase(
                    counterWithoutItems, nbOfCharactersWithoutItems);
                stringToWrite =
                    applyOffset(stringToWrite, firstNumberWithoutAnyItems);
                output << stringToWrite;
                counterWithoutItems = 0;
            } else if (i > 0) {
                // we need to differentiate this block and the future block
                // if two blocks with items are side-by-side
                // and we do not apply this separator character if it's the
                // the FIRST block
                output << nextBlockAction;
            }

            if (readString.length() != 5) {
                std::cerr << "An enemy is represent by 5 chars" << std::endl;
            }

            // side of enemy
            unsigned char charToWrite;
            switch (readString.at(0)) {
            case 'N': charToWrite = firstNumberSide; break;
            case 'S': charToWrite = firstNumberSide + 1; break;
            case 'E': charToWrite = firstNumberSide + 2; break;
            case 'W': charToWrite = firstNumberSide + 3; break;
            case 'U': charToWrite = firstNumberSide + 4; break;
            case 'D': charToWrite = firstNumberSide + 5; break;
            default:
                std::cerr << "Unknown side character: "
                          << readString.at(0) << std::endl;
                charToWrite = 0;
                break;
            }
            output << charToWrite;

            // type of enemy
            switch (readString.at(1)) {
            case 'L': charToWrite = firstNumberType; break;
            case 'T': charToWrite = firstNumberType + 1; break;
            case 'B': charToWrite = firstNumberType + 2; break;
            default:
                std::cerr << "Unknown type character: "
                          << readString.at(1) << std::endl;
                charToWrite = 0;
                break;
            }
            output << charToWrite;

            // side of movement
            switch (readString.at(2)) {
            case 'N': charToWrite = firstNumberSide; break;
            case 'S': charToWrite = firstNumberSide + 1; break;
            case 'E': charToWrite = firstNumberSide + 2; break;
            case 'W': charToWrite = firstNumberSide + 3; break;
            case 'U': charToWrite = firstNumberSide + 4; break;
            case 'D': charToWrite = firstNumberSide + 5; break;
            default:
                std::cerr << "Unknown side character: "
                          << readString.at(2) << std::endl;
                charToWrite = 0;
                break;
            }
            output << charToWrite;

            // Length
            constexpr char AtoFirstNumberLength = 33 - 65 + 1; // A=1, not 0
            const char outputLenght = readString.at(3) + AtoFirstNumberLength;
            output << outputLenght;

            // Color
            constexpr char AtoFirstNumberColor = 33 - 65; // A = None
            const char outputColor = readString.at(4) + AtoFirstNumberColor;
            output << outputColor;
        }
    }
    if (counterWithoutItems > 0) {
        std::string stringToWrite = convertToBase(
            counterWithoutItems, nbOfCharactersWithoutItems);
        stringToWrite =
            applyOffset(stringToWrite, firstNumberWithoutAnyItems);
        output << stringToWrite;
        counterWithoutItems = 0;
    }

    output << std::endl;
    // SPECIALS PART
    std::cout << "compress specials" << std::endl;
    input >> trash;
    readString.clear();

    for (unsigned int i = 0; i < width * depth * height; ++i) {
        input >> readString;
        if (std::isdigit(readString.at(0)) != 0) {
            ++counterWithoutItems;
        } else {
            if (counterWithoutItems > 0) {
                std::string stringToWrite = convertToBase(
                    counterWithoutItems, nbOfCharactersWithoutItems);
                stringToWrite = applyOffset(stringToWrite,
                                            firstNumberWithoutAnyItems);
                output << stringToWrite;
                counterWithoutItems = 0;
            } else if (i > 0) {
                // we need to differentiate this block and the future block
                // if two blocks with items are side-by-side
                // and we do not apply this separator character if it's the
                // the FIRST block
                output << nextBlockAction;
            }

            if (readString.length() != 2) {
                std::cerr << "A special is represent by 2 chars" << std::endl;
            }
            // side of special
            unsigned char charToWrite;
            switch (readString.at(0)) {
            case 'N': charToWrite = firstNumberSide; break;
            case 'S': charToWrite = firstNumberSide + 1; break;
            case 'E': charToWrite = firstNumberSide + 2; break;
            case 'W': charToWrite = firstNumberSide + 3; break;
            case 'U': charToWrite = firstNumberSide + 4; break;
            case 'D': charToWrite = firstNumberSide + 5; break;
            default:
                std::cerr << "Unknown side character: "
                          << readString.at(0) << std::endl;
                charToWrite = 0;
                break;
            }
            output << charToWrite;

            // type of special
            switch (readString.at(1)) {
            case 'A': charToWrite = firstNumberType; break;
            case 'B': charToWrite = firstNumberType + 1; break;
            case 'C': charToWrite = firstNumberType + 2; break;
            case 'D': charToWrite = firstNumberType + 3; break;
            case 'E': charToWrite = firstNumberType + 4; break;
            case 'F': charToWrite = firstNumberType + 5; break;
            case 'G': charToWrite = firstNumberType + 6; break;
            case 'H': charToWrite = firstNumberType + 7; break;
            default:
                std::cerr << "Unknown type character: "
                          << readString.at(1) << std::endl;
                charToWrite = 0;
                break;
            }
            output << charToWrite;
        }
    }
    if (counterWithoutItems > 0) {
        std::string stringToWrite = convertToBase(
            counterWithoutItems, nbOfCharactersWithoutItems);
        stringToWrite =
            applyOffset(stringToWrite, firstNumberWithoutAnyItems);
        output << stringToWrite;
        counterWithoutItems = 0;
    }
    output.close();
}

Map::BlockTypes MapGenerator::uintToBlockType (unsigned int number) {
    Map::BlockTypes type;
    switch (number) {
    case 0: type = Map::BlockTypes::None; break;
    case 1: type = Map::BlockTypes::Base; break;
    case 2: type = Map::BlockTypes::Fire; break;
    case 3: type = Map::BlockTypes::Ice; break;
    case 4: type = Map::BlockTypes::Sharp; break;
    case 5: type = Map::BlockTypes::Brittle; break;
    case 6: type = Map::BlockTypes::Jump; break;
    case 7: type = Map::BlockTypes::Ghost; break;
    default: type = Map::BlockTypes::None; break;
    }
    return type;
}

std::string MapGenerator::applyOffset (const std::string& s, int offset) {
    std::string offsetString = s;
    for (char& c : offsetString) {
        c += offset;
    }
    return offsetString;
}

void MapGenerator::verificationMap (std::ifstream& input, const Map& map) {
    std::ofstream output("outMapVerification.txt");

    output << map.width() << " " << map.depth() << " "
           << map.height() << std::endl;

    output << map.beginX() << " " << map.beginZ() << " "
           << map.beginY() << std::endl << std::endl << "BLOCKS" << std::endl;

    const std::function <unsigned char(size_t)>
    getDirection = [] (size_t face) {
                       unsigned char direction;
                       switch (face) {
                       case 0: direction = 'N'; break;
                       case 1: direction = 'S'; break;
                       case 2: direction = 'E'; break;
                       case 3: direction = 'W'; break;
                       case 4: direction = 'U'; break;
                       case 5: direction = 'D'; break;
                       default: direction = '?'; break;
                       }
                       return direction;
                   };

    std::cout << "Verification Blocks..." << std::endl;
    for (unsigned int y = 0; y < map.height(); y++) {
        for (unsigned int z = 0; z < map.depth(); z++) {
            for (unsigned int x = 0; x < map.width(); x++) {
                const CstBlock_sptr& block =
                    map.getBlock(x, y, z);
                unsigned int typeOfBlock = 0;

                if (block) {
                    typeOfBlock = static_cast <unsigned int>(map.getType({ x, y, z }));
                    output << typeOfBlock;
                } else {
                    output << static_cast <unsigned int>(Map::BlockTypes::None);
                }
                if (typeOfBlock == 4 || typeOfBlock == 6) {
                    output << " ";
                    const auto infos = block->faceInfo();
                    for (bool info : infos) {
                        if (info) output << "b";
                        else output << "a";
                    }
                }
                if (x != map.width() - 1) output << " ";
            }
            output << std::endl;
        }
        output << std::endl;
    }
    output << "OBJECTS" << std::endl;
    std::cout << "Verification Items..." << std::endl;
    for (unsigned int y = 0; y < map.height(); y++) {
        for (unsigned int z = 0; z < map.depth(); z++) {
            for (unsigned int x = 0; x < map.width(); x++) {
                const CstBlock_sptr& block =
                    map.getBlock(x, y, z);
                if (block) {
                    bool found = false;
                    for (size_t i = 0; i < Block::itemsNumber; ++i) {
                        const auto item = block->item(i);
                        if (item) {

                            std::string s;
                            s.push_back(getDirection(i));
                            if (
                                item->getCategory() ==
                                Item::CategoryOfItems::Key
                                ) {
                                output << "K" << s;
                            } else if (
                                item->getCategory() ==
                                Item::CategoryOfItems::Coin
                                ) {
                                output << "I" << s;
                            } else if (
                                item->getCategory() ==
                                Item::CategoryOfItems::Clock
                                ) {
                                output << "C" << s;
                            }
                            found = true;
                        }

                    }
                    if (!found) {

                        unsigned int typeOfBlock =
                            static_cast <unsigned int>(
                                map.getType({ x, y, z }));
                        output << typeOfBlock;
                    }
                    if (x != map.width() - 1) output << " ";
                } else {
                    output << static_cast <unsigned int>(Map::BlockTypes::None);
                    if (x != map.width() - 1) output << " ";
                }
            }
            output << std::endl;
        }
        output << std::endl;
    }

    // Enemies verification
    output << "ENEMIES" << std::endl;
    std::cout << "Verification Enemies..." << std::endl;
    size_t currentInfo = 0;
    size_t currentIndex = 0;
    for (unsigned int y = 0; y < map.height(); y++) {
        for (unsigned int z = 0; z < map.depth(); z++) {
            for (unsigned int x = 0; x < map.width(); x++) {
                if (
                    currentInfo >= map.getEnemiesInfo().size() ||
                    currentIndex < map.getEnemiesInfo().at(currentInfo).index
                    ) {
                    output << static_cast <unsigned int>(Map::BlockTypes::None);
                } else {
                    const JBTypes::Dir& dir =
                        map.getEnemiesInfo().at(currentInfo).enemy->direction();
                    const auto typeOfEnemy =
                        map.getEnemiesInfo().at(currentInfo).type;
                    const JBTypes::Dir moveDir = map.getEnemiesInfo().
                                                 at(currentInfo).enemy->movementDirection();
                    const size_t length =
                        map.getEnemiesInfo().at(currentInfo).enemy->length();
                    const auto color =
                        map.getEnemiesInfo().at(currentInfo).enemy->getColor();

                    unsigned int uintTypeOfEnemy =
                        static_cast <unsigned int>(typeOfEnemy);
                    unsigned char charToWriteType;
                    switch (uintTypeOfEnemy) {
                    case 0:
                        charToWriteType = 'L';
                        break;
                    case 1:
                        charToWriteType = 'T';
                        break;
                    case 2:
                        charToWriteType = 'B';
                        break;
                    default:
                        std::cerr << "Unknown value: "
                                  << uintTypeOfEnemy << std::endl;
                        charToWriteType = 0;
                        break;
                    }
                    const unsigned char charToWriteLength =
                        static_cast <char>(length) + 'A' - 1; // A = 1 not 0
                    const unsigned char charToWriteColor =
                        static_cast <char>(color) + 'A';

                    output << getDirection(static_cast <size_t>(dir))
                           << charToWriteType
                           << getDirection(static_cast <size_t>(moveDir))
                           << charToWriteLength
                           << charToWriteColor;
                    ++currentInfo;
                }
                if (x != map.width() - 1) output << " ";
                ++currentIndex;
            }
            output << std::endl;
        }
        output << std::endl;
    }

    // Specials verification
    output << "SPECIALS" << std::endl;
    std::cout << "Verification Specials..." << std::endl;
    currentInfo = 0;
    currentIndex = 0;
    for (unsigned int y = 0; y < map.height(); y++) {
        for (unsigned int z = 0; z < map.depth(); z++) {
            for (unsigned int x = 0; x < map.width(); x++) {
                if (
                    currentInfo >= map.getSpecialInfo().size() ||
                    currentIndex < map.getSpecialInfo().at(currentInfo).index
                    ) {
                    output << static_cast <unsigned int>(Map::BlockTypes::None);
                } else {
                    const JBTypes::Dir& dir = map.getSpecialInfo().at(currentInfo).special->direction();
                    const auto typeOfSpecial = map.getSpecialInfo().at(currentInfo).type;
                    const auto color = map.getSpecialInfo().at(currentInfo).special->getColor();

                    const unsigned int offsetType =
                        typeOfSpecial == Map::SpecialTypes::Teleporter
                        ? 4
                        : 0;
                    // -1 to count None in the Color type
                    const char charToWriteType =
                        static_cast <unsigned int>(typeOfSpecial)
                        * offsetType
                        + static_cast <unsigned int>(color) - 1
                        + 'A';
                    output << getDirection(static_cast <size_t>(dir))
                           << charToWriteType;
                    ++currentInfo;
                }
                if (x != map.width() - 1) output << " ";
                ++currentIndex;
            }
            output << std::endl;
        }
        output << std::endl;
    }

    output.close();

    std::ifstream inputV2("outMapVerification.txt");
    std::string lineV1;
    std::string lineV2;
    bool hasDifferences = false;
    unsigned int counterLines = 1;
    while (getline(input, lineV1) && getline(inputV2, lineV2)) {
        if (lineV1.compare(lineV2) != 0) {
            std::cout << "Differences found line " << counterLines << ":"
                      << std::endl;
            std::cout << "V1: " << lineV1 << std::endl;
            std::cout << "V2: " << lineV2 << std::endl;
            hasDifferences = true;
        }
        counterLines++;
    }
    if (hasDifferences) {
        std::cout << "Failed map test ... verify your map " <<
            "or the run length encoding" << std::endl;
    } else {
        std::cout << "Map test : Success !" << std::endl;
    }
    input.close();
    inputV2.close();
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

void MapGenerator::writeUnsignedInt (std::ofstream& output,
                                     unsigned int unsignedInt) {
    output << unsignedInt;
}

void MapGenerator::writeString (std::ofstream& output,
                                const std::string& string) {
    output << string;
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
            letterDigitBaseString.push_back(asciiToLetterDigitBase(c));
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
            asciiBaseString.push_back(letterDigitToAsciiBase(c));
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

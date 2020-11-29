/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MapGenerator.cpp
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

std::shared_ptr<Map> MapGenerator::loadMap(size_t mapNumber)
{
    std::shared_ptr<Map> map = nullptr;

    std::string mapFileToOpenV1 = "maps/map" +
        std::to_string(mapNumber) + ".txt";
    std::string mapFileToOpenV2 = "bin/maps/map" +
        std::to_string(mapNumber) + ".txt";
    const std::vector<std::string> fileNames {
        std::move(mapFileToOpenV1), std::move(mapFileToOpenV2)};

    bool foundFile = false;
    for (size_t i = 0; i < fileNames.size() && !foundFile; ++i) {
        std::ifstream mapFile;
        mapFile.open(fileNames.at(i));  //Opening file to read
        if (mapFile) {
            foundFile = true;
            map = std::make_shared<Map>(createMapInfo(mapFile));
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

Map::MapInfo MapGenerator::createMapInfo(std::ifstream& file)
{
    Map::MapInfo mapInfo{};

    unsigned int width;
    unsigned int deep;
    unsigned int height;

    file >> width;
    file >> deep;
    file >> height;

    mapInfo.width = width;
    mapInfo.height = height;
    mapInfo.deep = deep;

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
            if (counterBuffer.empty()){
                nbBlocksTowrite = 1;
            } else {
                counterBuffer = substractOffset(counterBuffer,firstNumberOfBlock);
                nbBlocksTowrite = convertToBase10(counterBuffer,
                                              nbOfCharactersUsedForNumbers);
            }
            const unsigned int typeOfBlock = readValue - firstKindOfBlock;
            for (unsigned int i = 0 ; i < nbBlocksTowrite ; ++i) {
                std::shared_ptr <Block> block = nullptr;
                const std::vector<unsigned int> blockWithSpecialParams {4,6};
                std::array<bool,6> specialParams
                {false,false,false,false,false,false};
                if( std::find( blockWithSpecialParams.begin(),
                    blockWithSpecialParams.end(), typeOfBlock) !=
                    blockWithSpecialParams.end()) {
                    const char readParam =  infoMap.front() - firstNumberParams;
                    infoMap.erase(infoMap.begin());
                    const std::string sParam =
                        convertToBase(static_cast<unsigned int>(readParam),2);
                    for(unsigned int i = 0 ; i < specialParams.size(); ++i){
                        const size_t diffSize =
                          specialParams.size()-sParam.size();
                        if ( i >=  diffSize){
                          if (sParam.at(i-diffSize) == 0) {
                            specialParams.at(i)  = false;
                          } else {
                              specialParams.at(i) = true;
                          }
                        } else {
                            specialParams.at(i) = false;
                        }
                    }
                }
                switch (typeOfBlock) {
                    case 0:
                        break ;
                    case 1:
                        block = std::make_shared<BaseBlock>(); break ;
                    case 2:
                        block = std::make_shared<FireBlock>(); break ;
                    case 3:
                        block = std::make_shared<IceBlock>(); break ;
                    case 4:
                        block = std::make_shared<SharpBlock>(specialParams);
                        break ;
                    case 5:
                        block = std::make_shared<BrittleBlock>(); break ;
                    case 6:
                        block = std::make_shared<JumpBlock>(specialParams);
                        break ;
                    case 7:
                        block = std::make_shared<GhostBlock>(); break;
                    default :
                        break;
                }
                if (block) {
                    mapInfo.blocksInfo.push_back(
                        {mapInfo.blocks.size(), uintToBlockType(typeOfBlock) });
                }
                mapInfo.blocks.push_back(block);
                counterBuffer.clear();
            }
        }
    }

    // OBJECTS PART
    std::cout << "decompress objects" << std::endl;
    std::string infoObjects;
    std::string counterWithoutObjectsBuffer;
    unsigned int currentIndex = 0;
    char previousReadValue = 0;

    file >> infoObjects;
    while (!infoObjects.empty()) {

        readValue = infoObjects.front();
        if (readValue == nextBlockAction){
            infoObjects.erase(infoObjects.begin());
            ++currentIndex;//We increment the cursor to the next block
        } else if (readValue >= firstNumberWithoutAnyObjects) {
            counterWithoutObjectsBuffer.push_back(readValue);
            infoObjects.erase(infoObjects.begin());

            if (previousReadValue < firstNumberWithoutAnyObjects &&
                previousReadValue >= firstNumberType) {
                ++currentIndex; //We increment the cursor to the next block
            }
        } else {
            if (!counterWithoutObjectsBuffer.empty()){
                counterWithoutObjectsBuffer = substractOffset(counterWithoutObjectsBuffer,
                                firstNumberWithoutAnyObjects);
                currentIndex += convertToBase10(counterWithoutObjectsBuffer,
                                                nbOfCharactersWithoutObjects);
            }

            const unsigned int typeOfObject = readValue - firstNumberType;

            infoObjects.erase(infoObjects.begin());
            readValue = infoObjects.front();

            const unsigned int side = readValue - firstNumberSide;
            const JBTypes::Dir dir =
                    JBTypesMethods::integerAsDirection(side);

            Object::CategoryOfObjects category;
            switch (typeOfObject) {
                case 0:
                    category = Object::CategoryOfObjects::Key;
                    break;
                case 1:
                    category = Object::CategoryOfObjects::Coin;
                    break;
                case 2:
                    category = Object::CategoryOfObjects::Clock;
                    break;
                default :
                    category = Object::CategoryOfObjects::Coin;
                    break;
            }

            mapInfo.blocks.at(currentIndex)->createObject(category,dir);
            counterWithoutObjectsBuffer.clear();
            infoObjects.erase(infoObjects.begin(), infoObjects.begin()+1);
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
        if (readValue == nextBlockAction){
            infoEnemies.erase(infoEnemies.begin());
            ++currentIndex;//We increment the cursor to the next block
        } else if (readValue >= firstNumberWithoutAnyObjects) {
            counterWithoutEnemiesBuffer.push_back(readValue);
            infoEnemies.erase(infoEnemies.begin());

            if (previousReadValue < firstNumberWithoutAnyObjects &&
                previousReadValue >= firstNumberType) {
                ++currentIndex; //We increment the cursor to the next block
            }
        } else {
            if (!counterWithoutEnemiesBuffer.empty()){
                counterWithoutEnemiesBuffer = substractOffset(counterWithoutEnemiesBuffer,
                                firstNumberWithoutAnyObjects);
                currentIndex += convertToBase10(counterWithoutEnemiesBuffer,
                                                nbOfCharactersWithoutObjects);
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
            const size_t length = readValue - firstNumberLength;
            // Color
            infoEnemies.erase(infoEnemies.begin());
            readValue = infoEnemies.front();
            const unsigned int charColor = readValue - firstNumberColor;
            const JBTypes::Color color = 
                static_cast<JBTypes::Color>(charColor);

            //Map::EnemyTypes category;
            std::shared_ptr<Enemy> enemy;
            const auto& blockPtr = mapInfo.blocks.at(currentIndex);
            switch (typeOfEnemy) {
                case 0:
                    //category = Map::EnemyTypes::Laser;
                    enemy = std::make_shared<Laser>(
                        color,
                        *blockPtr,
                        Map::getBlockCoords(currentIndex,width,deep),
                        dir,
                        length
                    );
                    break;
                case 1:
                    //category = Map::EnemyTypes::ThornBall;
                    enemy = std::make_shared<ThornBall>(
                        *blockPtr,
                        Map::getBlockCoords(currentIndex,width,deep),
                        dir,
                        movementDir,
                        length
                    );
                    break;
                case 2:
                    //category = Map::EnemyTypes::ThornBall;
                    enemy = std::make_shared<DarkBall>(
                        *blockPtr,
                        Map::getBlockCoords(currentIndex,width,deep),
                        dir,
                        movementDir,
                        length
                    );
                    break;
                    //category = Map::EnemyTypes::DarkBall;
                    enemy = std::make_shared<DarkBall>(
                        *blockPtr,
                        Map::getBlockCoords(currentIndex,width,deep),
                        dir,
                        movementDir,
                        length
                    );
                    break;
                default :
                    //category = Map::EnemyTypes::Laser;
                    enemy = std::make_shared<Laser>(
                        color,
                        *blockPtr,
                        Map::getBlockCoords(currentIndex,width,deep),
                        dir,
                        length
                    );
                    break;
            }
            Map::EnemyInfo enemyInfo;
            enemyInfo.enemy = enemy;
            enemyInfo.index = static_cast<size_t>(currentIndex);
            enemyInfo.type = static_cast<Map::EnemyTypes>(typeOfEnemy);
            mapInfo.enemiesInfo.push_back(enemyInfo);
            counterWithoutEnemiesBuffer.clear();
        }
        previousReadValue= readValue;
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
        if (readValue == nextBlockAction){
            infoSpecial.erase(infoSpecial.begin());
            ++currentIndex;//We increment the cursor to the next block
        } else if (readValue >= firstNumberWithoutAnyObjects) {
            counterWithoutSpecialBuffer.push_back(readValue);
            infoSpecial.erase(infoSpecial.begin());

            if (previousReadValue < firstNumberWithoutAnyObjects &&
                previousReadValue >= firstNumberType) {
                ++currentIndex; //We increment the cursor to the next block
            }
        } else {
            if (!counterWithoutSpecialBuffer.empty()){
                counterWithoutSpecialBuffer = substractOffset(counterWithoutSpecialBuffer,
                                firstNumberWithoutAnyObjects);
                currentIndex += convertToBase10(counterWithoutSpecialBuffer,
                                                nbOfCharactersWithoutObjects);
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
            std::shared_ptr<Special> special;
            const auto& blockPtr = mapInfo.blocks.at(currentIndex);
            Map::SpecialTypes specialType = Map::SpecialTypes::SwitchButton;
            switch (typeOfSpecial) {
                case 0:
                    special = std::make_shared<SwitchButton>(
                        JBTypes::Color::Red,
                        *blockPtr,
                        dir);
                    specialType = Map::SpecialTypes::SwitchButton;
                    break;
                case 1:
                    special = std::make_shared<SwitchButton>(
                        JBTypes::Color::Green,
                        *blockPtr,
                        dir);
                    specialType = Map::SpecialTypes::SwitchButton;
                    break;
                case 2:
                    special = std::make_shared<SwitchButton>(
                        JBTypes::Color::Blue,
                        *blockPtr,
                        dir);
                    specialType = Map::SpecialTypes::SwitchButton;
                    break;
                case 3:
                    special = std::make_shared<SwitchButton>(
                        JBTypes::Color::Yellow,
                        *blockPtr,
                        dir);
                    specialType = Map::SpecialTypes::SwitchButton;
                    break;
                case 4:
                    special = std::make_shared<Teleporter>(
                        JBTypes::Color::Red,
                        *blockPtr,
                        dir);
                    specialType = Map::SpecialTypes::Teleporter;
                    break;
                case 5:
                    special = std::make_shared<Teleporter>(
                        JBTypes::Color::Green,
                        *blockPtr,
                        dir);
                    specialType = Map::SpecialTypes::Teleporter;
                    break;
                case 6:
                    special = std::make_shared<Teleporter>(
                        JBTypes::Color::Blue,
                        *blockPtr,
                        dir);
                    specialType = Map::SpecialTypes::Teleporter;
                    break;
                case 7:
                    special = std::make_shared<Teleporter>(
                        JBTypes::Color::Yellow,
                        *blockPtr,
                        dir);
                    specialType = Map::SpecialTypes::Teleporter;
                    break;
                default :
                    break;
            }
            Map::SpecialInfo specialInfo;
            specialInfo.special = special;
            specialInfo.index = static_cast<size_t>(currentIndex);
            specialInfo.type = specialType;
            mapInfo.specialInfo.push_back(specialInfo);
            counterWithoutSpecialBuffer.clear();
        }
        previousReadValue= readValue;
    }

    return mapInfo;
}

std::string MapGenerator::convertToBase(unsigned int number, unsigned char base) {
    std::string convertedNumber;

    while (number > 0 ) {
        const unsigned int remainder = number % base;
        convertedNumber.insert( convertedNumber.begin(),
                remainder);

        number = number / base ;
    }

    return convertedNumber;
}

unsigned int MapGenerator::convertToBase10(const std::string& s,                                            
                                           unsigned int base) {
    unsigned int value = 0;
    std::string copyS = s;
    while (copyS.length() > 0 ) {
        const unsigned int number = static_cast<unsigned int> (copyS.front());
        value += 
            number * static_cast<unsigned int> (pow(base, copyS.length() - 1));
        copyS.erase(copyS.begin());
    }
    return value;
}

std::string MapGenerator::substractOffset(const std::string& s, int offset) {
    std::string offsetString = s;
    for (char& c : offsetString) {
        c -= offset;
    }
    return offsetString;
 }

void MapGenerator::compress(std::ifstream& input) {
    std::ofstream output ("outMap.txt");

    const unsigned int width = readUnsignedInt(input);
    const unsigned int deep = readUnsignedInt(input);
    const unsigned int height = readUnsignedInt(input);
    const unsigned int beginX = readUnsignedInt(input);
    const unsigned int beginZ = readUnsignedInt(input);
    const unsigned int beginY = readUnsignedInt(input);

    writeUnsignedInt(output, width);
    writeSeparator(output);

    writeUnsignedInt(output, deep);
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

    const auto getBlocksString = [&input](unsigned int width,
                                          unsigned int height,
                                          unsigned int deep) -> std::string {
        std::cout << "compress blocks" << std::endl;
        readingString(input);

        unsigned int currentType = readUnsignedInt(input);
        unsigned int counter = 1;
        std::string blocksString;
        const std::vector<unsigned int> blockWithSpecialParams{4, 6};
        for (unsigned int i = 1; i < width * deep * height; ++i)
        {
            const unsigned int readValue = readUnsignedInt(input);

            if (readValue != currentType)
            {
                if (counter > 1)
                {
                    std::string stringToWrite = convertToBase(
                        counter, nbOfCharactersUsedForNumbers);
                    stringToWrite = 
                        applyOffset(stringToWrite, firstNumberOfBlock);
                    blocksString.append(stringToWrite);
                }
                if (counter > 0)
                {
                    blocksString.append(std::to_string(currentType));
                }
                currentType = readValue;
                counter = 0;
            }
            counter++;
            if (std::find(blockWithSpecialParams.begin(),
                          blockWithSpecialParams.end(), readValue) !=
                blockWithSpecialParams.end())
            {
                currentType = readValue;
                blocksString.append(std::to_string(currentType));
                const std::string blockParams = readingString(input);
                const std::string blockParamsWithOffset = substractOffset(
                    blockParams, static_cast<unsigned int>('a'));
                const unsigned int numberBase10 = 
                    convertToBase10(blockParamsWithOffset, 2);
                std::string numberBase64 = 
                    convertToBase(numberBase10, 64);
                numberBase64 = applyOffset(numberBase64, firstNumberParams);
                blocksString.append(numberBase64);
                counter = 0;
            }
        }
        if (counter > 1)
        {
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
    const std::string blocksString =getBlocksString(width,height,deep);
    writeString(output,blocksString);
    writeEndLine(output);

    // OBJECTS PART
    std::cout << "compress objects" << std::endl;
    input >> trash;
    std::string readString;
    unsigned int counterWithoutObjects = 0;

    for (unsigned int i = 0 ; i < width * deep * height ; ++i) {
        input >> readString;
        if (std::isdigit(readString.at(0)) != 0) {
            ++counterWithoutObjects;
        } else {
            if (counterWithoutObjects > 0) {
                std::string stringToWrite = convertToBase(
                        counterWithoutObjects, nbOfCharactersWithoutObjects);
                stringToWrite = 
                    applyOffset(stringToWrite, firstNumberWithoutAnyObjects) ;
                output << stringToWrite;
                counterWithoutObjects = 0;
            } else if (i > 0 ) {
                // we need to differentiate this block and the future block
                // if two blocks with objects are side-by-side
                // and we do not apply this separator character if it's the
                // the FIRST block
                output << nextBlockAction;
            }

            for (unsigned char c : readString) {
                std::cout << c << " is read" << std::endl;
                unsigned char charToWrite;
                switch (c) {
                    case 'N' : charToWrite = firstNumberSide; break;
                    case 'S' : charToWrite = firstNumberSide+1; break;
                    case 'E' : charToWrite = firstNumberSide+2; break;
                    case 'W' : charToWrite = firstNumberSide+3; break;
                    case 'U' : charToWrite = firstNumberSide+4; break;
                    case 'D' : charToWrite = firstNumberSide+5; break;
                    case 'K' : charToWrite = firstNumberType; break;
                    case 'I' : charToWrite = firstNumberType+1; break;
                    case 'C' : charToWrite = firstNumberType+2; break;
                    default : std::cerr << "Unknown object character: "
                                        << c << std::endl;
                              charToWrite = 0; break;
                }
                output << charToWrite;
            }
        }
    }
    if (counterWithoutObjects > 0) {
        std::string stringToWrite = convertToBase(
            counterWithoutObjects, nbOfCharactersWithoutObjects);
        stringToWrite = applyOffset(stringToWrite, firstNumberWithoutAnyObjects);
        output << stringToWrite;
        counterWithoutObjects = 0;
    }

    output << std::endl;
    // ENEMIES PART
    std::cout << "compress enemies" << std::endl;
    input >> trash;
    readString.clear();

    for (unsigned int i = 0 ; i < width * deep * height ; ++i) {
        input >> readString;
        if (std::isdigit(readString.at(0)) != 0) {
            ++counterWithoutObjects;
        } else {
            if (counterWithoutObjects > 0) {
                std::string stringToWrite = convertToBase(
                        counterWithoutObjects, nbOfCharactersWithoutObjects);
                stringToWrite = applyOffset(stringToWrite, firstNumberWithoutAnyObjects) ;
                output << stringToWrite;
                counterWithoutObjects = 0;
            } else if (i > 0 ) {
                // we need to differentiate this block and the future block
                // if two blocks with objects are side-by-side
                // and we do not apply this separator character if it's the
                // the FIRST block
                output << nextBlockAction;
            }

            if (readString.length() != 5) {
                std::cerr << "An enemy is represent by 5 chars" << std::endl;
            }
            // side of enemy
            unsigned char charToWrite;
            switch (readString.at(0))
            {
            case 'N': charToWrite = firstNumberSide; break;
            case 'S': charToWrite = firstNumberSide + 1; break;
            case 'E': charToWrite = firstNumberSide + 2; break;
            case 'W': charToWrite = firstNumberSide + 3; break;
            case 'U' : charToWrite = firstNumberSide+4; break;
            case 'D' : charToWrite = firstNumberSide+5; break;
            default:
                std::cerr << "Unknown side character: "
                          << readString.at(0) << std::endl;
                charToWrite = 0;
                break;
            }
            output << charToWrite;

            // type of enemy
            switch (readString.at(1))
            {
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
            switch (readString.at(2))
            {
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

            //Length
            constexpr char AtoFirstNumberLength = 33 - 65 + 1; //A=1, not 0
            const char outputLenght = readString.at(3) + AtoFirstNumberLength;
            output << outputLenght;

            //Color
            constexpr char AtoFirstNumberColor = 33 - 65; //A=1, not 0
            output << readString.at(4) + AtoFirstNumberColor;
        }
    }
    if (counterWithoutObjects > 0) {
        std::string stringToWrite = convertToBase(
            counterWithoutObjects, nbOfCharactersWithoutObjects);
        stringToWrite = applyOffset(stringToWrite, firstNumberWithoutAnyObjects);
        output << stringToWrite;
        counterWithoutObjects = 0;
    }

    output << std::endl;
    // SPECIALS PART
    std::cout << "compress specials" << std::endl;
    input >> trash;
    readString.clear();

    for (unsigned int i = 0 ; i < width * deep * height ; ++i) {
        input >> readString;
        if (std::isdigit(readString.at(0)) != 0) {
            ++counterWithoutObjects;
        } else {
            if (counterWithoutObjects > 0) {
                std::string stringToWrite = convertToBase(
                        counterWithoutObjects, nbOfCharactersWithoutObjects);
                stringToWrite = applyOffset(stringToWrite, firstNumberWithoutAnyObjects);
                output << stringToWrite;
                counterWithoutObjects = 0;
            } else if (i > 0) {
                // we need to differentiate this block and the future block
                // if two blocks with objects are side-by-side
                // and we do not apply this separator character if it's the
                // the FIRST block
                output << nextBlockAction;
            }

            if (readString.length() != 2) {
                std::cerr << "A special is represent by 2 chars" << std::endl;
            }
            // side of special
            unsigned char charToWrite;
            switch (readString.at(0))
            {
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
            switch (readString.at(1))
            {
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
    if (counterWithoutObjects > 0) {
        std::string stringToWrite = convertToBase(
            counterWithoutObjects, nbOfCharactersWithoutObjects);
        stringToWrite = applyOffset(stringToWrite, firstNumberWithoutAnyObjects);
        output << stringToWrite;
        counterWithoutObjects = 0;
    }
    output.close();
}

 Map::BlockTypes MapGenerator::uintToBlockType(unsigned int number) 
 {
    Map::BlockTypes type;
    switch (number) {
        case 0 : type = Map::BlockTypes::None; break;
        case 1 : type = Map::BlockTypes::Base; break;
        case 2 : type = Map::BlockTypes::Fire; break;
        case 3 : type = Map::BlockTypes::Ice; break;
        case 4 : type = Map::BlockTypes::Sharp; break;
        case 5 : type = Map::BlockTypes::Brittle; break;
        case 6 : type = Map::BlockTypes::Jump; break;
        case 7 : type = Map::BlockTypes::Ghost; break;
        default : type = Map::BlockTypes::None; break;
    }
    return type;
 }

std::string MapGenerator::applyOffset(const std::string& s, int offset) {
    std::string offsetString = s;
    for (char& c : offsetString) {
        c += offset;
    }
    return offsetString;
 }

void MapGenerator::verificationMap(std::ifstream& input, const Map& map) 
{
    std::ofstream output ("outMapVerification.txt");

    output << map.width() << " " << map.deep() << " "
           << map.height() << std::endl ;

    output << map.beginX() << " " << map.beginZ() << " " 
        << map.beginY() << std::endl << std::endl << "BLOCKS" << std::endl;

    const std::function<unsigned char(size_t)>
        getDirection = [](size_t face) {
            unsigned char direction;
            switch (face)
            {
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
    for ( unsigned int y = 0 ; y < map.height() ; y++ ) {
        for ( unsigned int z = 0 ; z < map.deep() ; z++ ){
            for ( unsigned int x = 0 ; x < map.width() ; x++ ){
                const std::shared_ptr<const Block>& block = 
                    map.getBlock(x,y,z);
                unsigned int typeOfBlock = 0;

                if (block) {
                    typeOfBlock = static_cast<unsigned int> ( 
                        map.getType({x,y,z}));
                    output << static_cast<const unsigned int> (typeOfBlock) ;
                }
                else {
                    output << static_cast<const unsigned int>
                            (Map::BlockTypes::None);
                }
                if (typeOfBlock == 4 || typeOfBlock == 6) {
                    output << " ";
                    const auto infos = block->faceInfo();
                    for (bool info : infos) {
                        if (info) output << "b" ;
                        else output << "a" ;
                    }
                }
                if ( x != map.width() - 1 )  output << " ";
            }
            output << std::endl;
        }
        output << std::endl;
    }
    output << "OBJECTS" << std::endl;
    std::cout << "Verification Objects..." << std::endl;
    for ( unsigned int y = 0 ; y < map.height() ; y++ ) {
        for ( unsigned int z = 0 ; z < map.deep() ; z++ ){
            for ( unsigned int x = 0 ; x < map.width() ; x++ ){
                const std::shared_ptr<const Block>& block = 
                    map.getBlock(x,y,z);
                if (block) {
                    bool found = false;
                    for ( size_t i = 0 ; i < Block::objectsNumber; ++i ) {
                        const auto object = block->object(i);
                        if (object) {

                            std::string s;
                            s.push_back(getDirection(i));
                            if (object->getCategory() ==
                                    Object::CategoryOfObjects::Key) {
                                output << "K" << s;
                            }
                            else if (object->getCategory() ==
                                     Object::CategoryOfObjects::Coin) {
                                output << "I" << s;
                            }
                            else if (object->getCategory() ==
                                     Object::CategoryOfObjects::Clock) {
                                output << "C" << s;
                            }
                            found = true;
                        }

                    }
                    if (!found) {

                        unsigned int typeOfBlock =
                                static_cast<unsigned int> (
                                    map.getType({x,y,z}));
                        output << static_cast<const unsigned int> (typeOfBlock);
                    }
                    if (x != map.width() -1 ) output << " ";
                }
                else {
                    output << static_cast<const unsigned int>
                               (Map::BlockTypes::None);
                    if (x != map.width() -1 ) output << " ";
                }
            }
            output << std::endl;
        }
        output << std::endl;
    }

    //Enemies verification
    output << "ENEMIES" << std::endl;
    std::cout << "Verification Enemies..." << std::endl;
    size_t currentInfo = 0;
    size_t currentIndex = 0;
    for (unsigned int y = 0; y < map.height(); y++) {
        for (unsigned int z = 0; z < map.deep(); z++) {
            for (unsigned int x = 0; x < map.width(); x++) {
                if (currentInfo >= map.getEnemiesInfo().size() ||
                    currentIndex < map.getEnemiesInfo().at(currentInfo).index) {
                    output << static_cast<const unsigned int>
                               (Map::BlockTypes::None);
                } else {
                    const JBTypes::Dir& dir = 
                        map.getEnemiesInfo().at(currentInfo).enemy->direction();
                    const auto typeOfEnemy = 
                        map.getEnemiesInfo().at(currentInfo).type;
                    const JBTypes::Dir moveDir = map.getEnemiesInfo().
                        at(currentInfo).enemy->movementDirection();
                    const float length = 
                        map.getEnemiesInfo().at(currentInfo).enemy->length();
                    const auto color = 
                        map.getEnemiesInfo().at(currentInfo).enemy->getColor();

                    unsigned int uintTypeOfEnemy = 
                        static_cast<unsigned int>(typeOfEnemy);
                    unsigned char charToWriteType;
                    switch (uintTypeOfEnemy)
                    {
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
                        static_cast<char>(length) + 'A' - 1; // A = 1 not 0
                    const unsigned char charToWriteColor = 
                        static_cast<char>(color) + 'A';

                    output << getDirection(static_cast<size_t>(dir))
                        << charToWriteType
                        << getDirection(static_cast<size_t>(moveDir))
                        << charToWriteLength 
                        << charToWriteColor;
                    ++currentInfo;
                }
                if (x != map.width() -1 ) output << " ";
                ++currentIndex;
            }
            output << std::endl;
        }
        output << std::endl;
    }

    //Specials verification
    output << "SPECIALS" << std::endl;
    std::cout << "Verification Specials..." << std::endl;
    currentInfo = 0;
    currentIndex = 0;
    for (unsigned int y = 0; y < map.height(); y++) {
        for (unsigned int z = 0; z < map.deep(); z++) {
            for (unsigned int x = 0; x < map.width(); x++) {
                if (currentInfo >= map.getSpecialInfo().size() || 
                    currentIndex < map.getSpecialInfo().at(currentInfo).index) {
                    output << static_cast<const unsigned int>
                               (Map::BlockTypes::None);
                } else {
                    const JBTypes::Dir& dir = 
                      map.getSpecialInfo().at(currentInfo).special->direction();
                    const auto typeOfSpecial =
                      map.getSpecialInfo().at(currentInfo).type;
                    const auto color = 
                      map.getSpecialInfo().at(currentInfo).special->getColor();

                    const unsigned int offsetType =
                        typeOfSpecial == Map::SpecialTypes::Teleporter
                            ? 4
                            : 0;
                    // -1 to count None in the Color type
                    const char charToWriteType =
                    static_cast<unsigned int>(typeOfSpecial)
                            * offsetType
                            + static_cast<unsigned int>(color) - 1
                            + 'A';
                    output << getDirection(static_cast<size_t>(dir))
                        << charToWriteType;
                    ++currentInfo;
                }
                if (x != map.width() -1 ) output << " ";
                ++currentIndex;
            }
            output << std::endl;
        }
        output << std::endl;
    }

    output.close();

    std::ifstream inputV2 ("outMapVerification.txt");
    std::string lineV1;
    std::string lineV2;
    bool hasDifferences = false;
    unsigned int counterLines = 1;
    while(getline(input, lineV1) && getline(inputV2,lineV2))
    {
        if ( lineV1.compare(lineV2) != 0) {
            std::cout << "Differences found line " << counterLines <<":"
                      << std::endl;
            std::cout << "V1: " << lineV1 << std::endl;
            std::cout << "V2: " << lineV2 << std::endl;
            hasDifferences = true;
        }
        counterLines++;
    }
    if (hasDifferences) {
        std::cout << "Failed map test ... verify your map " <<
                     "or the run length encoding"<< std::endl;
    } else {
        std::cout << "Map test : Success !" << std::endl;
    }
    input.close();
    inputV2.close();
}

unsigned int MapGenerator::readUnsignedInt(std::ifstream &input) {
    unsigned int readValue;
    input >> readValue;
    return readValue;
}

std::string MapGenerator::readingString(std::ifstream &input) {
    std::string readValue;
    input >> readValue;
    return readValue;
}

void MapGenerator::writeSeparator(std::ofstream &output) {
    output << " ";
}

void MapGenerator::writeEndLine(std::ofstream &output) {
    output << std::endl;
}

void MapGenerator::writeUnsignedInt(std::ofstream &output,
                                    unsigned int unsignedInt) {
    output << unsignedInt;
}

void MapGenerator::writeString(std::ofstream &output,
                 const std::string &string) {
    output << string;
}
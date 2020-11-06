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
                substractOffset(counterBuffer,firstNumberOfBlock);
                nbBlocksTowrite = convertToBase10(counterBuffer,
                                              nbOfCharactersUsedForNumbers);
            }
            const unsigned int typeOfBlock = readValue - firstKindOfBlock;
            for (unsigned int i = 0 ; i < nbBlocksTowrite ; ++i) {
                std::shared_ptr <Block> block = nullptr ;
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
                substractOffset(counterWithoutObjectsBuffer,
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

    // ENEMIES PART
    std::string infoEnemies;
    std::string counterWithoutEnemiesBuffer;
    currentIndex = 0;
    previousReadValue = 0;

    file >> infoEnemies;
    while (!infoEnemies.empty()) {

        readValue = infoEnemies.front();
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
                substractOffset(counterWithoutObjectsBuffer,
                                firstNumberWithoutAnyObjects);
                currentIndex += convertToBase10(counterWithoutObjectsBuffer,
                                                nbOfCharactersWithoutObjects);
            }
            // direction
            const unsigned int side = readValue - firstNumberSide;
            const JBTypes::Dir dir =
                    JBTypesMethods::integerAsDirection(side);

            // enemy type
            infoObjects.erase(infoObjects.begin());
            readValue = infoObjects.front();
            const unsigned int typeOfEnemy = readValue - firstNumberType;

            // enemy movement dir
            infoObjects.erase(infoObjects.begin());
            readValue = infoObjects.front();

            const unsigned int movementSide = readValue - firstNumberSide;
            const JBTypes::Dir movementDir =
                    JBTypesMethods::integerAsDirection(movementSide);

            // Length
            infoObjects.erase(infoObjects.begin());
            readValue = infoObjects.front();
            const size_t length = readValue - firstNumberLength;

            // Color
            infoObjects.erase(infoObjects.begin());
            readValue = infoObjects.front();
            const unsigned int charColor = readValue - firstNumberColor;
            const Enemy::Color color = static_cast<Enemy::Color>(charColor);

            Map::EnemyTypes category;
            std::shared_ptr<Enemy> enemy;
            const auto& blockPtr = mapInfo.blocks.at(currentIndex);
            switch (typeOfEnemy) {
                case 0:
                    category = Map::EnemyTypes::Laser;
                    enemy = std::make_shared<Laser>(
                        color,
                        *blockPtr,
                        Map::getBlockCoords(currentIndex,width,deep),
                        dir,
                        length
                    );
                    break;
                case 1:
                    category = Map::EnemyTypes::ThornBall;
                    enemy = std::make_shared<ThornBall>(
                        *blockPtr,
                        Map::getBlockCoords(currentIndex,width,deep),
                        dir,
                        movementDir,
                        length
                    );
                    break;
                case 2:
                    category = Map::EnemyTypes::ThornBall;
                    enemy = std::make_shared<ThornBall>(
                        *blockPtr,
                        Map::getBlockCoords(currentIndex,width,deep),
                        dir,
                        movementDir,
                        length
                    );
                    break;
                    category = Map::EnemyTypes::DarkBall;
                    enemy = std::make_shared<DarkBall>(
                        *blockPtr,
                        Map::getBlockCoords(currentIndex,width,deep),
                        dir,
                        movementDir,
                        length
                    );
                    break;
                default :
                    category = Map::EnemyTypes::Laser;
                    enemy = std::make_shared<Laser>(
                        color,
                        *blockPtr,
                        Map::getBlockCoords(currentIndex,width,deep),
                        dir,
                        length
                    );
                    break;
            }
            mapInfo.enemiesInfo.push_back({enemy,category});
            counterWithoutObjectsBuffer.clear();
            infoObjects.erase(infoObjects.begin(), infoObjects.begin()+1);
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

unsigned int MapGenerator::convertToBase10(std::string& s, unsigned int base) {
    unsigned int value = 0;
    while (s.length() > 0 ) {
        const unsigned int number = static_cast<unsigned int> (s.front());
        value += number * static_cast<unsigned int> (pow(base,s.length()-1));
        s.erase(s.begin());
    }
    return value;
}

void MapGenerator::substractOffset(std::string& s, int offset) {
    for (char& c : s) {
        c -= offset;
    }
 }



void MapGenerator::compress(std::ifstream& input) {
    std::ofstream output ("outMap.txt");

    unsigned int width;
    unsigned int deep;
    unsigned int height;
    unsigned int beginX;
    unsigned int beginY;
    unsigned int beginZ;

    input >> width;
    input >> deep;
    input >> height;

    output << width << " ";
    output << deep << " ";
    output << height << " ";
    output << std::endl;

    input >> beginX;
    input >> beginZ;
    input >> beginY;

    output << beginX << " ";
    output << beginZ << " ";
    output << beginY << " ";
    output << std::endl;

    unsigned int counter = 1;
    unsigned int currentType;

    // BLOCKS PART
    input >> currentType;

    const std::vector<unsigned int> blockWithSpecialParams {4,6};
    for (unsigned int i = 1 ; i < width * deep * height ; ++i) {

        unsigned int readValue ;
        input >> readValue;

        if (readValue != currentType){
                if (counter > 1 )
                {
                    std::string stringToWrite = convertToBase(
                            counter, nbOfCharactersUsedForNumbers);
                    applyOffset(stringToWrite, firstNumberOfBlock) ;
                    output << stringToWrite;
                }
                if (counter > 0 ) {
                    output << currentType;
                }
                currentType = readValue;
                counter = 1;
        }
        else {
            counter++;
        }
        if( std::find( blockWithSpecialParams.begin(),
                blockWithSpecialParams.end(), readValue ) !=
                blockWithSpecialParams.end()) {
            currentType = readValue;
            output << currentType;
            std::string blockParams;
            input >> blockParams;
            substractOffset(blockParams, static_cast<unsigned int>('a'));
            const unsigned int numberBase10= convertToBase10(blockParams,2);
            std::string numberBase64 = convertToBase(numberBase10,64);
            applyOffset(numberBase64,firstNumberParams);
            output << numberBase64;
            counter = 0;
        }

    }
    if (counter > 1 )
    {
        std::string stringToWrite = convertToBase(
                counter, nbOfCharactersUsedForNumbers);
        applyOffset(stringToWrite, firstNumberOfBlock) ;
        output << stringToWrite;
    }
    output << currentType << std::endl;

    // OBJECTS PART
    std::string readString;
    unsigned int counterWithoutObjects = 0;

    for (unsigned int i = 0 ; i < width * deep * height ; ++i) {
        input >> readString;
        std::cout << "Read string : " << readString << std::endl;
        if (std::isdigit(readString.at(0)) != 0) {
            ++counterWithoutObjects;
        } else {
            if (counterWithoutObjects > 0) {
                std::string stringToWrite = convertToBase(
                        counterWithoutObjects, nbOfCharactersWithoutObjects);
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

    // ENEMIES PART
    readString.clear();
    counterWithoutObjects = 0;

    for (unsigned int i = 0 ; i < width * deep * height ; ++i) {
        input >> readString;
        std::cout << "Read string : " << readString << std::endl;
        if (std::isdigit(readString.at(0)) != 0) {
            ++counterWithoutObjects;
        } else {
            if (counterWithoutObjects > 0) {
                std::string stringToWrite = convertToBase(
                        counterWithoutObjects, nbOfCharactersWithoutObjects);
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
            default:
                std::cerr << "Unknown side character: "
                          << readString.at(2) << std::endl;
                charToWrite = 0;
                break;
            }
            output << charToWrite;

            //Length
            constexpr char AtoFirstNumberLength = 65 - 33;
            output << readString.at(3) - AtoFirstNumberLength;

            //Color
            constexpr char AtoFirstNumberColor = 65 - 33;
            output << readString.at(4) - AtoFirstNumberColor;
        }
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

void MapGenerator::applyOffset(std::string& s, int offset) {
    for (char& c : s) {
        c += offset;
    }
 }

void MapGenerator::verificationMap(std::ifstream& input, const Map& map) 
{
    std::ofstream output ("outMapVerification.txt");

    output << map.width() << " " << map.deep() << " "
           << map.height() << std::endl ;

    output << map.beginX() << " " << map.beginZ() << " " 
        << map.beginY() << std::endl << std::endl;

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
    output << std::endl;
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
    output << std::endl;
    const std::vector<Map::EnemyInfo>& enemiesInfo = map.getEnemiesInfo();
    size_t currentInfo = 0;
    size_t currentIndex = 0;
    for (unsigned int y = 0; y < map.height(); y++) {
        for (unsigned int z = 0; z < map.deep(); z++) {
            for (unsigned int x = 0; x < map.width(); x++) {
                if (currentIndex < map.getEnemiesInfo().at(currentInfo).index) {
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
                        map.getEnemiesInfo().at(currentInfo).enemy->size();
                    const auto color = 
                        map.getEnemiesInfo().at(currentInfo).enemy->getColor();
                    output << getDirection(static_cast<unsigned int>(dir))
                        << typeOfEnemy
                        << getDirection(static_cast<unsigned int>(moveDir))
                        << static_cast<char(length + firstNumberLength)
                        << static_cast<char(color);
                    ++currentInfo;
                }
                if (x != map.width() -1 ) output << " ";
                ++currentIndex;
            }
            output << std::endl;
        }
        output << std::endl;
    }

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

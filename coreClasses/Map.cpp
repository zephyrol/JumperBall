/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.cpp
 * Author: Morgenthaler S
 * 
 * Created on 1 octobre 2019, 21:18
 */

#include <iostream>
#include <algorithm>
#include <functional>
#include <math.h>
#include <cctype>
#include "Map.h"


Map::Map(std::ifstream& file):_id (nbMaps),
                              _blocks{},
                              _width (0),
                              _height (0),
                              _deep (0),
                              _beginX (0),
                              _beginY (0),
                              _beginZ (0),
                              _timeCreation(std::chrono::system_clock::now())
{

    unsigned int width;
    unsigned int deep;
    unsigned int height;

    file >> width; 
    file >> deep; 
    file >> height;
    
    _width = width;
    _height = height;
    _deep = deep;

    file >> _beginX;
    file >> _beginZ;
    file >> _beginY;

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
                    default :
                        break;
                }
                _blocks.push_back(block);
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
            JBTypes::Dir dir = 
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
            
            _blocks.at(currentIndex)->createObject(category,dir);
            counterWithoutObjectsBuffer.clear();
            infoObjects.erase(infoObjects.begin(), infoObjects.begin()+1);
        }
        previousReadValue = readValue;
    }

    for (size_t i = 0; i < _blocks.size(); ++i) {
        if (_blocks.at(i)) {
            _validIndicesBlocks.push_back(i);
        }
    }

    Map::nbMaps++;
}

std::shared_ptr<Block> Map::getBlock(int x, int y, int z){

    std::shared_ptr<const Block> constBlock =
            static_cast<const Map&>(*this).getBlock(x,y,z);

    return std::const_pointer_cast<Block> (constBlock);

}

std::shared_ptr<Block> Map::getBlock(size_t index)
{
    std::shared_ptr<const Block> constBlock =
            static_cast<const Map&>(*this).getBlock(index);

    return std::const_pointer_cast<Block> (constBlock);
}

std::shared_ptr<const Block> Map::getBlock(int x, int y, int z) const {
    std::shared_ptr<const Block> block;
    if (x >= static_cast<int>(_width) ||
            y >= static_cast<int>(_height) ||
            z >= static_cast<int>(_deep) ||
            x < 0 || y < 0 || z < 0 )
        block = nullptr;
    else {
        size_t index = _width * (z + y * _deep) + x;
        block = _blocks.at(index);
    }
    return block;
}

std::shared_ptr<const Block> Map::getBlock(size_t index) const {
    return _blocks.at(index);
}

void Map::printMap() const {
    for ( unsigned int y = 0 ; y < _height ; y++ ) {
        for ( unsigned int z = 0 ; z < _deep ; z++ ){
            for ( unsigned int x = 0 ; x < _width ; x++ ){
                const std::shared_ptr<const Block>& block = getBlock(x,y,z);
                if (block) {
                    std::cout << static_cast<const unsigned int>
                            (block->getType()) << " ";
                }
                else std::cout << static_cast<const unsigned int>
                            (Block::categoryOfBlocksInFile::None) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void Map::verificationMap(std::ifstream& input) const
{

    std::ofstream output ("outMapVerification.txt");

    output << width() << " " << deep() << " "
           << height() << std::endl ;

    output << beginX() << " " << beginZ() << " " << beginY() << std::endl
           << std::endl;

    for ( unsigned int y = 0 ; y < _height ; y++ ) {
        for ( unsigned int z = 0 ; z < _deep ; z++ ){
            for ( unsigned int x = 0 ; x < _width ; x++ ){
                const std::shared_ptr<const Block>& block = getBlock(x,y,z);

                unsigned int typeOfBlock = 0;

                if (block) {

                    typeOfBlock = static_cast<unsigned int> (block->getType());

                    output << static_cast<const unsigned int> (typeOfBlock) ;
                }
                else {
                    output << static_cast<const unsigned int>
                            (Block::categoryOfBlocksInFile::None);
                }
                if (typeOfBlock == 4 || typeOfBlock == 6) {
                    output << " ";
                    const auto infos = block->faceInfo();
                    for (bool info : infos) {
                        if (info) output << "b" ;
                        else output << "a" ;
                    }
                }
                if ( x != _width - 1 )  output << " ";
            }
            output << std::endl;
        }
        output << std::endl;
    }

    output << std::endl;

    for ( unsigned int y = 0 ; y < _height ; y++ ) {
        for ( unsigned int z = 0 ; z < _deep ; z++ ){
            for ( unsigned int x = 0 ; x < _width ; x++ ){
                const std::shared_ptr<const Block>& block = getBlock(x,y,z);
                if (block) {
                    const auto objects = block->objects();
                    bool found = false;
                    for ( size_t i = 0 ; i < objects.size(); ++i ) {

                        if (objects.at(i)) {
                            const std::function<unsigned char(size_t)>
                                    getDirection = [](size_t face) {

                                unsigned char direction;
                                switch(face) {
                                case 0 : direction = 'N'; break;
                                case 1 : direction = 'S'; break;
                                case 2 : direction = 'E'; break;
                                case 3 : direction = 'W'; break;
                                case 4 : direction = 'U'; break;
                                case 5 : direction = 'D'; break;
                                default: direction = '?'; break;
                                }

                                return direction;
                            };

                            std::string s;
                            s.push_back(getDirection(i));
                            if (objects.at(i)->getCategory() ==
                                    Object::CategoryOfObjects::Key) {
                                output << "K" << s;
                            }
                            else if (objects.at(i)->getCategory() ==
                                     Object::CategoryOfObjects::Coin) {
                                output << "I" << s;
                            }
                            else if (objects.at(i)->getCategory() ==
                                     Object::CategoryOfObjects::Clock) {
                                output << "C" << s;
                            }
                            found = true;
                        }

                    }
                    if (!found) {
                        unsigned int typeOfBlock =
                                static_cast<unsigned int> (block->getType());
                        output << static_cast<const unsigned int> (typeOfBlock);
                    }
                    if (x != _width -1 ) output << " ";
                }
                else {
                    output << static_cast<const unsigned int>
                               (Block::categoryOfBlocksInFile::None);
                    if (x != _width -1 ) output << " ";
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

std::array<unsigned int, 3> Map::getBlockCoords(size_t index) const {

    unsigned int uIntIndex = static_cast<unsigned int> (index);
    const unsigned int widthDeep = _width * _deep;
    return { uIntIndex % _width,
             uIntIndex / widthDeep,
             (uIntIndex % widthDeep) / _width };
}

size_t Map::getIndex(const std::array<unsigned int, 3>& coords) const {
    return _width * (coords.at(3) + coords .at(2)* _deep) + coords .at(1);
}

const std::vector<size_t> &Map::validIndicesBlocks() const {
    return _validIndicesBlocks;
}

unsigned int Map::width() const {
    return _width;
}

unsigned int Map::height() const {
    return _height;
}

unsigned int Map::deep() const {
    return _deep;
}

unsigned int Map::beginX() const {
    return _beginX;
}

unsigned int Map::beginY() const {
    return _beginY;
}

unsigned int Map::beginZ() const {
    return _beginZ;
}

std::string Map::convertToBase(unsigned int number, unsigned char base) {
    std::string convertedNumber;
    
    while (number > 0 ) {
        const unsigned int remainder = number % base;
        convertedNumber.insert( convertedNumber.begin(),
                remainder);
        
        number = number / base ;
    }
    
    return convertedNumber;
}

unsigned int Map::convertToBase10(std::string& s, unsigned int base) {
    unsigned int value = 0;
    while (s.length() > 0 ) {
        const unsigned int number = static_cast<unsigned int> (s.front());
        value += number * static_cast<unsigned int> (pow(base,s.length()-1));
        s.erase(s.begin());
    }
    return value;
}

void Map::substractOffset(std::string& s, int offset) {
    for (char& c : s) {
        c -= offset;
    }
 }

void Map::applyOffset(std::string& s, int offset) {
    for (char& c : s) {
        c += offset;
    }
 }

void Map::compress(std::ifstream& input) {
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
    output.close();
}

std::chrono::time_point<std::chrono::system_clock> Map::timeCreation() const {
    return _timeCreation;
}

Map::EffectOnBall Map::interaction(
        const JBTypes::Dir& ballDir, 
        const JBTypes::vec3f& posBall) {
    
    Map::EffectOnBall effect = Map::EffectOnBall::Nothing;
    auto timeNow = JBTypesMethods::getTimePointMSNow();

    //Blocks interaction
    for (unsigned int i = 0 ; i < _validIndicesBlocks.size(); ++i) {
         const std::shared_ptr<Block>& block =
            getBlock(_validIndicesBlocks.at(i));
         block->interaction(
                     ballDir, timeNow, posBall,
                     getBlockCoords(_validIndicesBlocks.at(i)));
         if (block->burstBall()) {
             effect = Map::EffectOnBall::Burst;
         }
    }
    return effect; 
}

std::shared_ptr<Map> Map::loadMap(size_t mapNumber)
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
            map = std::make_shared<Map>(mapFile);
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

unsigned int Map::nbMaps = 0;

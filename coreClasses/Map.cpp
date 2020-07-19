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
#include <math.h>
#include <cctype>
#include "Map.h"

unsigned int Map::nbMaps = 0;

Map::Map() :  _id     (nbMaps++),
              _blocks {},
              _boundingBoxXMax (0),
              _boundingBoxYMax (0),
              _boundingBoxZMax (0),
              _beginX (0),
              _beginY (0),
              _beginZ (0),
              _timeCreation(std::chrono::system_clock::now())
{
}


Map::Map(std::ifstream& file):_id (nbMaps),        
                              _blocks{},
                              _boundingBoxXMax (0),
                              _boundingBoxYMax (0),
                              _boundingBoxZMax (0),
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
    
    _boundingBoxXMax = width;
    _boundingBoxYMax = height;
    _boundingBoxZMax = deep;

    file >> _beginX;
    file >> _beginZ;
    file >> _beginY;


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

    std::string infoObjects;
    std::string counterWithoutObjectsBuffer;
    unsigned int currentIndex = 0;
    char previousReadValue = 0;

    file >> infoObjects;
    while (!infoObjects.empty()) {
        
        readValue = infoObjects.front();
        if (readValue >= firstNumberWithoutAnyObjects) {
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
            JumperBallTypes::Direction dir = 
                    JumperBallTypesMethods::integerAsDirection(side);

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

    Map::nbMaps++;
}


std::shared_ptr<Block> Map::getBlock(int x, int y, int z){

    std::shared_ptr<const Block> constBlock =
            static_cast<const Map&>(*this).getBlock(x,y,z);

    return std::const_pointer_cast<Block> (constBlock);

}

std::shared_ptr<const Block> Map::getBlock(int x, int y, int z) const {
    std::shared_ptr<const Block> block;
    if (x >= static_cast<int>(_boundingBoxXMax) ||  
            y >= static_cast<int>(_boundingBoxYMax) ||
            z >= static_cast<int>(_boundingBoxZMax) ||
            x < 0 || y < 0 || z < 0 )
        block = nullptr;
    else {
        size_t index = _boundingBoxXMax * (z + y * _boundingBoxZMax) + x;
        block = _blocks.at(index);
    }
    return block;
}



void Map::printMap() const {
    for ( unsigned int y = 0 ; y < _boundingBoxYMax ; y++ ) {
        for ( unsigned int z = 0 ; z < _boundingBoxZMax ; z++ ){
            for ( unsigned int x = 0 ; x < _boundingBoxXMax ; x++ ){
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

unsigned int Map::boundingBoxXMax() const {
    return _boundingBoxXMax;
}

unsigned int Map::boundingBoxYMax() const {
    return _boundingBoxYMax;
}

unsigned int Map::boundingBoxZMax() const {
    return _boundingBoxZMax;
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
    output << currentType;

    output << std::endl;  
    
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
        const JumperBallTypes::Direction& ballDir, 
        const JumperBallTypes::vec3f& posBall) {
    
    Map::EffectOnBall effect = Map::EffectOnBall::Nothing;
    auto timeNow = JumperBallTypesMethods::getTimePointMSNow();

    //Blocks interaction 
    std::array<unsigned int,3> currentBlockPosition;
    for ( unsigned int y = 0 ; y < _boundingBoxYMax ; y++ ) {
        currentBlockPosition.at(1) = y;
        for ( unsigned int z = 0 ; z < _boundingBoxZMax ; z++ ){
            currentBlockPosition.at(2) = z;
            for ( unsigned int x = 0 ; x < _boundingBoxXMax ; x++ ){
                currentBlockPosition.at(0) = x;
                const std::shared_ptr<Block>& block = getBlock(x,y,z);
                if (block) {
                    block->interaction(
                        ballDir, timeNow, posBall,currentBlockPosition);
                    if (block->burstBall()) {
                        effect = Map::EffectOnBall::Burst;
                    }
                }
            }
        }
    }

    //Objects interaction 
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
        JumperBallTypesMethods::displayInstallError();
        exit(EXIT_FAILURE);
    }
    std::cout << "Map " << mapNumber << " loaded" << std::endl;


    return map;
}

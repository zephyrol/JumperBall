/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.cpp
 * Author: seb
 * 
 * Created on 1 octobre 2019, 21:18
 */

#include <iostream>
#include <math.h>
#include "Map.h"

unsigned int Map::nbMaps = 0;

Map::Map() : _id (nbMaps),
             //_blocks{},
             _map3DData{},
             _boundingBoxXMax (0),
             _boundingBoxYMax (0),
             _boundingBoxZMax (0),
             _beginX (0),
             _beginY (0),
             _beginZ (0)
{
    Map::nbMaps++;
}


Map::Map(std::ifstream& file):_id (nbMaps),        
                             _map3DData{},
                             _boundingBoxXMax (0),
                             _boundingBoxYMax (0),
                             _boundingBoxZMax (0),
                             _beginX (0),
                             _beginY (0),
                             _beginZ (0)
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


    auto convertToBase10 = [] (std::string s, unsigned int base)-> unsigned int{
        unsigned int value = 0; 
        while (s.length() > 0 ) {
          unsigned int number = static_cast<unsigned int> (s.front());
          value += number * static_cast<unsigned int> (pow(base,s.length()-1));
          s.erase(s.begin());
        }
        return value;
    };

    auto substractOffset = [] (std::string& s, unsigned int offset) {
        for (char& c : s) {
            c -= offset;
        }
    };

    std::string infoMap;
    std::string counterBuffer;
    char readValue;

    file >> infoMap;
    while (!infoMap.empty()) {
        
        readValue = infoMap.front();
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
                        block = std::make_shared<SharpBlock>(); break ;
                    case 5:
                        block = std::make_shared<BrittleBlock>(); break ;
                    default :
                        break;
                }
              _map3DData.push_back(block); 
              counterBuffer.clear();
            }
        }
        infoMap.erase(infoMap.begin());
    }
    Map::nbMaps++;
}


Map::~Map() {
}

std::shared_ptr<Block> Map::map3DData(int x, int y, int z) const {
    
    std::shared_ptr<Block> block;
    if (x >= static_cast<int>(_boundingBoxXMax) ||  
            y >= static_cast<int>(_boundingBoxYMax) ||
            z >= static_cast<int>(_boundingBoxZMax) ||
            x < 0 || y < 0 || z < 0 )
        block = nullptr;
    else 
        block = _map3DData.at(_boundingBoxXMax* (z + y * _boundingBoxZMax) + x);
    return block;
}

void Map::printMap() const {
    for ( unsigned int y = 0 ; y < _boundingBoxYMax ; y++ ) {
        for ( unsigned int z = 0 ; z < _boundingBoxZMax ; z++ ){
            for ( unsigned int x = 0 ; x < _boundingBoxXMax ; x++ ){
                const std::shared_ptr<const Block>& block = map3DData(x,y,z);
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

    auto applyOffset = [] (std::string& s, unsigned int offset) {
        for (char& c : s) {
            c += offset;
        }
    };
    
    auto convertToBase = [] (unsigned int number, unsigned char base) 
        -> std::string {
        
        std::string convertedNumber;
        
        while (number > 0 ) {
            const unsigned int remainder = number % base;
            convertedNumber.insert( convertedNumber.begin(),
                    remainder);
            
            number = number / base ;
        }
        
        return convertedNumber;
    };
    
    input >> currentType;

    for (unsigned int i = 0 ; i < width * deep * height ; ++i) {
        
        
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
            output << currentType;

            counter = 1;       
            currentType = readValue;
        }
        else {
            counter++;
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
    output.close();
}

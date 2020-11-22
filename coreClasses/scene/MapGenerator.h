/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MapGenerator.h
 * Author: Morgenthaler S
 * 
 * Created on 2 novembre 2020, 22:38
 */

#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include "Map.h"

namespace MapGenerator {

    //---------CONSTANTS------------//

    //Blocks compression
    static constexpr unsigned char        firstKindOfBlock                 = 48;
    static constexpr unsigned char        firstNumberOfBlock               = 58;
    static constexpr unsigned int         nbOfCharactersUsedForNumbers     = 69;
    static constexpr unsigned int         firstNumberParams                = 33;

    //Objects and special compression
    static constexpr unsigned char        nextBlockAction                  = 33;
    static constexpr unsigned char        firstNumberType                  = 34;
    static constexpr unsigned char        firstNumberSide                  = 42;
    static constexpr unsigned char        firstNumberWithoutAnyObjects     = 48;
    static constexpr unsigned int         nbOfCharactersWithoutObjects     = 79;

    //Enemies compression
    static constexpr unsigned char        firstNumberLength                = 33;
    static constexpr unsigned char        firstNumberColor                 = 33;

    void compress(std::ifstream &input);
    std::shared_ptr<Map> loadMap(size_t mapNumber);
    Map::MapInfo createMapInfo(std::ifstream &file);
    std::string                     convertToBase (unsigned int number,
                                                  unsigned char base);
    unsigned int                    convertToBase10(const std::string& s,
                                                   unsigned int base);
    std::string                     applyOffset (const std::string& s,
                                                int offset);
    std::string                     substractOffset(const std::string& s,
                                                   int offset);
    Map::BlockTypes                 uintToBlockType(unsigned int number);

    // Reading functions
    unsigned int                    readUnsignedInt(std::ifstream& input);
    std::string                     readingString(std::ifstream& input);

    // Writing functions
    void                            writeSeparator(std::ofstream& output);
    void                            writeEndLine(std::ofstream& output);
    void                            writeUnsignedInt( std::ofstream& output,
                                                      unsigned int unsignedInt);
    void                            writeString( std::ofstream& output,
                                                 const std::string& string);

    void                            verificationMap(std::ifstream &input,
                                                   const Map& map);
}

#endif

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.h
 * Author: Morgenthaler S
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef MAP_H
#define MAP_H
#include "Types.h"
#include "blocks/Block.h"
#include "blocks/BaseBlock.h"
#include "blocks/FireBlock.h"
#include "blocks/IceBlock.h"
#include "blocks/SharpBlock.h"
#include "blocks/BrittleBlock.h"
#include "blocks/JumpBlock.h"
#include "objects/Key.h"
#include "objects/Coin.h"
#include "objects/Clock.h"
#include <fstream>

class Map {
public:

    //------------TYPES------------//
    enum class KindOfData   {Raw,Optimized};
    enum class EffectOnBall {Nothing,Burst,Burnt,Slide};
    
    //---------CONSTANTS------------//

    //Blocks compression
    static constexpr unsigned char        firstKindOfBlock                 = 48;
    static constexpr unsigned char        firstNumberOfBlock               = 58;
    static constexpr unsigned int         nbOfCharactersUsedForNumbers     = 69;

    static constexpr unsigned int         firstNumberParams                = 33;

    //Objects compression
    static constexpr unsigned char        firstNumberType                  = 33;
    static constexpr unsigned char        firstNumberSide                  = 42;
    static constexpr unsigned char        firstNumberWithoutAnyObjects     = 48;
    static constexpr unsigned int         nbOfCharactersWithoutObjects     = 79;

    //--CONSTRUCTORS & DESTRUCTORS--//
    Map                                   ();
    Map                                   ( const std::string content, 
                                            Map::KindOfData kData );

    Map                                   ( std::ifstream& file );


    //-------CONST METHODS----------//
    unsigned int                          beginX()                        const;
    unsigned int                          beginY()                        const;
    unsigned int                          beginZ()                        const;

    std::shared_ptr< const Block>         getBlock(int x, int y, int z)   const;

    unsigned int                          boundingBoxXMax()               const;
    unsigned int                          boundingBoxYMax()               const;
    unsigned int                          boundingBoxZMax()               const;




    std::chrono::time_point<std::chrono::system_clock>
                                          timeCreation()                  const;
    void                                  printMap()                      const;

    //----------METHODS------------//
    EffectOnBall                          interaction(
                                      const JumperBallTypes::Direction& ballDir,
                                      const JumperBallTypes::vec3f& posBall );
    std::shared_ptr<Block>                getBlock(int x, int y, int z);

    //--------STATIC METHODS-------//
    static void                           compress(std::ifstream& input);


private:

    //--------ATTRIBUTES-----------//
    const unsigned int                    _id;

    std::vector<std::shared_ptr<Block> >  _blocks;
    //std::vector<std::shared_ptr<Object> > _objects;
    unsigned int                          _boundingBoxXMax;
    unsigned int                          _boundingBoxYMax;
    unsigned int                          _boundingBoxZMax;

    unsigned int                          _beginX;
    unsigned int                          _beginY;
    unsigned int                          _beginZ;
    std::chrono::time_point<std::chrono::system_clock>  
                                          _timeCreation;
    
    static unsigned int                   nbMaps;


    //--------STATIC METHODS-------//
    static std::string                    convertToBase (unsigned int number,
                                                          unsigned char base);
    static unsigned int                   convertToBase10(std::string& s,
                                                            unsigned int base);
    static void                           applyOffset (std::string& s,
                                                        int offset);
    static void                           substractOffset(std::string& s,
                                                          int offset);

};

#endif /* MAP_H */


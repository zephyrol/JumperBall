/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.h
 * Author: seb
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
#include "ClassicalMechanics.h"
#include <fstream>

class Map {
public:

    //------------TYPES------------//
    enum class KindOfData {Raw,Optimized};
    
    //---------CONSTANTS------------//
    static constexpr unsigned char        firstNumberOfBlock               = 58;
    static constexpr unsigned int         nbOfCharactersUsedForNumbers     = 69;
    static constexpr unsigned char        firstKindOfBlock                 = 48;

    //--CONSTRUCTORS & DESTRUCTORS--//
    Map                                   ();
    Map                                   ( const Map& orig );
    Map                                   ( const std::string content, 
                                            Map::KindOfData kData );

    Map                                   ( std::ifstream& file );
    virtual                               ~Map();


    //-------CONST METHODS----------//
    std::shared_ptr< const Block>         map3DData(int x, int y, int z)  const;
    void                                  printMap()                      const;

    unsigned int                          boundingBoxXMax()               const;
    unsigned int                          boundingBoxYMax()               const;
    unsigned int                          boundingBoxZMax()               const;

    unsigned int                          beginX()                        const;
    unsigned int                          beginY()                        const;
    unsigned int                          beginZ()                        const;

    std::chrono::time_point<std::chrono::system_clock>  
                                          timeCreation()                  const;

    //----------METHODS------------//
    void                                  interaction(
                                      const JumperBallTypes::Direction& ballDir,
                                      const JumperBallTypes::vec3f& posBall
    );
    std::shared_ptr<Block>                map3DData(int x, int y, int z);

    //--------STATIC METHODS-------//
    static void                           compress(std::ifstream& input);
    
private:

    //--------ATTRIBUTES-----------//
    const unsigned int                    _id;

    std::vector< std::shared_ptr<Block> > _map3DData;
    unsigned int                          _boundingBoxXMax;
    unsigned int                          _boundingBoxYMax;
    unsigned int                          _boundingBoxZMax;

    unsigned int                          _beginX;
    unsigned int                          _beginY;
    unsigned int                          _beginZ;
    std::chrono::time_point<std::chrono::system_clock>  
                                          _timeCreation;
    
    static unsigned int                   nbMaps;

};

#endif /* MAP_H */


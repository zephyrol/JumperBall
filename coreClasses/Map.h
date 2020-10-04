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
#include "ParallelTask.h"
#include <fstream>

class Map {
public:

    //---------CONSTANTS------------//

    //Blocks compression
    static constexpr unsigned char        firstKindOfBlock                 = 48;
    static constexpr unsigned char        firstNumberOfBlock               = 58;
    static constexpr unsigned int         nbOfCharactersUsedForNumbers     = 69;

    static constexpr unsigned int         firstNumberParams                = 33;

    //Objects compression
    static constexpr unsigned char        nextBlockAction                  = 33;
    static constexpr unsigned char        firstNumberType                  = 34;
    static constexpr unsigned char        firstNumberSide                  = 42;
    static constexpr unsigned char        firstNumberWithoutAnyObjects     = 48;
    static constexpr unsigned int         nbOfCharactersWithoutObjects     = 79;


    // ----------- TYPES -----------//
    enum class BlockTypes { None, Base, Fire, Ice, Sharp,
                            Brittle, Jump, Ghost };

    struct BlockInfo { size_t index; 
                       BlockTypes type; };

    struct MapInfo { unsigned int width;
                     unsigned int height;
                     unsigned int deep;
                     unsigned int beginX;
                     unsigned int beginY;
                     unsigned int beginZ;
                     std::vector<std::shared_ptr<Block> > blocks;
                     std::vector<BlockInfo> blocksInfo;
                  };

    //--CONSTRUCTORS & DESTRUCTORS--//
    Map                                   ( MapInfo&& mapInfo );

    //-------CONST METHODS----------//
    unsigned int                          beginX()                        const;
    unsigned int                          beginY()                        const;
    unsigned int                          beginZ()                        const;

    std::shared_ptr< const Block>         getBlock(int x, int y, int z)   const;
    std::shared_ptr< const Block>         getBlock(size_t index)          const;

    unsigned int                          width()                         const;
    unsigned int                          height()                        const;
    unsigned int                          deep()                          const;

    std::chrono::time_point<std::chrono::system_clock>
                                          timeCreation()                  const;
    void                                  verificationMap(std::ifstream &input)
                                                                          const;

    std::array<unsigned int, 3>           getBlockCoords(size_t index)    const;
    size_t                                getIndex(
                               const std::array<unsigned int, 3>& coords) const;
    BlockTypes                            getType (
                             const std::array<unsigned int, 3>& position) const;
    const std::vector<BlockInfo>&         blocksInfo()                    const;

    //----------METHODS------------//
    Block::Effect                         interaction(
                                  const JBTypes::Dir& ballDir,
                                  const JBTypes::vec3f& posBall , float radius);
    std::shared_ptr<Block>                getBlock(int x, int y, int z);
    std::shared_ptr<Block>                getBlock(size_t index);

    std::vector<BlockInfo>                getBlocksWithInteraction()      const;
    std::vector<BlockInfo>                getBlocksWithObjects()          const;

    //--------STATIC METHODS-------//
    static void                           compress(std::ifstream& input);

    static std::shared_ptr<Map>           loadMap(size_t mapNumber);

    static MapInfo                        createMapInfo(std::ifstream& file);

private:

    //--------ATTRIBUTES-----------//
    std::vector<std::shared_ptr<Block> >  _blocks;
    std::vector<BlockInfo>                _blocksInfo;
    std::vector<BlockInfo>                _blocksWithInteractionInfo;
    std::vector<BlockInfo>                _blocksWithObjectsInfo;

    unsigned int                          _width;
    unsigned int                          _height;
    unsigned int                          _deep;
    unsigned int                          _beginX;
    unsigned int                          _beginY;
    unsigned int                          _beginZ;
    std::chrono::time_point<std::chrono::system_clock>  
                                          _timeCreation;
    // Multithreading
    ParallelTask<Block::Effect>           _blocksInteractions;
    ParallelTask<void>                    _objectsInteractions;

    JBTypes::Dir                          _dirBallInteractions;
    JBTypes::vec3f                        _posBallInteractions;
    float                                 _radiusInteractions;
    JBTypes::timePointMs                  _timeInteractions;
    // --------------

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
    static BlockTypes                     uintToBlockType(unsigned int number);

};

#endif /* MAP_H */


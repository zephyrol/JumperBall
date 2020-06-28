/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Types.h"

JumperBallTypes::vec3f JumperBallTypesMethods::directionAsVector (
                                                JumperBallTypes::Direction dir){
    
    JumperBallTypes::vec3f dirVec3 {0.f,0.f,0.f};
    switch (dir) {
        case JumperBallTypes::Direction::North:
            dirVec3.z = -1.f;
            break;
        case JumperBallTypes::Direction::South:
            dirVec3.z = 1.f;
            break;
        case JumperBallTypes::Direction::East:
            dirVec3.x = 1.f;
            break;
        case JumperBallTypes::Direction::West:
            dirVec3.x = -1.f;
            break;
        case JumperBallTypes::Direction::Up:
            dirVec3.y = 1.f;
            break;
        case JumperBallTypes::Direction::Down:
            dirVec3.y = -1.f;
            break;
        default :
            break;
    }
    
    return dirVec3;
}

JumperBallTypes::Direction JumperBallTypesMethods::integerAsDirection  
                                                        (unsigned int number) 
{
    JumperBallTypes::Direction dir ; 
    switch (number){
        case 0 : 
            dir = JumperBallTypes::Direction::North;
            break;
        case 1 : 
            dir = JumperBallTypes::Direction::South;
            break;
        case 2 : 
            dir = JumperBallTypes::Direction::East;
            break;
        case 3 : 
            dir = JumperBallTypes::Direction::West;
            break;
        case 4 : 
            dir = JumperBallTypes::Direction::Up;
            break;
        case 5 : 
            dir = JumperBallTypes::Direction::Down;
            break;
        default:
            dir = JumperBallTypes::Direction::North;
            break;
    }
    
    return dir;
}


JumperBallTypes::timePointMs JumperBallTypesMethods::getTimePointMSNow() 
                                                                    noexcept {
    return std::chrono::time_point_cast<std::chrono::milliseconds> 
                                    (std::chrono::system_clock::now());
}

float JumperBallTypesMethods::getTimeSecondsSinceTimePoint(
                      const JumperBallTypes::timePointMs& timePoint) noexcept{

    const JumperBallTypes::timePointMs timeNowMs
                                              = JumperBallTypesMethods::
                                                            getTimePointMSNow();
    const JumperBallTypes::durationMs timeNowSinceEpoch   
                                              = timeNowMs.time_since_epoch();
    
    const JumperBallTypes::timePointMs timeActionMs           
                                              = timePoint; 
    const JumperBallTypes::durationMs timeActionSinceEpoch    
                                              = timeActionMs.time_since_epoch();
    
    const JumperBallTypes::durationMs difference 
                                              = timeNowSinceEpoch - 
                                                          timeActionSinceEpoch;
    const std::chrono::duration<float> durationFloatDifference 
                                              = difference;
    const float fDifference                   = durationFloatDifference.count();

    return fDifference;
}

    float JumperBallTypesMethods::getFloatFromDurationMS(
                                    const JumperBallTypes::durationMs& dms )
    {
        const std::chrono::duration<float> durationFloatDifference = dms;
        const float fDifference = durationFloatDifference.count();
        return fDifference;
    }

JumperBallTypes::timePointMs JumperBallTypesMethods::getTimePointMsFromTimePoint
(const std::chrono::time_point<std::chrono::system_clock> & timePoint) noexcept{

    return std::chrono::time_point_cast<std::chrono::milliseconds> (timePoint);
}

unsigned int JumperBallTypesMethods::directionAsInteger(
                                              JumperBallTypes::Direction dir) {
    
    unsigned int number;
    switch (dir){
        case JumperBallTypes::Direction::North : 
            number = 0;
            break;
        case JumperBallTypes::Direction::South : 
            number = 1;
            break;
        case JumperBallTypes::Direction::East : 
            number = 2;
            break;
        case JumperBallTypes::Direction::West : 
            number = 3;
            break;
        case JumperBallTypes::Direction::Up: 
            number = 4;
            break;
        case JumperBallTypes::Direction::Down: 
            number = 5;
            break;
        default:
            number = 0;
            break;
    }
    return number;
}

void JumperBallTypesMethods::displayInstallError() {
        #ifdef _MSC_VER
        std::cerr << "Did you forget to install the files ?" << std::endl
         << "On Visual Studio, build the INSTALL solution" << std::endl;
        #else
        std::cerr << "Did you forget to install the project ?" << std::endl <<
            "Run \"cmake --build . --target install\" " <<
            "or \"make install\"" << std::endl;
        #endif
}

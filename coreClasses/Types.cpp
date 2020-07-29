/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Types.h"

JBTypes::vec3f JBTypesMethods::directionAsVector (
                                                JBTypes::Dir dir){
    
    JBTypes::vec3f dirVec3 {0.f,0.f,0.f};
    switch (dir) {
        case JBTypes::Dir::North:
            dirVec3.z = -1.f;
            break;
        case JBTypes::Dir::South:
            dirVec3.z = 1.f;
            break;
        case JBTypes::Dir::East:
            dirVec3.x = 1.f;
            break;
        case JBTypes::Dir::West:
            dirVec3.x = -1.f;
            break;
        case JBTypes::Dir::Up:
            dirVec3.y = 1.f;
            break;
        case JBTypes::Dir::Down:
            dirVec3.y = -1.f;
            break;
        default :
            break;
    }
    
    return dirVec3;
}

JBTypes::Dir JBTypesMethods::integerAsDirection  
                                                        (unsigned int number) 
{
    JBTypes::Dir dir ; 
    switch (number){
        case 0 : 
            dir = JBTypes::Dir::North;
            break;
        case 1 : 
            dir = JBTypes::Dir::South;
            break;
        case 2 : 
            dir = JBTypes::Dir::East;
            break;
        case 3 : 
            dir = JBTypes::Dir::West;
            break;
        case 4 : 
            dir = JBTypes::Dir::Up;
            break;
        case 5 : 
            dir = JBTypes::Dir::Down;
            break;
        default:
            dir = JBTypes::Dir::North;
            break;
    }
    
    return dir;
}


JBTypes::timePointMs JBTypesMethods::getTimePointMSNow() 
                                                                    noexcept {
    return std::chrono::time_point_cast<std::chrono::milliseconds> 
                                    (std::chrono::system_clock::now());
}

float JBTypesMethods::getTimeSecondsSinceTimePoint(
                      const JBTypes::timePointMs& timePoint) noexcept{

    const JBTypes::timePointMs timeNowMs
                                              = JBTypesMethods::
                                                            getTimePointMSNow();
    const JBTypes::durationMs timeNowSinceEpoch   
                                              = timeNowMs.time_since_epoch();
    
    const JBTypes::timePointMs timeActionMs           
                                              = timePoint; 
    const JBTypes::durationMs timeActionSinceEpoch    
                                              = timeActionMs.time_since_epoch();
    
    const JBTypes::durationMs difference 
                                              = timeNowSinceEpoch - 
                                                          timeActionSinceEpoch;
    const std::chrono::duration<float> durationFloatDifference 
                                              = difference;
    const float fDifference                   = durationFloatDifference.count();

    return fDifference;
}

    float JBTypesMethods::getFloatFromDurationMS(
                                    const JBTypes::durationMs& dms )
    {
        const std::chrono::duration<float> durationFloatDifference = dms;
        const float fDifference = durationFloatDifference.count();
        return fDifference;
    }

JBTypes::timePointMs JBTypesMethods::getTimePointMsFromTimePoint
(const std::chrono::time_point<std::chrono::system_clock> & timePoint) noexcept{

    return std::chrono::time_point_cast<std::chrono::milliseconds> (timePoint);
}

unsigned int JBTypesMethods::directionAsInteger(
                                              JBTypes::Dir dir) {
    
    unsigned int number;
    switch (dir){
        case JBTypes::Dir::North : 
            number = 0;
            break;
        case JBTypes::Dir::South : 
            number = 1;
            break;
        case JBTypes::Dir::East : 
            number = 2;
            break;
        case JBTypes::Dir::West : 
            number = 3;
            break;
        case JBTypes::Dir::Up: 
            number = 4;
            break;
        case JBTypes::Dir::Down: 
            number = 5;
            break;
        default:
            number = 0;
            break;
    }
    return number;
}

void JBTypesMethods::displayInstallError() {
        #ifdef _MSC_VER
        std::cerr << "Did you forget to install the files ?" << std::endl
         << "On Visual Studio, build the INSTALL solution" << std::endl;
        #else
        std::cerr << "Did you forget to install the project ?" << std::endl <<
            "Run \"cmake --build . --target install\" " <<
            "or \"make install\"" << std::endl;
        #endif
}

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <Types.h>

JumperBallTypes::vec3f JumperBallTypesMethods::directionAsVector (
                                                JumperBallTypes::Direction dir) {
    
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
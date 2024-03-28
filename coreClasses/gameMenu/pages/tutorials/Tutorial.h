//
// Created by S.Morgenthaler on 28/03/2024.
//

#ifndef TUTORIAL_H
#define TUTORIAL_H
#include <string>
#include <array>
#include <vector>
#include "scene/MovableObject.h"

class Tutorial;
using Tutorial_uptr = std::unique_ptr<Tutorial>;

class Tutorial {
public:
   /**
    * A tutorial message is two strings indicating text to display.
    * The first string is displayed on the top of the screen and the second one below the first one.
    */
   using Message = std::array<std::string, 2>;

   /**
    * Get the messages.
    */
   virtual std::vector<Tutorial::Message> getMessages() const = 0;

   /**
    * Get the current message to display through its number id.
    * Returns -1 If no message needs to be display.
    */
   virtual short getCurrentMessageNumberId() const = 0;

   /**
    * Get the animation duration time of the message in seconds.
    * t < 0.f means disappearing
    * t >= 0.f && < 1.f means appearing
    * t >= 1.f means changing color animation.
    */
   virtual float getAnimationTime() = 0;

   /**
    * Update the tutorial status
    */
   virtual void update() = 0;

   virtual ~Tutorial() = default;
};

#endif //TUTORIAL_H

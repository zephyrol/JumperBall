//
// Created by S.Morgenthaler on 11/04/2024.
//

#ifndef ABOVEMOVINGCAMERA_H
#define ABOVEMOVINGCAMERA_H

class AboveMovingCamera;
using CstAboveMovingCamera_sptr = std::shared_ptr<const AboveMovingCamera>;

class AboveMovingCamera {
   public:
    virtual bool isMovingAbove() const = 0;
    virtual ~AboveMovingCamera() = default;
};
#endif  // ABOVEMOVINGCAMERA_H

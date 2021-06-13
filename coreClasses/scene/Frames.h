/*
 * File: Frames.h
 * Author: Morgenthaler S
 *
 * Created on 12 juin 2021, 20h05
 */

#ifndef FRAMES_H
#define FRAMES_H
#include "system/Types.h"
#include "ObjectState.h"

/*class Frames;
using Frames_sptr = std::shared_ptr <Frames>;
using CstFrames_sptr = std::shared_ptr <const Frames>;
using vecCstFrames_sptr = std::vector <CstFrames_sptr>;
using vecFrames_sptr = std::vector <Frames_sptr>;
using Frames_uptr = std::unique_ptr <Frames>;*/

template<typename ObjState> class Frames {

public:
enum class CurrentFrame { FrameA, FrameB };

Frames(const std::shared_ptr<ObjState>& stateFrameA, const std::shared_ptr<ObjState>& stateFrameB);
void swapFrames();
const std::shared_ptr<ObjState>& getUpdatableState();
std::shared_ptr<const ObjState> getRenderableState() const;

template<typename UpdatableObj> static std::unique_ptr<Frames<ObjState> > 
genFrames(const UpdatableObj& object);

template<typename UpdatableObj, typename ChildState> static std::unique_ptr<Frames<ObjState> > 
genFrames(const UpdatableObj& object);

private: 
std::shared_ptr<ObjState> _stateFrameA;
std::shared_ptr<ObjState> _stateFrameB;
CurrentFrame _currentFrame;
};

template<typename ObjState> Frames<ObjState>::Frames(
  const std::shared_ptr<ObjState>& stateFrameA,
  const std::shared_ptr<ObjState>& stateFrameB
  ):
_stateFrameA(stateFrameA),
_stateFrameB(stateFrameB),
_currentFrame(Frames::CurrentFrame::FrameA)
{
}

template<typename ObjState> void Frames<ObjState>::swapFrames() {
  const auto& inter = _stateFrameA;
  _stateFrameA = _stateFrameB;
  _stateFrameB = inter;
}

template<typename ObjState> const std::shared_ptr<ObjState>& Frames<ObjState>::getUpdatableState() {
  return _currentFrame == CurrentFrame::FrameA 
    ? _stateFrameA
    : _stateFrameB;
}

template<typename ObjState> std::shared_ptr<const ObjState> Frames<ObjState>::getRenderableState() const {

  const auto& renderableState = _currentFrame == CurrentFrame::FrameA
    ? _stateFrameB
    : _stateFrameA;

  return std::const_pointer_cast<const ObjState>(renderableState);
}

template<typename ObjState> 
template<typename UpdatableObj> 
std::unique_ptr<Frames<ObjState> > Frames<ObjState>::genFrames( const UpdatableObj& object) {

  /*const std::shared_ptr<ObjState> objStateFrameA = std::make_shared<ObjState>(object);
  const std::shared_ptr<ObjState> objStateFrameB = std::make_shared<ObjState>(object);

  return std::unique_ptr<Frames<ObjState> >(new Frames<ObjState>(objStateFrameA, objStateFrameB));*/
  return Frames<ObjState>::genFrames<UpdatableObj, ObjState>(object);
}

template<typename ObjState> 
template<typename UpdatableObj, typename ChildState> 
std::unique_ptr<Frames<ObjState> > Frames<ObjState>::genFrames(const UpdatableObj& object) {

  const std::shared_ptr<ObjState> objStateFrameA = std::make_shared<ChildState>(object);
  const std::shared_ptr<ObjState> objStateFrameB = std::make_shared<ChildState>(object);

  return std::unique_ptr<Frames<ObjState> >(new Frames<ObjState>(objStateFrameA, objStateFrameB));
}

template<typename ObjState> using Frames_sptr = std::shared_ptr <Frames<ObjState> >;
template<typename ObjState> using CstFrames_sptr = std::shared_ptr <const Frames<ObjState> >;
template<typename ObjState> using vecCstFrames_sptr = std::vector <CstFrames_sptr<ObjState> >;
template<typename ObjState> using vecFrames_sptr = std::vector <Frames_sptr<ObjState> >;
template<typename ObjState> using Frames_uptr = std::unique_ptr <Frames<ObjState> >;

// using Frames_sptr = std::shared_ptr <Frames>;
// using CstFrames_sptr = std::shared_ptr <const Frames>;
//template<typename ObjState> using vecCstFrames_sptr = std::vector <CstFrames_sptr<ObjState> >;
//template<typename ObjState> using vecFrames_sptr = std::vector <Frames_sptr<ObjState> >;

#endif // FRAMES_H

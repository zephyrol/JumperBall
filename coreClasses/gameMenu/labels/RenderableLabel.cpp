//
// Created by S.Morgenthaler on 16/02/2023.
//

#include "RenderableLabel.h"

RenderableLabel::RenderableLabel(CstDisplayable_sptr label, CstDisplayable_sptr page) :
    _label(std::move(label)),
    _page(std::move(page)) {
}

Displayable::GlobalState RenderableLabel::getGlobalState() const {
    return GlobalState::United;
}

std::vector<unsigned char> RenderableLabel::getStaticUnsignedByteValues() const {
    return _label->getStaticUnsignedByteValues();
}

std::vector<short> RenderableLabel::getStaticShortValues() const {
    return _label->getStaticShortValues();
}

Displayable::StaticValues<float> RenderableLabel::getStaticFloatValues() const {
    return _label->getStaticFloatValues();
}

Displayable::StaticValues<JBTypes::vec2f> RenderableLabel::getStaticVec2fValues() const {
    return _label->getStaticVec2fValues();
}

Displayable::StaticValues<JBTypes::vec3f> RenderableLabel::getStaticVec3fValues() const {
    return _label->getStaticVec3fValues();
}

Displayable::DynamicNames RenderableLabel::getDynamicIntNames() const {
    return _page->getDynamicIntNames();
}

Displayable::DynamicValues<int> RenderableLabel::getDynamicIntValues() const {
    return _page->getDynamicIntValues();
}

Displayable::DynamicNames RenderableLabel::getDynamicFloatNames() const {
    return _page->getDynamicFloatNames();
}

Displayable::DynamicValues<float> RenderableLabel::getDynamicFloatValues() const {
    return _page->getDynamicFloatValues();
}

Displayable::DynamicNames RenderableLabel::getDynamicVec2fNames() const {
    return _page->getDynamicVec2fNames();
}

Displayable::DynamicValues<JBTypes::vec2f> RenderableLabel::getDynamicVec2fValues() const {
    return _page->getDynamicVec2fValues();
}

Displayable::DynamicNames RenderableLabel::getDynamicVec3fNames() const {
    return _page->getDynamicVec3fNames();
}

Displayable::DynamicValues<JBTypes::vec3f> RenderableLabel::getDynamicVec3fValues() const {
    return _page->getDynamicVec3fValues();
}

Displayable::DynamicNames RenderableLabel::getDynamicQuaternionNames() const {
    return _page->getDynamicQuaternionNames();
}

Displayable::DynamicValues<JBTypes::Quaternion> RenderableLabel::getDynamicQuaternionValues() const {
    return _page->getDynamicQuaternionValues();
}


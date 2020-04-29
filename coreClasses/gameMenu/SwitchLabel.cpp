#include "MessageLabel.h"

MessageLabel::MessageLabel(float width,float height,const std::string& message):
    Label(width,height),
    _message(message)
{
}

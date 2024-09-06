#include "KeyboardEvent.h"

KeyBoardEvent::KeyBoardEvent() : type(EventType::Invalid), key(0u) {}
KeyBoardEvent::KeyBoardEvent(const EventType type, const unsigned char key) : type(type), key(key){}

bool KeyBoardEvent::IsPress() const
{
    return this->type == EventType::Press;
}

bool KeyBoardEvent::IsRelease() const
{
    return this->type == EventType::Release;
}

bool KeyBoardEvent::IsValid() const
{
    return this->type != EventType::Invalid;
}

unsigned char KeyBoardEvent::GetKeyCode() const
{
    return this->key;
}



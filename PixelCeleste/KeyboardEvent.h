#pragma once

class KeyBoardEvent
{
public:
    enum EventType
    {
        Press,
        Release,
        Invalid
    };
public:
    KeyBoardEvent();
    KeyBoardEvent(const EventType type, const unsigned char key);
    
public:
    bool IsPress() const;
    bool IsRelease() const;
    bool IsValid() const;
    unsigned char GetKeyCode() const;

private:
    EventType type;
    unsigned char key;

};


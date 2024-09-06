#pragma once
#include "KeyboardEvent.h"
#include <queue>

class KeyboardClass
{
public:
    KeyboardClass();
    bool KeyIsPressed(const unsigned char keycode);
    bool KeyBufferIsEmpty();
    bool CharBufferIsEmpty();
    KeyBoardEvent ReadKey();
    unsigned char ReadChar();

    void OnKeyPressed(const unsigned char key);
    void OnKeyReleased(const unsigned char key);
    void OnChar(const unsigned char key);

    void EnableAutoRepeatKeys();
    void DisableAutoRepeatKeys();
    void EnableAutoRepeatChars();
    void DisableAutoRepeatChars();

    bool IsKeysAutoRepeat();
    bool IsCharsAutoRepeat();

private:
    bool autoRepeatKeys = false;
    bool autoRepeatChars = false;

    bool keyStates[256];

    std::queue<KeyBoardEvent> keyBuffer;
    std::queue<unsigned char> charBuffer;
};


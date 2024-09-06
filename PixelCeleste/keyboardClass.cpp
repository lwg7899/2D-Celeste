#include "keyboardClass.h"

KeyboardClass::KeyboardClass() 
{
    for (int i = 0; i <256; i++) 
    {
        this->keyStates[i] = false;
    }
}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode)
{
    return this->keyStates[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty()
{
    return this->keyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
{
    return this->charBuffer.empty();
}

KeyBoardEvent KeyboardClass::ReadKey()
{
    //버퍼에 키보드 이벤트가 있는 경우에만 키를 읽어야 하지만 어떤 이유로 키를 호출하는 경우 잘못된 키보드를 반환하게된다.
    if (this->keyBuffer.empty()) 
    {
        return KeyBoardEvent();
    }
    else 
    {
        //큐의 맨 앞에 있는 키 이벤트를 가져오고, 가져왔으니깐 이미 읽은 키 이벤트는 버퍼에서 제거하여 중복처리되지 않도록한다.
        KeyBoardEvent e = this->keyBuffer.front();
        this->keyBuffer.pop();
        return e;
    }
}

unsigned char KeyboardClass::ReadChar()
{
    if (this->charBuffer.empty()) 
    {
        //u는 unsigned를 나타내며, 반환하는 값이 부호 없는 정수임을 나타낸다.
        return 0u;
    }
    else 
    {
        unsigned char e = this->charBuffer.front();
        this->charBuffer.pop();
        return e;
    }
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
    this->keyStates[key] = true;
    this->keyBuffer.push(KeyBoardEvent(KeyBoardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
    this->keyStates[key] = false;
    this->keyBuffer.push(KeyBoardEvent(KeyBoardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
    this->charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
    this->autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys()
{
    this->autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
    this->autoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
    this->autoRepeatChars = false;
}

bool KeyboardClass::IsKeysAutoRepeat()
{
    return this->autoRepeatKeys;
}

bool KeyboardClass::IsCharsAutoRepeat()
{
    return this->autoRepeatChars;
}
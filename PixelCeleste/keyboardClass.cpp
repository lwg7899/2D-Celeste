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
    //���ۿ� Ű���� �̺�Ʈ�� �ִ� ��쿡�� Ű�� �о�� ������ � ������ Ű�� ȣ���ϴ� ��� �߸��� Ű���带 ��ȯ�ϰԵȴ�.
    if (this->keyBuffer.empty()) 
    {
        return KeyBoardEvent();
    }
    else 
    {
        //ť�� �� �տ� �ִ� Ű �̺�Ʈ�� ��������, ���������ϱ� �̹� ���� Ű �̺�Ʈ�� ���ۿ��� �����Ͽ� �ߺ�ó������ �ʵ����Ѵ�.
        KeyBoardEvent e = this->keyBuffer.front();
        this->keyBuffer.pop();
        return e;
    }
}

unsigned char KeyboardClass::ReadChar()
{
    if (this->charBuffer.empty()) 
    {
        //u�� unsigned�� ��Ÿ����, ��ȯ�ϴ� ���� ��ȣ ���� �������� ��Ÿ����.
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
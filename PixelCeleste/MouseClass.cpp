#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y)
{
    this->leftIsDown = true;
    MouseEvent mouseEvent(MouseEvent::LPress, x, y);
    this->eventBuffer.push(mouseEvent);
}

void MouseClass::OnLeftReleased(int x, int y)
{
    this->leftIsDown = false;
    MouseEvent mouseEvent(MouseEvent::LRelease, x, y);
    this->eventBuffer.push(mouseEvent);
}

void MouseClass::OnRightPressed(int x, int y)
{
    this->rightIsDown = true;
    this->eventBuffer.push(MouseEvent::MouseEvent(MouseEvent::RPress, x, y));
}

void MouseClass::OnRightReleased(int x, int y)
{
    this->rightIsDown = false;
    this->eventBuffer.push(MouseEvent::MouseEvent(MouseEvent::RRelease, x, y));
}

void MouseClass::OnMiddlePressed(int x, int y)
{
    this->mbuttonDown = true;
    this->eventBuffer.push(MouseEvent::MouseEvent(MouseEvent::MPress, x, y));
}

void MouseClass::OnMiddleReleased(int x, int y)
{
    this->mbuttonDown = false;
    this->eventBuffer.push(MouseEvent::MouseEvent(MouseEvent::MRelease, x, y));
}

void MouseClass::OnWheelUp(int x, int y)
{
    this->eventBuffer.push(MouseEvent::MouseEvent(MouseEvent::WheelUp, x, y));
}

void MouseClass::OnWheelDown(int x, int y)
{
    this->eventBuffer.push(MouseEvent::MouseEvent(MouseEvent::WheeelDown, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
    this->x = x;
    this->y = y;
    this->eventBuffer.push(MouseEvent::MouseEvent(MouseEvent::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
    this->eventBuffer.push(MouseEvent::MouseEvent(MouseEvent::RAW_MOVE, x, y));
}

bool MouseClass::IsLeftDown()
{
    return this->leftIsDown;
}

bool MouseClass::IsMiddleDown()
{
    return this->mbuttonDown;
}

bool MouseClass::IsRightDown()
{
    return this->rightIsDown;
}

int MouseClass::GetPosX()
{
    return this->x;
}

int MouseClass::GetPosY()
{
    return this->y;
}

MousePoint MouseClass::GetPos()
{
    return {this->x, this->y};
}

bool MouseClass::EventBufferIsEmpty()
{
    return this->eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
    if (this->eventBuffer.empty()) 
    {
        return MouseEvent();
    }
    else 
    {
        MouseEvent e = this->eventBuffer.front();
        this->eventBuffer.pop();
        return e;
    }
}

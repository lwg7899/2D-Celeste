#pragma once
#include <DirectXMath.h>

struct Vertex
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 texCoord;

    Vertex() :pos(0,0,0), texCoord(0,0){}
    Vertex(float x, float y, float z, float u, float v) : pos(x, y, z), texCoord(u, v) {}

};


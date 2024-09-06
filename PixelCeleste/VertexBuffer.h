#pragma once
#ifndef VertexBuffer_h__
#define VertexBuffer_h__

#include <iostream>
#include <d3d11.h>
#include "wrl.h"
#include "ErrorLogger.h"

template<class T>
class VertexBuffer
{
private:
    VertexBuffer(const VertexBuffer<T>& rhs) {}

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    std::unique_ptr<UINT> stride;
    UINT bufferSize = 0;

public:

    VertexBuffer() {}

    ID3D11Buffer* Get()const 
    {
        return buffer.Get();
    }

    ID3D11Buffer* const* GetAddressOf() const 
    {
        return buffer.GetAddressOf();
    }

    UINT BufferSize() const 
    {
        return this->bufferSize;
    }

    const UINT Stride() const 
    {
        return *this->stride.get();
    }

    const UINT* StridePtr() const 
    {
        return this->stride.get();
    }

    HRESULT Initialize(ID3D11Device* device, T* data, UINT numVertiecs) 
    {
        this->bufferSize = numVertiecs;
        this->stride = std::make_unique<UINT>(sizeof(T));

        /*���� 1��
         �������� �������� ������Ƽ�� �����Ͱ� �ʿ��ϴ�.������Ƽ�� �����ʹ� DirectX���� ������ �Ǵ� ������ �ּ����� ���� ��, �� ,���� ���Ѵ�.
         ������Ƽ�� �����͸� �������ҷ��� DricetX11������ ���ؽ� ���ۿ� �ε��� ���۸� �غ��ؾ��Ѵ�.
        */

        //���� 1���� �غ� �ܰ�: ���ؽ� ������ ������ ������ �����ϴ� ����ü ����(���۸� �����ϴ� ����ü)
        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(T) * numVertiecs; //������ ������ ũ��
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;      //���۰� �����������ο� ���ε��Ǵ� ����� �ĺ�
        vertexBufferDesc.CPUAccessFlags = 0;                        //CPU�� ���ۿ� �����ϴ� ����� �ĺ�, CPU�� ���۸� ������� �ʴ´ٸ� 0�� �����Ѵ�.
        vertexBufferDesc.MiscFlags = 0;                             //��Ÿ �÷��� �ĺ�

        //���ҽ� ���� �ִ� ���� �����͸� �ǹ� ������ �ʱ�ȭ�� ����� �ڷḦ �����ϴ� D3D11_SUBRESOURCE_DATA��� ����ü
        D3D11_SUBRESOURCE_DATA vertexBufferData;
        ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
        vertexBufferData.pSysMem = data;           //���� ������ �ʱ�ȭ
        //vertexBufferData.SysMemPitch = 0;       //�޸��� Pitch��
        //vertexBufferData.SysMemSlicePitch = 0;  //���� ����(3D ����ó ���ҽ������� ���)

        //CreateBuffer�� ȣ���ؼ� ���۸� ���� CreateBuffer( pDesc: ������ ���۸� �����ϴ� ����ü,  pInitialData: ���۸� �ʱ�ȭ�ϴµ� ����� �ڷ�, ppBuffer: ������ ���� ��ȯ     )
        HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->buffer.GetAddressOf());
        if (FAILED(hr))
        {
            ErrorLogger::Log(hr, "Failed to create vertex buffer");
            return false;
        }
        return hr;

    }

};

#endif // VertexBuffer_h__
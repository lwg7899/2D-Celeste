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

        /*설명 1번
         폴리곤을 찍을려면 프리미티브 데이터가 필요하다.프리미티브 데이터는 DirectX에서 렌더링 되는 도형의 최소한의 단위 점, 선 ,면을 말한다.
         프리미티브 데이터를 렌더링할려면 DricetX11에서는 버텍스 버퍼와 인덱스 버퍼를 준비해야한다.
        */

        //설명 1번의 준비 단계: 버텍스 버퍼의 데이터 형식을 정의하는 구조체 선언(버퍼를 서술하는 구조체)
        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(T) * numVertiecs; //생성할 버퍼의 크기
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;      //버퍼가 ㅍ파이프라인에 바인딩되는 방법을 식별
        vertexBufferDesc.CPUAccessFlags = 0;                        //CPU가 버퍼에 접근하는 방식을 식별, CPU가 버퍼를 사용하지 않는다면 0을 지정한다.
        vertexBufferDesc.MiscFlags = 0;                             //기타 플래그 식별

        //리소스 내에 있는 실제 데이터를 의미 버퍼의 초기화에 사용할 자료를 서술하는 D3D11_SUBRESOURCE_DATA라는 구조체
        D3D11_SUBRESOURCE_DATA vertexBufferData;
        ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
        vertexBufferData.pSysMem = data;           //버퍼 데이터 초기화
        //vertexBufferData.SysMemPitch = 0;       //메모리의 Pitch값
        //vertexBufferData.SysMemSlicePitch = 0;  //깊이 레벨(3D 텏스처 리소스에서만 사용)

        //CreateBuffer를 호출해서 버퍼를 생성 CreateBuffer( pDesc: 생성할 버퍼를 서술하는 구조체,  pInitialData: 버퍼를 초기화하는데 사용할 자료, ppBuffer: 생성된 버퍼 반환     )
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
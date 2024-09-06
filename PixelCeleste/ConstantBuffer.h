#pragma once
#include "d3d11.h"
#include "DirectXMath.h"
#include "wrl.h"
#include "ErrorLogger.h"
#include "ConstantBufferType.h"
/*
JPG: 압축을 통해 용량을 줄이면 손실이 일어나기 때문에 줄였다가 키우면 복원이 안되서 게임에서 사용하지 않는다
GIF: 손실압축
PNG: 무손실 압축, 압축도 잘되고 지원되는 기능도 많은 32채널~ 128채널 고해상도(요즘엔 PNG만 거의씀)
TGA: 옛날에 쓰던 무손실압축방식이다. 채널은 12채널밖에 지원되지않고 RGBA일 떄 알파 값이 없다. A인 투명처리가 없다. 투명처리를할때 컬러키 1 0 1처럼 이런 특징인 색상을 사용한다.
핑크색이 비슷한 마제타색이라고 하는데 이걸 투명으로 사용함
MBP: 압축이 없고 텍스쳐 구조 연습할 떄 씀. 압축이 안되니깐 용량이 크다.
*/

template<typename T>
class ConstantBuffer
{
private:
    ConstantBuffer(const ConstantBuffer<T>& rhs);

    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;

public:
    T data;

public:
    ConstantBuffer() {}

    ID3D11Buffer* Get() const 
    {
        return buffer.Get();
    }

    ID3D11Buffer* const* GetAddressOf() const
    {
        return buffer.GetAddressOf();
    }

    HRESULT Initialize(ID3D11Device* device,ID3D11DeviceContext* deviceContext) 
    {

        this->deviceContext = deviceContext;

        D3D11_BUFFER_DESC desc;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
        desc.StructureByteStride = 0;

        HRESULT hr = device->CreateBuffer(&desc, 0, buffer.GetAddressOf());
        if (FAILED(hr))
        {
            ErrorLogger::Log(hr, "Failed to map constant buffer.");
        }
        return hr;
    }

    bool ApplyChanges() 
    {
        //Update ConstantBuffer;
        D3D11_MAPPED_SUBRESOURCE mappedResource;    //함수가 성공적으로 호출된 후에 매핑된 버퍼의 포인터와 피치정보를 저장하는 구조체

        /*
        Map함수를 사용하여 매핑을 시작하고 작업을 마치면 Umap함수를 호출하여 매핑을 해제한다.
        Map 다른 프로세스가 해당 메모리에 접근을 하지 못하게 막는 것을 의미
        메모리 데이터를 변경하기 전에 락 (Map) 을 걸어서 타 프로세스의 접근을 제한하고 쓰기 작업이 다 끝나고 나면 다시 락을 풀게 ( Unmap )한다.
        
        1. 잠군다 Map

        2. 메모리 작업 (쓰기, 변경, 지우기 등) memcpy 등

        3. 풀어준다 Unmap
        */

        //Map CPU가 GPU메모리에 직접 접근할 수 있게 한다. D3D11_MAP_WRITE_DISCARD:기존버퍼를 모두 버리고 새로운 데이터를 채우겠다는 것
        HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD,0, &mappedResource);
        if (FAILED(hr)) 
        {
            ErrorLogger::Log(hr, "Failed to map constant buffer.");
            return false;
        }
        CopyMemory(mappedResource.pData, &data, sizeof(T)); //데이터를 복사하는데 data는 버퍼에 복사할 데이터를 가리키는 포인터
        this->deviceContext->Unmap(buffer.Get(), 0);

        return true;
    }

};
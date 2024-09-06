#pragma once
#include "d3d11.h"
#include "DirectXMath.h"
#include "wrl.h"
#include "ErrorLogger.h"
#include "ConstantBufferType.h"
/*
JPG: ������ ���� �뷮�� ���̸� �ս��� �Ͼ�� ������ �ٿ��ٰ� Ű��� ������ �ȵǼ� ���ӿ��� ������� �ʴ´�
GIF: �սǾ���
PNG: ���ս� ����, ���൵ �ߵǰ� �����Ǵ� ��ɵ� ���� 32ä��~ 128ä�� ���ػ�(���� PNG�� ���Ǿ�)
TGA: ������ ���� ���սǾ������̴�. ä���� 12ä�ιۿ� ���������ʰ� RGBA�� �� ���� ���� ����. A�� ����ó���� ����. ����ó�����Ҷ� �÷�Ű 1 0 1ó�� �̷� Ư¡�� ������ ����Ѵ�.
��ũ���� ����� ����Ÿ���̶�� �ϴµ� �̰� �������� �����
MBP: ������ ���� �ؽ��� ���� ������ �� ��. ������ �ȵǴϱ� �뷮�� ũ��.
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
        D3D11_MAPPED_SUBRESOURCE mappedResource;    //�Լ��� ���������� ȣ��� �Ŀ� ���ε� ������ �����Ϳ� ��ġ������ �����ϴ� ����ü

        /*
        Map�Լ��� ����Ͽ� ������ �����ϰ� �۾��� ��ġ�� Umap�Լ��� ȣ���Ͽ� ������ �����Ѵ�.
        Map �ٸ� ���μ����� �ش� �޸𸮿� ������ ���� ���ϰ� ���� ���� �ǹ�
        �޸� �����͸� �����ϱ� ���� �� (Map) �� �ɾ Ÿ ���μ����� ������ �����ϰ� ���� �۾��� �� ������ ���� �ٽ� ���� Ǯ�� ( Unmap )�Ѵ�.
        
        1. �ᱺ�� Map

        2. �޸� �۾� (����, ����, ����� ��) memcpy ��

        3. Ǯ���ش� Unmap
        */

        //Map CPU�� GPU�޸𸮿� ���� ������ �� �ְ� �Ѵ�. D3D11_MAP_WRITE_DISCARD:�������۸� ��� ������ ���ο� �����͸� ä��ڴٴ� ��
        HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD,0, &mappedResource);
        if (FAILED(hr)) 
        {
            ErrorLogger::Log(hr, "Failed to map constant buffer.");
            return false;
        }
        CopyMemory(mappedResource.pData, &data, sizeof(T)); //�����͸� �����ϴµ� data�� ���ۿ� ������ �����͸� ����Ű�� ������
        this->deviceContext->Unmap(buffer.Get(), 0);

        return true;
    }

};
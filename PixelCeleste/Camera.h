#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
private:
    DirectX::XMVECTOR posVector;
    DirectX::XMVECTOR rotVector;
    
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;

    DirectX::XMMATRIX viewMatrix;
    DirectX::XMMATRIX projectionMatrix;

    const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);  //eyePos
    const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);       //upVector

    const DirectX::XMVECTOR DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
    const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
    const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

    DirectX::XMVECTOR vec_Forward;
    DirectX::XMVECTOR vec_Backward;
    DirectX::XMVECTOR vec_Right;
    DirectX::XMVECTOR vec_Left;


private:
    void UpdateViewMatrix();

public:
    Camera();

    void SetProjectionValues(float fovRadians, float aspectRatio, float nearZ, float farZ);
    void SetLookAtPos(DirectX::XMFLOAT3 lookAtPos);
  
    const DirectX::XMMATRIX& GetViewMatrix() const;
    const DirectX::XMMATRIX& GetProjectionMatrix() const;

    const DirectX::XMVECTOR& GetPositionVector() const;
    const DirectX::XMFLOAT3& GetPositionFloat3() const;

    const DirectX::XMVECTOR& GetRotationVector() const;
    const DirectX::XMFLOAT3& GetRotationFloat3() const;

    void SetPosition(const DirectX::XMVECTOR& pos);
    void SetPosition(float x, float y, float z);
    
    void AdjustPosition(const DirectX::XMVECTOR& pos);
    void AdjustPosition(float x, float y, float z);
    
    void SetRotation(const DirectX::XMVECTOR& rot);
    void SetRotation(float x, float y, float z);

    void AdjustRotation(const DirectX::XMVECTOR& rot);
    void AdjustRotation(float x, float y, float z);

public:
    const DirectX::XMVECTOR& GetForwardVector();
    const DirectX::XMVECTOR& GetBackwardVector();
    const DirectX::XMVECTOR& GetRightVector();
    const DirectX::XMVECTOR& GetLeftVector();
};


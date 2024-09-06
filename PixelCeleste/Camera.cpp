#include "Camera.h"

void Camera::UpdateViewMatrix()
{
    //ī�޶� ȸ����� ���   XMMatrixRotationRollPitchYaw �Լ��� ī�޶��� ��(roll, x�� ȸ��), ��ġ(pitch, y�� ȸ��), ��(yaw, z�� ȸ��) ���� ���ڷ� �޾� ȸ�� ����� ����
    DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);

    //ī�޶� Ÿ���� ���� ���� ��� XMVector3TransformCoord:3D���͸� �־��� ��Ʈ������ �����ϴ� �Լ�,  �� �κ��� ī�޶��� ���� ������ ��Ÿ���� ���� ���͸� ����Ѵ�. �� ���ʹ� ī�޶� ȸ�� ��Ŀ� ���� ��ȯ�Ѵ�.
    DirectX::XMVECTOR camTarget = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);

    //ī�޶� Ÿ�� ��ġ ����
    camTarget += this->posVector;
    //camTarget = DirectX::XMVectorAdd(camTarget, this->posVector);

    //camRotationMatrix�� ��ȯ�Ͽ� ���� ī�޶� ȸ���� �ݿ��� ����� ���� upDir�� ��´�
    DirectX::XMVECTOR upDir = DirectX::XMVector3Transform(this->DEFAULT_UP_VECTOR, camRotationMatrix);
    
    //XMMatrixLookAtLH �Լ��� ����Ͽ� �� ���(viewMatrix)�� ����Ѵ�
    this->viewMatrix = DirectX::XMMatrixLookAtLH(this->posVector, camTarget, upDir);

    DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f,this->rotation.y, 0.0f);
 
    this->vec_Forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_Backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_Left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_Right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

}


Camera::Camera()
{
    //XMStoreFloat3: XMVECTOR�� XMFLOAT3�� ��ȯ�ϴ� �Լ� 
    //XMLoadFloat3: XMFLOAT3�� XMVECTOR�� ��ȯ�ϴ� �Լ�

    this->position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->posVector = DirectX::XMLoadFloat3(&this->position);

    this->rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->rotVector = DirectX::XMLoadFloat3(&this->rotation);

    UpdateViewMatrix();
}


void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
    this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}


void Camera::SetLookAtPos(DirectX::XMFLOAT3 lookAtPos)
{
    if (lookAtPos.x == this->position.x && lookAtPos.y == this->position.y && lookAtPos.z == this->position.z) 
    {
        return;
    }
    
    lookAtPos.x = this->position.x - lookAtPos.x;   //x�� �Ÿ����
    lookAtPos.y = this->position.y - lookAtPos.y;   //y�� �Ÿ����
    lookAtPos.z = this->position.z - lookAtPos.z;   //z�� �Ÿ����

    float pitch = 0.0f;
    if (lookAtPos.y !=0.0f) 
    {
        //x,y,z��ǥ�� ���� ���� ���
        const float distance = sqrtf(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
        pitch = atan(lookAtPos.y / distance);
    }

    this->SetRotation(pitch,0.0f, 0.0f);

}


const DirectX::XMMATRIX& Camera::GetViewMatrix() const
{
    return viewMatrix;
}


const DirectX::XMMATRIX& Camera::GetProjectionMatrix() const
{
    return projectionMatrix;
}


const DirectX::XMVECTOR& Camera::GetPositionVector() const
{
    return posVector;
}


const DirectX::XMFLOAT3& Camera::GetPositionFloat3() const
{
    return position;
}


const DirectX::XMVECTOR& Camera::GetRotationVector() const
{
    return rotVector;
}


const DirectX::XMFLOAT3& Camera::GetRotationFloat3() const
{
    return rotation;
}

void Camera::SetPosition(const DirectX::XMVECTOR& pos)
{
    DirectX::XMStoreFloat3(&this->position , pos);
    this->posVector = pos;
    this->UpdateViewMatrix();
}


void Camera::SetPosition(float x, float y, float z)
{
    this->position = DirectX::XMFLOAT3(x,y,z);
    this->posVector = DirectX::XMLoadFloat3(&this->position);
    this->UpdateViewMatrix();
}


void Camera::AdjustPosition(const DirectX::XMVECTOR& pos)
{
    this->posVector += pos;
    //this->posVector = DirectX::XMVectorAdd(this->posVector, pos);
    DirectX::XMStoreFloat3(&this->position, this->posVector);
    this->UpdateViewMatrix();
}


void Camera::AdjustPosition(float x, float y, float z)
{
    //XMStoreFloat3: XMVECTOR�� XMFLOAT3�� ��ȯ�ϴ� �Լ� 

//XMLoadFloat3: XMFLOAT3�� XMVECTOR�� ��ȯ�ϴ� �Լ�
    this->position.x += x;
    this->position.y += y;
    this->position.z += z;
    this->posVector =  DirectX::XMLoadFloat3(&this->position);
    this->UpdateViewMatrix();
}

void Camera::SetRotation(const DirectX::XMVECTOR& rot)
{
    this->rotVector = rot;
    DirectX::XMStoreFloat3(&this->rotation, rot);
    this->UpdateViewMatrix();
}


void Camera::SetRotation(float x, float y, float z)
{
    this->rotation = DirectX::XMFLOAT3(x,y,z);
    this->rotVector = DirectX::XMLoadFloat3(&this->rotation);
    this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const DirectX::XMVECTOR& rot)
{
    this->rotVector = DirectX::XMVectorAdd(this->rotVector, rot);
    DirectX::XMStoreFloat3(&this->rotation, this->rotVector);
    this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
    this->rotation.x += x;
    this->rotation.y += y;
    this->rotation.z += z;

    this->UpdateViewMatrix();
}

const DirectX::XMVECTOR& Camera::GetForwardVector()
{
    return this->vec_Forward;
}

const DirectX::XMVECTOR& Camera::GetRightVector()
{
    return this->vec_Right;
}

const DirectX::XMVECTOR& Camera::GetBackwardVector()
{
    return this->vec_Backward;
}

const DirectX::XMVECTOR& Camera::GetLeftVector()
{
    return this->vec_Left;
}

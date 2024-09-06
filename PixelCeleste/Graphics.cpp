#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    this->windowWidth = width;
    this->windowHeight = height;

    //스왑체인, 디바이스 생성, 뷰포트 설정
    if (!InitializeDirectX(hwnd))
    {
        return false;
    }

    //버텍스 픽셀, 레이아웃
    if (!InitializeShaders())
    {
        return false;
    }

    //정점 설정
    if (!InitializeScene())
    {   
        return false;
    }

    return true;
}


bool Graphics::InitializeDirectX(HWND hwnd)
{
    std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

    if (adapters.size() < 1)
    {
        ErrorLogger::Log("No DXGI Adapters found");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferDesc.Width = this->windowWidth;
    scd.BufferDesc.Height = this->windowHeight;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;

    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.OutputWindow = hwnd;
    scd.Windowed = TRUE;

    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT hr;
    hr = D3D11CreateDeviceAndSwapChain(
        adapters[0].pAdapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        NULL,
        NULL,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &scd,
        this->swapChain.GetAddressOf(),
        this->device.GetAddressOf(),
        NULL,
        this->deviceContext.GetAddressOf()
    );

    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to craete device and swapchain");
        return false;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "GetBuffer Failed");
        return false;
    }

    hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create reder target view");
        return false;
    }

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = this->windowWidth;
    depthStencilDesc.Height = this->windowHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;    //DXGI_FORMAT_D24_UNORM_S8_UINT: 깊이 버퍼 스텐실을 32비트로 사용하고 나머지 8비트를 스텐실 채널에 사용한다는 의미
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create depth stencil buffer.");
        return false;
    }

    hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create depth stencil view.");
        return false;
    }

    this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

    //Creatre depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthstencildesc;
    ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthstencildesc.DepthEnable = true;
    depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    /*
    D3D11_COMPARISON_NEVER
    비교를 통과하지 마세요.   항상 거짓을 반환

    D3D11_COMPARISON_LESS
    원본 데이터가 대상 데이터보다 작으면 비교가 통과됩니다.  < 연산자

    D3D11_COMPARISON_EQUAL
    원본 데이터가 대상 데이터와 같으면 비교가 통과합니다.  == 연산자

    D3D11_COMPARISON_LESS_EQUAL
    원본 데이터가 대상 데이터보다 작거나 같으면 비교가 통과합니다. <= 연산자

    D3D11_COMPARISON_GREATER    > 연산자
    원본 데이터가 대상 데이터보다 크면 비교가 통과합니다.

    D3D11_COMPARISON_NOT_EQUAL  != 연산자
    원본 데이터가 대상 데이터와 같지 않으면 비교가 통과합니다.

    D3D11_COMPARISON_GREATER_EQUAL  >= 연산자
    원본 데이터가 대상 데이터보다 크거나 같으면 비교가 통과합니다.

    D3D11_COMPARISON_ALWAYS 항상 참을 반환
    항상 비교를 전달합니다.
    */

    hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create depth stencil state");
        return false;
    }

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = this->windowWidth;
    viewport.Height = this->windowHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    this->deviceContext->RSSetViewports(1, &viewport);


    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

    hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create rasterizer state.");
        return false;
    }

    spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
    spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data/Fonts/comic_sans_ms_16_spritefont");

    //Create sampler description sampler state
    D3D11_SAMPLER_DESC sampDesc;    // 텍스처 샘플링 방식을 정의하는 구조체이다. 텍스처의 주소모드, 피렅링 방식, 경계 색상 등을 설정하여 텍스처가 어떻게 렌더링 될지 제어할 수 있다.
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf());

    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create sampler state.");
        return false;
    }

    return true;
}

bool Graphics::InitializeShaders()
{
    std::wstring shaderfolder = L"";

#pragma region DetermineShaderPath
    if (IsDebuggerPresent() == TRUE)
    {
#ifdef _DEBUG   //DebugMode
#ifdef _WIN64   //x64
        shaderfolder = L"";
#else //x86(Win32)
        shaderfolder = L"";
#endif

#else//Release Mode
#ifdef _WIN64
        shaderfolder = L"";
#else
        shaderfolder = L"";
#endif
#endif
    }

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
       {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
       {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    //DXGI_FORMAT_R32G32_FLOAT

    UINT numElements = ARRAYSIZE(layout);

    if (!vertexShader.Initialize(this->device, L"vertexshader.cso", layout, numElements))
    {
        return false;
    }

    if (!pixelShader.Initialize(this->device, L"pixelshader.cso"))
    {
        return false;
    }

    return true;
}

bool Graphics::InitializeScene()
{
    //텍스처 사각형
    Vertex v[] =
    {
        Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f), //Bottom Left 
        Vertex(-0.5f,  0.5f, 0.0f, 0.0f, 0.0f), //Top Left
        Vertex( 0.5f,  0.5f, 0.0f, 1.0f, 0.0f), //Top Right
        Vertex( 0.5f, -0.5f, 0.0f, 1.0f, 1.0f), //Top Right

    };

    //Load Vertex Data
    HRESULT hr = this->vertexBuffer.Initialize(this->device.Get(), v, ARRAYSIZE(v));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to creaste vertx buffer");
        return false;
    }


    DWORD indices[] =
    {
        0,1,2,
        0,2,3
    };

    indicesBuffer.Initialize(device.Get(),indices, ARRAYSIZE(indices) );
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create indices Buffer");
        return false;
    }


    hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data/Textures/HarpSeal.jpg", nullptr, myTexture.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create wic texutre from fule");
        return false;
    }


    //Initialize Constant Buffer(s)
    hr = constantBuffer.Initialize(device.Get(), deviceContext.Get());
    if (FAILED(hr)) 
    {
        ErrorLogger::Log(hr, "Failed to initialize constant buffer");
        return false;
    }

    camera.SetPosition(0.0f, 0.0f, -2.0f);
    camera.SetProjectionValues(90.0f , static_cast<float>(this->windowWidth) / static_cast<float>(windowHeight), 0.1f, 1000.0f);

    return true;
}


void Graphics::RenderFrame()
{
    float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
    this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
    this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);   //D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST     D3D10_PRIMITIVE_TOPOLOGY_LINELIST
    this->deviceContext->RSSetState(this->rasterizerState.Get());

    this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
    this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());


    this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
    this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);


    UINT offset = 0;

    ////Update ConstantBuffer
    DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
    float PI = 3.14159265358979323846;

   /* world.r[0] = DirectX::XMVectorSet(cos(1.507f), -sin(1.507f),0.0f,0.0f);
    world.r[1] = DirectX::XMVectorSet(sin(1.507f),cos(1.507f),0.0f,0.0f);*/

    //camera.AdjustPosition(0.0f, 0.01f, 0.0f);
    // camera.SetLookAtPos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    //카메라 행렬(Camera Matrix)은 보통 투영 행렬과 결합하여 뷰-투영 행렬(View-Projection Matrix)을 구성하여 최종적으로 뷰 행렬과 투영 행렬의 역할을 수행 viewMatrix * projection이 식이 카메라 행렬을 나타낸다    
    constantBuffer.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
    //constantBuffer.data.mat.r[0] = DirectX::XMVectorSet(cos(PI / 2), -sin(PI / 2), 0.0f, 0.0f);
    //constantBuffer.data.mat.r[1] = DirectX::XMVectorSet(sin(PI / 2), cos(PI / 2), 0.0f, 0.0f);
   

    constantBuffer.data.mat = DirectX::XMMatrixTranspose(constantBuffer.data.mat);

    if (!constantBuffer.ApplyChanges()) 
    {
        return;
    }
    this->deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
    
    //Square
    this->deviceContext->PSSetShaderResources(0, 1, this->myTexture.GetAddressOf());
    this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
    this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);

    //Draw Text
    spriteBatch->Begin();
    spriteFont->DrawString(spriteBatch.get(), L"HELLO WORLD", DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
    spriteBatch->End();

    this->swapChain->Present(1, NULL);
}
#pragma once
#include "ErrorLogger.h"
#include<d3d11.h>
#include <wrl.h>

//--------------------- fmod -----------------------
 
////x86
//#pragma comment(lib, "../fmod/lib/x86/fmodL_vc.lib")
//#pragma comment(lib, "../fmod/lib/x86/fmod_vc.lib")
//
////x64
//#pragma comment(lib, "../fmod/lib/x64/fmodL_vc.lib")
//#pragma comment(lib, "../fmod/lib/x64/fmod_vc.lib")


//#pragma comment(lib, "fmod_vc.lib")
//#pragma comment(lib, "fmodL_vc.lib")

//--------------------------------------------------

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")

#include <wrl.h>
#include <vector>

class AdapterData
{
public:
    AdapterData(IDXGIAdapter* pAdapter);
    IDXGIAdapter* pAdapter;
    DXGI_ADAPTER_DESC description;
};

class AdapterReader
{
public:
    static std::vector<AdapterData> GetAdapters();

private:
    static std::vector<AdapterData> adapters;
};


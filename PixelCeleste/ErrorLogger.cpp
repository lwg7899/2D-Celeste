#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log(std::string message)
{
    std::string error_message = "Error: " + message;
    MessageBoxA(NULL, error_message.c_str(), +"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::string message)
{
    _com_error error(hr);
    std::wstring error_message = L"Error: " + StringConverter::StringToWide(message) + L"\n" + error.ErrorMessage() + L"\n";
    MessageBoxW(NULL, error_message.c_str(), +L"Error", MB_ICONERROR);
    //메세지라는 박스이름은 Error라는 이름이고 이 메세지 박스 안에 error_message가 가지는 문자열 내용을 출력한다
}

void ErrorLogger::Log(HRESULT hr, std::wstring message)
{
    _com_error error(hr);
    std::wstring error_message = L"Error: " + message + L"\n" + error.ErrorMessage() + L"\n";
    MessageBoxW(NULL, error_message.c_str(), +L"Error", MB_ICONERROR);
}

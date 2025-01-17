#pragma once
#include <Windows.h>
#include "StringConverter.h"

class ErrorLogger 
{
public:
    static void Log(std::string message);
    static void Log(HRESULT hr, std::string message);
    static void Log(HRESULT hr, std::wstring message);
};
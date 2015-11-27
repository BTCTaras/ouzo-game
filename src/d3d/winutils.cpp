#ifdef _WIN32

#include "d3d/winutils.hpp"

#include <Windows.h>
#include <d3d9.h>

#include <stdio.h>

bool WinUtils::CheckResult(HRESULT result) {
	return SUCCEEDED(result);
}

#endif
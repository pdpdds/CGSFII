#pragma once
#ifdef _WIN32
#include <windows.h>
#include "SFEngine.h"
#else
#include "../EngineLayerLinux/SFEngine.h"
#endif

#ifdef _WIN32
#pragma comment(lib, "EngineLayer.lib")
//#pragma comment(lib, "DatabaseLayer.lib")
#endif
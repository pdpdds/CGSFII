#pragma once
#include "SFConstant.h"
#ifdef _WIN32
#include "../CGSFNet/EngineStructure.h"
#include "../CGSFNet/BasePacket.h"
#include "../CGSFNet/IEngine.h"
#else
#include "../CGSFNetLinux/EngineStructure.h"
#include "../CGSFNetLinux/BasePacket.h"
#include "../CGSFNetLinux/IEngine.h"
#endif
#include "SFStructure.h"
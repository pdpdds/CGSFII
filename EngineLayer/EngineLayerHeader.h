#pragma once

#ifdef _WIN32
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// TODO: reference additional headers your program requires here
#include <windows.h>
#include <tchar.h>
#endif

#include "../BaseLayer/CGSFDefine.h"
#include "CommonHeader.h"
#include "../CGSFNet/ErrorCode.h"
#include "../CGSFNet/EngineConstant.h"
#include "../CGSFNet/IEngine.h"
#include "../CGSFNet/INetworkEngine.h"
#include "../CGSFNet/INetworkCallback.h"
#include "../CGSFNet/IPacketProtocol.h"
#include "../CGSFNet/BasePacket.h"
//#include "../BaseLayer/SFString.h"
#include "../BaseLayer/SFObjectPool.h"
#include "../BaseLayer/Macro.h"
#include "SFLogicGateway.h"
#include "ILogicEntry.h"
#include "ILogicDispatcher.h"
#include "LogicEntry.h"
#include "SFPacket.h"
#include "SFPacketProtocol.h"

#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"


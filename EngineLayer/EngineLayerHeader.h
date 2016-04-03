#pragma once

#ifdef _WIN32
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// TODO: reference additional headers your program requires here
#include <windows.h>
#include <tchar.h>
#endif

#ifdef _WIN32
#include "../BaseLayer/CGSFDefine.h"
#include "CommonHeader.h"
#include "../CGSFNet/ErrorCode.h"
#include "../CGSFNet/EngineConstant.h"
#include "../CGSFNet/IEngine.h"
#include "../CGSFNet/INetworkEngine.h"
#include "../CGSFNet/INetworkCallback.h"
#include "../CGSFNet/IPacketProtocol.h"
#include "../CGSFNet/BasePacket.h"
#include "../BaseLayer/SFString.h"
#include "../BaseLayer/SFObjectPool.h"
#include "../BaseLayer/Macro.h"
#else
#include "../BaseLayerLinux/CGSFDefine.h"
#include "CommonHeader.h"
#include "../CGSFNetLinux/ErrorCode.h"
#include "../CGSFNetLinux/EngineConstant.h"
#include "../CGSFNetLinux/IEngine.h"
#include "../CGSFNetLinux/INetworkEngine.h"
#include "../CGSFNetLinux/INetworkCallback.h"
#include "../CGSFNetLinux/IPacketProtocol.h"
#include "../CGSFNetLinux/BasePacket.h"
#include "../BaseLayerLinux/SFString.h"
#include "../BaseLayerLinux/SFObjectPool.h"
#include "../BaseLayerLinux/Macro.h"
#endif

#include "SFLogicGateway.h"
#include "ILogicEntry.h"
#include "ILogicDispatcher.h"
#include "LogicEntry.h"
#include "SFPacket.h"
#include "SFPacketProtocol.h"

#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"

#define BOOST_ALL_NO_LIB 

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "boost_thread-vc140-mt-gd-1_60.lib")
#pragma comment(lib, "boost_system-vc140-mt-gd-1_60.lib")
#else
#pragma comment(lib, "boost_thread-vc140-mt-1_60.lib")
#pragma comment(lib, "boost_system-vc140-mt-1_60.lib")
#endif
#endif
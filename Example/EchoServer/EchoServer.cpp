#include "EchoServerHeader.h"
#include "EchoLogicEntry.h"
#include "../EngineLayer/SFCGSFPacketProtocol.h"
#include "../EngineLayer/SFMultiLogicDispatcher.h"
//#include "DBStruct.h"
//#include "SFZdb.h"
#include "SFDispatch.h"
#include <functional>
//SFZdb* g_db = nullptr;

void CreateDatabase();

#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
	EchoLogicEntry* pLogicEntry = new EchoLogicEntry();

	auto errorCode = SFEngine::GetInstance()->Intialize(pLogicEntry, new SFMultiLogicDispatcher(10));
	if (errorCode != NET_ERROR_CODE::SUCCESS)
		return 0;

	SFEngine::GetInstance()->AddPacketProtocol(new SFPacketProtocol<SFCGSFPacketProtocol>);	
	SFEngine::GetInstance()->AddTimer(1, 1000, 1000);

	CreateDatabase();

	bool result = SFEngine::GetInstance()->Start();

	if (false == result)
		return 0;

	getchar();
	
	SFEngine::GetInstance()->ShutDown();

	return 0;
}

void CreateDatabase()
{
	//g_db = new SFZdb();

}
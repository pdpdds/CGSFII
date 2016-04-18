#include "WorkerThread.h"
#include "ACEHeader.h"

void ProactorWorkerThread(void* Args)
{
	ACE_Proactor::instance()->proactor_run_event_loop();
}

void ReactorWorkerThread(void* Args)
{
	ACE_Reactor::instance()->run_reactor_event_loop();
}
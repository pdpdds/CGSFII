#include "SFConfigure.h"
#include "../BaseLayer/Markup.h"

SFConfigure::SFConfigure(void)
{
}


SFConfigure::~SFConfigure(void)
{
}

bool SFConfigure::Initialize()
{
	m_ConfigureInfo.Clear();

	return true;
}

bool SFConfigure::Read(const char* pFileName)
{
	CMarkup xml;
	bool result = xml.Load(pFileName);

	if (result == false)
		return false;

	xml.FindChildElem("ENGINENAME");
	m_ConfigureInfo.engineName = xml.GetChildData();
	xml.FindChildElem("SERVERIP");
	m_ConfigureInfo.serverIP = xml.GetChildData();
	xml.FindChildElem("SERVERPORT");
	m_ConfigureInfo.serverPort = atoi(xml.GetChildData().c_str());
	xml.FindChildElem("MAXACCEPT");
	m_ConfigureInfo.maxAccept = atoi(xml.GetChildData().c_str());		

	return true;
}

bool SFConfigure::Write(const char* pFileName)
{

	return true;
}

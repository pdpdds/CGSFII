#pragma once
#include <string>

//////////////////////////////////////////////////////////////
//Shouter Structure
//////////////////////////////////////////////////////////////
typedef struct tag_EngineConfig
{
	std::string engineName;	
	std::string serverIP;
	unsigned short serverPort;
	unsigned short maxAccept;	
	
	tag_EngineConfig()
	{
		Clear();
	}

	void Clear()
	{
		serverPort = 25251;
		maxAccept = 5000;
	}

}_EngineConfig;

class SFConfigure
{
public:
	SFConfigure(void);
	virtual ~SFConfigure(void);

	bool Initialize();

	bool Read(const char* pFileName);
	bool Write(const char* pFileName);

	_EngineConfig* GetConfigureInfo(){return &m_ConfigureInfo;}
	
protected:
	

private:
	_EngineConfig m_ConfigureInfo;
};




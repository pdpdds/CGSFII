#pragma once

#ifndef TCHAR
#define TCHAR wchar_t 
#endif

class SFRegexChecker
{
public:
	SFRegexChecker(void);
	virtual ~SFRegexChecker(void);

	bool IsValidCharName(TCHAR* szStr);
	bool IsValidResidentRegistrationNumber(TCHAR* szStr);
	bool IsValidURL(TCHAR* szStr);
	bool IsValidMacAddress(TCHAR* szStr);
	bool IsValidEMail(TCHAR* szStr);
	bool IsValidIPAddress( TCHAR* szStr );
protected:

private:
};

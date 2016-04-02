#include "SFRegexChecker.h"
#include <regex>
#include <memory>
#include <string>
#include <sstream>

using namespace std;

#define UNICODE_CONSTANT( CONSTANT ) towstring( CONSTANT )

wstring towstring(const char* lpszValue) {
	wostringstream os;
	os << lpszValue;
	return os.str();
}

SFRegexChecker::SFRegexChecker(void)
{
}

SFRegexChecker::~SFRegexChecker(void)
{
}

bool SFRegexChecker::IsValidCharName(TCHAR* szStr)
{

	if(nullptr == szStr)
		return false;
#ifdef _WIN32
	wregex     regexCommandCapturePattern( L"(([°¡-ÆR]|[a-zA-Z0-9_]|[\\-\\[\\]\\(\\)\\{\\}])+)");
	#else
	wstring string = towstring("(([°¡-ÆR]|[a-zA-Z0-9_]|[\\-\\[\\]\\(\\)\\{\\}])+)");
	wregex     regexCommandCapturePattern(string);
#endif // _WIN32

	wsmatch    regexMatchResult;

	std::wstring    wstrInputString(szStr );

	return regex_match( wstrInputString, regexMatchResult, regexCommandCapturePattern );
}

bool SFRegexChecker::IsValidResidentRegistrationNumber( TCHAR* szStr )
{
	if(NULL == szStr)
		return false;

	wregex     regexCommandCapturePattern(L"\\d{6}\\-\\d{7}");
	wsmatch    regexMatchResult;

	std::wstring    wstrInputString(szStr );

	return regex_match( wstrInputString, regexMatchResult, regexCommandCapturePattern );
}

bool SFRegexChecker::IsValidURL( TCHAR* szStr )
{
	if(nullptr == szStr)
		return false;

	wregex     regexCommandCapturePattern(L"(ftp|http|https):\\/\\/(\\w+)(\\.\\w+)*(\\/([\\w\\d])+\\/{0,1})*");
	wsmatch    regexMatchResult;

	std::wstring    wstrInputString(szStr );

	return regex_match( wstrInputString, regexMatchResult, regexCommandCapturePattern );
}

bool SFRegexChecker::IsValidMacAddress( TCHAR* szStr )
{
	if(nullptr == szStr)
		return false;

	wregex     regexCommandCapturePattern(L"([0-9a-fA-F][0-9a-fA-F]-){5}([0-9a-fA-F][0-9a-fA-F])");
	wsmatch    regexMatchResult;

	std::wstring    wstrInputString(szStr );

	return regex_match( wstrInputString, regexMatchResult, regexCommandCapturePattern );
}

bool SFRegexChecker::IsValidEMail( TCHAR* szStr )
{
	if(nullptr == szStr)
		return false;

	wregex     regexCommandCapturePattern(L"[0-9a-zA-Z]([-_.]?[0-9a-zA-Z])*@[0-9a-zA-Z]([-_.]?[0-9a-zA-Z])*[.][a-zA-Z]{2,3}");
	wsmatch    regexMatchResult;

	std::wstring    wstrInputString(szStr );

	return regex_match( wstrInputString, regexMatchResult, regexCommandCapturePattern );
}

bool SFRegexChecker::IsValidIPAddress( TCHAR* szStr )
{
	if(nullptr == szStr)
		return false;

	wregex     regexCommandCapturePattern(L"([1]?\\d{1,2}|[2][0-4]\\d|25[0-5])[.]([1]?\\d{1,2}|[2][0-4]\\d|25[0-5])[.]([1]?\\d{1,2}|[2][0-4]\\d|25[0-5])[.]([1]?\\d{1,2}|[2][0-4]\\d|25[0-5])");

	wsmatch    regexMatchResult;

	std::wstring    wstrInputString(szStr );

	return regex_match( wstrInputString, regexMatchResult, regexCommandCapturePattern );
}
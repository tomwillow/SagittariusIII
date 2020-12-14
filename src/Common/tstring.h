#pragma once
#include <string>
#include <Windows.h>
#include <algorithm>

#ifdef _UNICODE

#define tstring wstring

#define tostream wostream

#define tifstream wifstream

#define tofstream wofstream

#define tistringstream wistringstream

#define tostringstream wostringstream

#define tcout wcout

#define tto_string to_wstring

#else

#define tstring string

#define tostream ostream

#define tifstream ifstream

#define tofstream ofstream

#define tistringstream istringstream

#define tostringstream ostringstream

#define tcout cout

#define tto_string to_string

#endif

inline std::wstring string2wstring(const std::string& str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return std::wstring(L"");

	wchar_t* pwszDst = new wchar_t[nLen];

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return wstr;
}

inline std::string wstring2string(const std::wstring& wstr)
{
	LPCWSTR pwszSrc = wstr.c_str();
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
		return std::string("");

	char* pszDst = new char[nLen];

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	std::string str(pszDst);
	delete[] pszDst;
	pszDst = NULL;

	return str;
}

inline std::string to_string(const std::wstring& ws)
{
	return wstring2string(ws);
}

inline std::wstring to_wstring(const std::string& s)
{
	return string2wstring(s);
}

inline void tolower(std::string& s)
{
	for_each(s.begin(), s.end(), [](char& c) {c = tolower(c); });
}

inline void toupper(std::string& s)
{
	for_each(s.begin(), s.end(), [](char& c) {c = toupper(c); });
}

inline int HexStr2Int(std::string hex_str)
{
	return std::stol(hex_str, nullptr, 16);
}
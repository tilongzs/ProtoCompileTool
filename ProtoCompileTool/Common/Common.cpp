#include "pch.h"
#include "Common.h"
#include <sstream>

CString GetModuleDir()
{
	WCHAR buf[MAX_PATH];
	GetModuleFileName(NULL, buf, MAX_PATH * sizeof(WCHAR));

	GetLongPathName(buf, buf, MAX_PATH * sizeof(WCHAR));
	PathRemoveFileSpec(buf);
	return buf;
}

CString GetModuleDir(const CString& moduleName)
{
	if (moduleName.IsEmpty())
	{
		return GetModuleDir();
	}

	WCHAR buf[MAX_PATH] = { 0 };
	HMODULE hMod = GetModuleHandle(moduleName);
	if (hMod != NULL)
	{
		GetModuleFileName(hMod, buf, MAX_PATH * sizeof(WCHAR));
	}

	if (wcslen(buf) > 0)
	{
		GetLongPathName(buf, buf, MAX_PATH * sizeof(WCHAR));
		PathRemoveFileSpec(buf);
	}
	return buf;
}

string CombinePath(const string& folder, const string& extraPath)
{
	char buf[MAX_PATH];
	strcpy_s(buf, folder.size() + 1, folder.c_str());
	PathAppendA(buf, extraPath.c_str());
	return buf;
}

wstring CombinePath(const wstring& folder, const wstring& extraPath)
{
	wchar_t buf[MAX_PATH];
	wcscpy_s(buf, folder.c_str());
	PathAppend(buf, extraPath.c_str());
	return buf;
}

CString CombinePath(const CString& folder, const CString& extraPath)
{
	WCHAR buf[MAX_PATH] = { 0 };
	wcscpy_s(buf, folder);
	PathAppend(buf, extraPath);
	return buf;
}

CString PathGetDir(const CString& path)
{
	CString tmpPath(path);
	wchar_t* buffer = tmpPath.GetBuffer();
	PathRemoveFileSpec(buffer);
	tmpPath.ReleaseBuffer();

	return tmpPath;
}

CString Double2CStr(const double& num)
{
	CString str;
	str.Format(L"%.8lf", num);
	return str.Trim();
}

double CStr2Double(const CString& str)
{
	double num = 0;

	CString tmp(str);
	wistringstream iss(tmp.GetBuffer());
	iss >> num;
	tmp.ReleaseBuffer();

	return num;
}

#include "pch.h"
#include "Common.h"
#include "ConfigFile.h"

CConfigFile::CConfigFile()
{
}

CConfigFile::CConfigFile(const CString & moduleName)
{
	_moduleName = moduleName;
}

CConfigFile::~CConfigFile()
{
}

void CConfigFile::Initialize(const CString& filename, bool rercreate)
{
	CString workDirectory = GetModuleDir(_moduleName);
	_filePath = CombinePath(workDirectory, filename);

	if (rercreate)
	{
		DeleteFile(_filePath);
	}
}

void CConfigFile::SetModuleName(const CString & name)
{
	_moduleName = name;
}

void CConfigFile::SetString(const CString& strSection, const CString& strKey, const CString& data)
{
	WritePrivateProfileString(strSection, strKey, data, _filePath);
}

void CConfigFile::SetInt(const CString& strSection, const CString& strKey, int data)
{
	CString strData;
	strData.Format(L"%d", data);
	WritePrivateProfileString(strSection, strKey, strData, _filePath);
}

void CConfigFile::SetDouble(const CString& strSection, const CString& strKey, double data)
{
	CString strData;
	strData.Format(L"%lf", data);
	WritePrivateProfileString(strSection, strKey, strData, _filePath);
}

CString CConfigFile::GetString(const CString& strSection, const CString& strKey, const CString& default)
{
	WCHAR buf[256] = { 0 };
	GetPrivateProfileString(strSection, strKey, default, buf, 256, _filePath);

	return buf;
}

int CConfigFile::GetInt(const CString& strSection, const CString& strKey, int default)
{
	return GetPrivateProfileInt(strSection, strKey, default, _filePath);
}

double CConfigFile::GetDouble(const CString& strSection, const CString& strKey, double default)
{
	WCHAR buf[32] = { 0 };
	GetPrivateProfileString(strSection, strKey, Double2CStr(default), buf, 256, _filePath);

	return CStr2Double(buf);
}
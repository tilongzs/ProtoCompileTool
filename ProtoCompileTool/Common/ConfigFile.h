#pragma once

/*
配置文件类。
*/

#define CFGKEY_COMMON	L"Common"
#define CFG_ProtocPath	L"ProtocPath"
#define CFG_PluginPath	L"PluginPath"
#define CFG_SaveType	L"SaveType"
#define CFG_SavePath	L"SavePath"
#define CFG_SelectType	L"SelectType"
#define CFG_ProtoFilesPath	L"ProtoFilesPath"
#define CFG_ProtocLang	L"ProtocLang"
#define CFG_ProtoImportPath	L"ProtoImportPath"

class CConfigFile
{
public:
	CConfigFile();
	CConfigFile(const CString& moduleName);
	~CConfigFile();

private:
	CString		_filePath;
	CString		_moduleName;	// 模块名称。默认为主程序。
public:
	void Initialize(const CString& filename, bool rercreate = false);
	void SetModuleName(const CString& name);	// 设置使用的程序模块名称。默认为主程序。

	void SetString(const CString& strSection, const CString& strKey, const CString& data);
	void SetInt(const CString& strSection, const CString& strKey, int data);
	void SetDouble(const CString& strSection, const CString& strKey, double data);
	CString GetString(const CString& strSection, const CString& strKey, const CString& default = L"");
	int GetInt(const CString& strSection, const CString& strKey, int default = 0);
	double GetDouble(const CString& strSection, const CString& strKey, double default = 0);
};

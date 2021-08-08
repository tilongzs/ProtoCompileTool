#include <string>
using namespace std;

// 路径操作
CString GetModuleDir();	// 获得主程序所在路径
CString GetModuleDir(const CString& moduleName);// 获得指定程序模块所在路径，例如xxx.dll
string CombinePath(const string& folder, const string& extraPath);
wstring CombinePath(const wstring& folder, const wstring& extraPath);
CString CombinePath(const CString& folder, const CString& extraPath);
CString PathGetDir(const CString& path);	// 去除文件名，得到目录

// 数字转换
CString Double2CStr(const double& num);
double CStr2Double(const CString& str);
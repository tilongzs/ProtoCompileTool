#include <string>
using namespace std;

// ·������
CString GetModuleDir();	// �������������·��
CString GetModuleDir(const CString& moduleName);// ���ָ������ģ������·��������xxx.dll
string CombinePath(const string& folder, const string& extraPath);
wstring CombinePath(const wstring& folder, const wstring& extraPath);
CString CombinePath(const CString& folder, const CString& extraPath);
CString PathGetDir(const CString& path);	// ȥ���ļ������õ�Ŀ¼

// ����ת��
CString Double2CStr(const double& num);
double CStr2Double(const CString& str);
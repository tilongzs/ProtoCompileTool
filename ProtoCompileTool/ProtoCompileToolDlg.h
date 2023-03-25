#pragma once
#include "Common/ConfigFile.h"
#include <memory>
#include <functional>
using namespace std;

class CProtoCompileToolDlg : public CDialogEx
{
public:
	CProtoCompileToolDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROTOCOMPILETOOL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	struct TheadFunc
	{
		function<void()> Func;
	};
protected:
	HICON m_hIcon;
	CEdit _editProtocPath;
	CEdit _editPluginPath;
	CComboBox _comboboxProtocLang;
	CComboBox _comboboxSaveType;
	CEdit _editSavePath;
	CComboBox _comboboxSelectType;
	CEdit _editProtoPath;
	CButton _btnSavePath;
	CButton _btnProtoPath;
	CEdit _editRecv;
	CEdit _editImportPath;
	CButton _btnEnableGRPC;

	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	shared_ptr<CConfigFile>	_config = nullptr;

	void LoadConfig();
	bool IsProtoFileHasService(const CString& protoPath);
	bool RunProtoc(const CString& protocPath, CString param);
	CString GetProtocLang();
	void AppendMsg(const WCHAR* msg);
	LRESULT OnFunction(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBtnProtocPath();
	afx_msg void OnBtnPluginPath();
	afx_msg void OnBtnSavePath();
	afx_msg void OnCbnSelchangeComboSaveType();
	afx_msg void OnCbnSelchangeComboSelectType();
	afx_msg void OnBtnProtoPath();
	afx_msg void OnBtnGenerate();
	afx_msg void OnBtnClearPluginPath();
	afx_msg void OnCbnSelchangeComboProtocLang();
	afx_msg void OnBtnImportPath();
};

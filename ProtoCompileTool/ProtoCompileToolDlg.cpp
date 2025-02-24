#include "pch.h"
#include "framework.h"
#include "ProtoCompileTool.h"
#include "ProtoCompileToolDlg.h"
#include "afxdialogex.h"
#include "Common/Common.h"
#include <direct.h>
#include <corecrt_io.h>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WMSG_FUNCTION		WM_USER + 1

CProtoCompileToolDlg::CProtoCompileToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROTOCOMPILETOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProtoCompileToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROTOC_PATH, _editProtocPath);
	DDX_Control(pDX, IDC_EDIT_PLUGIN_PATH, _editPluginPath);
	DDX_Control(pDX, IDC_COMBO_SAVE_TYPE, _comboboxSaveType);
	DDX_Control(pDX, IDC_EDIT_SAVE_PATH, _editSavePath);
	DDX_Control(pDX, IDC_COMBO_SELECT_TYPE, _comboboxSelectType);
	DDX_Control(pDX, IDC_EDIT_PROTO_PATH, _editProtoPath);
	DDX_Control(pDX, IDC_BTN_SAVE_PATH, _btnSavePath);
	DDX_Control(pDX, IDC_BTN_PROTO_PATH, _btnProtoPath);
	DDX_Control(pDX, IDC_EDIT_RECV, _editRecv);
	DDX_Control(pDX, IDC_EDIT_IMPORT_PATH, _editImportPath);
	DDX_Control(pDX, IDC_BTN_GENERATE, _btnGenerate);
	DDX_Control(pDX, IDC_COMBO_PROTOC_LANG, _comboboxProtocLang);
}

BEGIN_MESSAGE_MAP(CProtoCompileToolDlg, CDialogEx)
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WMSG_FUNCTION, &CProtoCompileToolDlg::OnFunction)
	ON_BN_CLICKED(IDC_BTN_PROTOC_PATH, &CProtoCompileToolDlg::OnBtnProtocPath)
	ON_BN_CLICKED(IDC_BTN_PLUGIN_PATH, &CProtoCompileToolDlg::OnBtnPluginPath)
	ON_BN_CLICKED(IDC_BTN_SAVE_PATH, &CProtoCompileToolDlg::OnBtnSavePath)
	ON_CBN_SELCHANGE(IDC_COMBO_SAVE_TYPE, &CProtoCompileToolDlg::OnCbnSelchangeComboSaveType)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT_TYPE, &CProtoCompileToolDlg::OnCbnSelchangeComboSelectType)
	ON_BN_CLICKED(IDC_BTN_PROTO_PATH, &CProtoCompileToolDlg::OnBtnProtoPath)
	ON_BN_CLICKED(IDC_BTN_GENERATE, &CProtoCompileToolDlg::OnBtnGenerate)
	ON_BN_CLICKED(IDC_BTN_IMPORT_PATH, &CProtoCompileToolDlg::OnBtnImportPath)
	ON_EN_CHANGE(IDC_EDIT_PROTOC_PATH, &CProtoCompileToolDlg::OnEnChangeEditProtocPath)
	ON_EN_CHANGE(IDC_EDIT_PLUGIN_PATH, &CProtoCompileToolDlg::OnEnChangeEditPluginPath)
	ON_EN_CHANGE(IDC_EDIT_IMPORT_PATH, &CProtoCompileToolDlg::OnEnChangeEditImportPath)
	ON_EN_CHANGE(IDC_EDIT_SAVE_PATH, &CProtoCompileToolDlg::OnEnChangeEditSavePath)
	ON_EN_CHANGE(IDC_EDIT_PROTO_PATH, &CProtoCompileToolDlg::OnEnChangeEditProtoPath)
	ON_CBN_SELCHANGE(IDC_COMBO_PROTOC_LANG, &CProtoCompileToolDlg::OnCbnSelchangeComboProtocLang)
END_MESSAGE_MAP()

BOOL CProtoCompileToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// 加载配置文件
	LoadConfig();

	return TRUE;
}

HCURSOR CProtoCompileToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProtoCompileToolDlg::LoadConfig()
{
	_config = make_shared<CConfigFile>();
	_config->Initialize(L"config.ini");

	// 读取编译语言配置
	_comboboxProtocLang.AddString(L"C++");
	_comboboxProtocLang.AddString(L"Java");
	int protocLangIndex = _config->GetInt(CFGKEY_COMMON, CFG_ProtocLang);
	_comboboxProtocLang.SetCurSel(protocLangIndex);
	_protocLang = (ProtocLang)protocLangIndex;

	// 读取protoc.exe路径
	CString protocPath = _config->GetString(CFGKEY_COMMON, CFG_ProtocPath);
	if (!protocPath.IsEmpty())
	{
		if (PathFileExists(protocPath))
		{
			_editProtocPath.SetWindowText(protocPath);
		}
		else
		{
			// 删除配置
			_config->SetString(CFGKEY_COMMON, CFG_ProtocPath, L"");

			// 提示路径
			AppendMsg(L"protoc.exe路径 例如：vcpkg\\installed\\x64-windows\\tools\\protobuf\\protoc.exe");
		}
	}
	else
	{
		// 提示路径
		AppendMsg(L"protoc.exe路径 例如：vcpkg\\installed\\x64-windows\\tools\\protobuf\\protoc.exe");
	}

	// 读取使用proto方式配置
	_comboboxSelectType.AddString(L"单个proto文件");
	_comboboxSelectType.AddString(L"包含proto文件的文件夹");
	int selectType = _config->GetInt(CFGKEY_COMMON, CFG_SelectType);
	if (selectType == 1)
	{
		CString protoPath = _config->GetString(CFGKEY_COMMON, CFG_ProtoFilesPath);
		if (PathIsDirectory(protoPath))
		{
			_editProtoPath.SetWindowText(protoPath);
		}
		else
		{
			// 删除配置
			_config->SetString(CFGKEY_COMMON, CFG_ProtoFilesPath, L"");
		}
	}
	_comboboxSelectType.SetCurSel(selectType);

	// 读取生成代码文件的存放路径配置
	_btnSavePath.EnableWindow(FALSE);
	_editSavePath.EnableWindow(FALSE);
	_comboboxSaveType.AddString(L"proto同级文件夹");
	_comboboxSaveType.AddString(L"指定文件夹");
	int saveType = _config->GetInt(CFGKEY_COMMON, CFG_SaveType);	
	_comboboxSaveType.SetCurSel(saveType);

	// 读取通用proto文件夹路径
	CString importPath = _config->GetString(CFGKEY_COMMON, CFG_ProtoImportPath);
	if (!importPath.IsEmpty())
	{
		if (PathIsDirectory(importPath))
		{
			_editImportPath.SetWindowText(importPath);
		}
		else
		{
			// 删除配置
			_config->SetString(CFGKEY_COMMON, CFG_ProtoImportPath, L"");

			// 提示路径
			AppendMsg(L"通用proto（如empty.proto）的inlucde路径 例如：vcpkg\\installed\\x64-windows\\include");
		}
	}
	else
	{
		// 提示路径
		AppendMsg(L"通用的proto（如empty.proto）的inlucde路径 例如：vcpkg\\installed\\x64-windows\\include");
	}

	OnCbnSelchangeComboProtocLang();
}

bool CProtoCompileToolDlg::IsProtoFileHasService(const CString& protoPath)
{
	bool isHas = false;
	CStdioFile file;
	if (!file.Open(protoPath, CFile::modeRead))
	{
		return isHas;
	}

	WCHAR buf[BUFSIZ + 1];
	while (file.ReadString(buf, BUFSIZ))
	{
		// 通过检查是否包含service 来判断是否是RPC文件
		if (wcsstr(buf, L"service ") != nullptr)
		{
			isHas = true;
			break;
		}
	}

	file.Close();
	return isHas;
}

bool CProtoCompileToolDlg::RunProtoc(const CString& protocPath, CString param)
{
	PROCESS_INFORMATION ProceInfo;
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	HANDLE hRead = NULL;
	HANDLE hWrite = NULL;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return false;
	}

	STARTUPINFO StartInfo;
	ZeroMemory(&StartInfo, sizeof(StartInfo));
	StartInfo.cb = sizeof(StartInfo);
	StartInfo.wShowWindow = SW_HIDE;
	//使用指定的句柄作为标准输入输出的文件句柄,使用指定的显示方式
	StartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	StartInfo.hStdError = hWrite;
	StartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	StartInfo.hStdOutput = hWrite;

	bool ret = true;
	param.Format(L"%s %s", protocPath, param);
	if (CreateProcess(nullptr, param.GetBuffer(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &StartInfo, &ProceInfo))
	{
		WaitForSingleObject(ProceInfo.hProcess, /*INFINITE*/3000); // 等待3秒

		CHAR chBuf[1024] = { 0 };
		DWORD dwRead = 0;
		DWORD dwAvail = 0;
		if (PeekNamedPipe(hRead, NULL, NULL, &dwRead, &dwAvail, NULL) && dwAvail > 0)//PeekNamePipe用来预览一个管道中的数据，用来判断管道中是否为空
		{
			BOOL bSuccess = ReadFile(hRead, chBuf, 1024, &dwRead, NULL); // 这里是读管道，即便已经没有数据，仍然会等待接收数据，所以才需要PeekNamePipe
			if (bSuccess && dwRead != 0)
			{
				// 读取到错误消息
				ret = false;
				AppendMsg(CString(chBuf));
			}
		}

		CloseHandle(ProceInfo.hThread);
		CloseHandle(ProceInfo.hProcess);
	}
	else
	{
		ret = false;
	}

	CloseHandle(hRead);
	CloseHandle(hWrite);

	return ret;
}

void CProtoCompileToolDlg::OnBtnProtocPath()
{
	CString defaultDir = GetModuleDir();	//默认打开的文件路径
	CString fileName = L"";			//默认打开的文件名
	CString filter = L"protoc.exe|protoc.exe||";	//文件过虑的类型
	CFileDialog openFileDlg(TRUE, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK)
	{
		CString protocPath = openFileDlg.GetPathName();
		_editProtocPath.SetWindowText(protocPath);

		// 保存配置
		_config->SetString(CFGKEY_COMMON, CFG_ProtocPath, protocPath);
	}
}

void CProtoCompileToolDlg::OnEnChangeEditProtocPath()
{
	CString newPath;
	_editProtocPath.GetWindowText(newPath);
	// 保存配置
	_config->SetString(CFGKEY_COMMON, CFG_ProtocPath, newPath);
}

void CProtoCompileToolDlg::OnBtnPluginPath()
{
	CString defaultDir = GetModuleDir();	//默认打开的文件路径
	CString fileName = L"";			//默认打开的文件名
	CString filter = L"插件|*.exe||";	//文件过虑的类型
	CFileDialog openFileDlg(TRUE, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK)
	{
		CString pluginPath = openFileDlg.GetPathName();
		_editPluginPath.SetWindowText(pluginPath);
	}
}

void CProtoCompileToolDlg::OnEnChangeEditPluginPath()
{
	CString newPath;
	_editPluginPath.GetWindowText(newPath);
	// 保存配置
	switch (_protocLang)
	{
	case ProtocLang::CPP:
	{
		_config->SetString(CFGKEY_CPP, CFG_PluginPath, newPath);
	}
	break;
	case ProtocLang::Java:
	{
		_config->SetString(CFGKEY_Java, CFG_PluginPath, newPath);
	}
	break;
	}
}

void CProtoCompileToolDlg::OnBtnSavePath()
{
	TCHAR szFolderPath[MAX_PATH] = { 0 };
	BROWSEINFO sInfo;
	ZeroMemory(&sInfo, sizeof(BROWSEINFO));

	sInfo.pidlRoot = CSIDL_DESKTOP;//文件夹的根目录，此处为桌面
	sInfo.lpszTitle = L"请选择一个文件夹";
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_NEWDIALOGSTYLE;

	// 显示文件夹选择对话框
	LPITEMIDLIST lpidlBrowse = SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != nullptr)
	{
		// 取得文件夹名
		if (SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			_editSavePath.SetWindowText(szFolderPath);

			// 保存配置
			_config->SetString(CFGKEY_COMMON, CFG_SavePath, szFolderPath);
		}

		CoTaskMemFree(lpidlBrowse);
	}
}

void CProtoCompileToolDlg::OnEnChangeEditSavePath()
{
	CString newPath;
	_editSavePath.GetWindowText(newPath);
	// 保存配置
	switch (_protocLang)
	{
	case CPP:
		_config->SetString(CFGKEY_CPP, CFG_SavePath, newPath);
		break;
	case Java:
		_config->SetString(CFGKEY_Java, CFG_SavePath, newPath);
		break;
	}	
}

void CProtoCompileToolDlg::OnBtnProtoPath()
{
	if (_comboboxSelectType.GetCurSel() == 0) // 单个proto文件
	{
		CString defaultDir = GetModuleDir();	//默认打开的文件路径
		CString fileName = L"";			//默认打开的文件名
		CString filter = L"proto文件|*.proto||";	//文件过虑的类型
		CFileDialog openFileDlg(TRUE, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
		INT_PTR result = openFileDlg.DoModal();
		if (result == IDOK)
		{
			CString protoFilePath = openFileDlg.GetPathName();
			_editProtoPath.SetWindowText(protoFilePath);
		}
	}
	else// 包含proto文件的文件夹
	{
		TCHAR szFolderPath[MAX_PATH] = { 0 };
		BROWSEINFO sInfo;
		ZeroMemory(&sInfo, sizeof(BROWSEINFO));

		sInfo.pidlRoot = CSIDL_DESKTOP;//文件夹的根目录，此处为桌面
		sInfo.lpszTitle = L"请选择proto文件所在文件夹";
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_NEWDIALOGSTYLE;

		// 显示文件夹选择对话框
		LPITEMIDLIST lpidlBrowse = SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != nullptr)
		{
			// 取得文件夹名
			if (SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			{
				_editProtoPath.SetWindowText(szFolderPath);

				// 保存配置
				_config->SetString(CFGKEY_COMMON, CFG_ProtoFilesPath, szFolderPath);
			}

			CoTaskMemFree(lpidlBrowse);
		}
	}
}

void CProtoCompileToolDlg::OnEnChangeEditProtoPath()
{
	CString newPath;
	_editProtoPath.GetWindowText(newPath);
	// 保存配置
	_config->SetString(CFGKEY_COMMON, CFG_ProtoFilesPath, newPath);
}

void CProtoCompileToolDlg::OnCbnSelchangeComboSaveType()
{
	int saveType = _comboboxSaveType.GetCurSel();
	_btnSavePath.EnableWindow(saveType == 1);
	_editSavePath.EnableWindow(saveType == 1);
	_config->SetInt(CFGKEY_COMMON, CFG_SaveType, saveType);

	if (saveType == 1)
	{
		CString savePath;
		switch (_protocLang)
		{
		case CPP:
			savePath = _config->GetString(CFGKEY_CPP, CFG_SavePath);
			break;
		case Java:
			savePath = _config->GetString(CFGKEY_Java, CFG_SavePath);
			break;
		}
		if (savePath.IsEmpty())
		{
			_editSavePath.SetWindowText(savePath);
		}
		else
		{
			if (PathIsDirectory(savePath))
			{
				_editSavePath.SetWindowText(savePath);
			}
			else
			{
				savePath = L"";
				// 删除配置
				switch (_protocLang)
				{
				case CPP:
					_config->SetString(CFGKEY_CPP, CFG_SavePath, savePath);
					break;
				case Java:
					_config->GetString(CFGKEY_Java, CFG_SavePath, savePath);
					break;
				}
			}

			_editSavePath.SetWindowText(savePath);
		}		
	}
	else
	{
		_editSavePath.Clear();
	}
}

void CProtoCompileToolDlg::OnCbnSelchangeComboSelectType()
{
	int selectType = _comboboxSelectType.GetCurSel();
	_editProtoPath.SetWindowText(L"");
	_config->SetInt(CFGKEY_COMMON, CFG_SelectType, selectType);
}

void CProtoCompileToolDlg::OnBtnGenerate()
{
	// 检查配置项
	// protoc位置
	CString protocPath;
	_editProtocPath.GetWindowText(protocPath);
	if (protocPath.IsEmpty())
	{
		MessageBox(L"请选择protoc.exe位置");
		return;
	}
	_config->SetString(CFGKEY_COMMON, CFG_ProtocPath, protocPath);

	// gRPC插件路径（可选）
	CString pluginPath;
	_editPluginPath.GetWindowText(pluginPath);

	// 通用proto文件所在文件夹（可选）
	CString importPath;
	_editImportPath.GetWindowText(importPath);
	CString commonProtoPath;
	if (!importPath.IsEmpty())
	{
		_config->SetString(CFGKEY_COMMON, CFG_ProtoImportPath, importPath);
		commonProtoPath.Format(L" --proto_path=\"%s\"", importPath);
	}

	// 缓存所有proto文件路径
	CString protoPath;
	_editProtoPath.GetWindowText(protoPath);
	vector<CString> protoFiles;
	if (_comboboxSelectType.GetCurSel() == 0) // 单个proto文件
	{
		// 检查文件是否存在
		if (protoPath.IsEmpty())
		{
			MessageBox(L"请选择一个proto文件");
			return;
		}

		if (!PathFileExists(protoPath))
		{
			MessageBox(L"proto文件不存在");
			_editProtoPath.SetWindowText(L"");
			return;
		}

		protoFiles.push_back(protoPath);
	}
	else // 包含proto文件的文件夹
	{
		if (!PathIsDirectory(protoPath))
		{
			MessageBox(L"proto文件夹路径错误");
			_editProtoPath.SetWindowText(L"");
			return;
		}

		// 遍历文件夹
		_chdir(CStringA(protoPath));
		_finddata_t fileinfo;
		intptr_t hFile = _findfirst("*.proto", &fileinfo);
		if (hFile != -1)
		{
			do
			{
				// 检查是不是目录，如果不是则保存
				if (!(fileinfo.attrib & _A_SUBDIR))
				{
					protoFiles.push_back(CombinePath(protoPath, CString(fileinfo.name)));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}

	// 生成代码文件的存放路径
	CString outputDir;
	if (_comboboxSaveType.GetCurSel() == 0) // proto同级文件夹
	{
		if (_comboboxSelectType.GetCurSel() == 0) // 单个proto文件
		{
			outputDir = PathGetDir(protoPath);
		}
		else // 包含proto文件的文件夹
		{
			outputDir = protoPath;
		}
	}
	else // 指定文件夹
	{
		_editSavePath.GetWindowText(outputDir);
		if (!PathIsDirectory(outputDir))
		{
			MessageBox(L"存放位置不存在");
			_editSavePath.SetWindowText(L"");
			return;
		}
	}

	// 保存配置
	_config->SetString(CFGKEY_COMMON, CFG_SavePath, outputDir);
	_config->SetString(CFGKEY_COMMON, CFG_ProtoFilesPath, protoPath);

	// 开始转换
	_btnGenerate.SetWindowText(L"正在转换...");

	CString param, protocCmd, pluginCmd;
	for each (const auto & filePath in protoFiles)
	{
		// 生成Protobuf类文件
		protocCmd = GetProtocCmd();
		param.Format(L"-I \"%s\" --%s=\"%s\" \"%s\"", PathGetDir(filePath), protocCmd, outputDir, filePath);
		if (!commonProtoPath.IsEmpty())
		{
			param += commonProtoPath;
		}

		if (!RunProtoc(protocPath, param))
		{
			_btnGenerate.SetWindowText(L"开始转换");
			return;
		}

		// 生成gRPC类文件
		if (!pluginPath.IsEmpty())
		{
			// 检查该proto文件里是否包含service
			if (IsProtoFileHasService(filePath))
			{
				param.Format(L"-I \"%s\" --grpc_out=\"%s\" --plugin=protoc-gen-grpc=\"%s\" \"%s\"", PathGetDir(filePath), outputDir, pluginPath, filePath);
			}

			if (!commonProtoPath.IsEmpty())
			{
				param += commonProtoPath;
			}

			if (!RunProtoc(protocPath, param))
			{
				_btnGenerate.SetWindowText(L"开始转换");
				return;
			}
		}
	}

	_btnGenerate.SetWindowText(L"开始转换");
	AppendMsg(L"转换完成");
	MessageBox(L"转换完成");
}

void CProtoCompileToolDlg::AppendMsg(const WCHAR* msg)
{
	WCHAR* tmpMsg = new WCHAR[wcslen(msg) + 1];
	memset(tmpMsg, 0, sizeof(WCHAR) * (wcslen(msg) + 1));
	wsprintf(tmpMsg, msg);

	TheadFunc* pFunc = new TheadFunc;
	pFunc->Func = ([=]()
		{
			if (_editRecv.GetLineCount() > 100)
			{
				_editRecv.Clear();
			}

			CString curMsg;
			_editRecv.GetWindowTextW(curMsg);
			curMsg += "\r\n";

			CString strTime;
			SYSTEMTIME   tSysTime;
			GetLocalTime(&tSysTime);
			strTime.Format(L"[%02ld:%02ld:%02ld.%03ld] ",
				tSysTime.wHour, tSysTime.wMinute, tSysTime.wSecond, tSysTime.wMilliseconds);

			curMsg += strTime;
			curMsg += tmpMsg;
			_editRecv.SetWindowTextW(curMsg);
			_editRecv.LineScroll(_editRecv.GetLineCount());

			delete[] tmpMsg;
		});

	PostMessage(WMSG_FUNCTION, (WPARAM)pFunc);
}

LRESULT CProtoCompileToolDlg::OnFunction(WPARAM wParam, LPARAM lParam)
{
	TheadFunc* pFunc = (TheadFunc*)wParam;
	pFunc->Func();
	delete pFunc;

	return TRUE;
}

CString CProtoCompileToolDlg::GetProtocCmd()
{
	switch (_protocLang)
	{
	case ProtocLang::CPP: return L"cpp_out";
	case ProtocLang::Java: return L"java_out";
	default:
		return L"cpp_out";
	}
}

void CProtoCompileToolDlg::OnBtnImportPath()
{
	TCHAR szFolderPath[MAX_PATH] = { 0 };
	BROWSEINFO sInfo;
	ZeroMemory(&sInfo, sizeof(BROWSEINFO));

	sInfo.pidlRoot = CSIDL_DESKTOP;//文件夹的根目录，此处为桌面
	sInfo.lpszTitle = L"请选择引用的通用proto文件所在文件夹";
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_NEWDIALOGSTYLE;

	// 显示文件夹选择对话框
	LPITEMIDLIST lpidlBrowse = SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != nullptr)
	{
		// 取得文件夹名
		if (SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			_editImportPath.SetWindowText(szFolderPath);

			// 保存配置
			_config->SetString(CFGKEY_COMMON, CFG_ProtoImportPath, szFolderPath);
		}

		CoTaskMemFree(lpidlBrowse);
	}
}

void CProtoCompileToolDlg::OnEnChangeEditImportPath()
{
	CString newPath;
	_editImportPath.GetWindowText(newPath);
	// 保存配置
	_config->SetString(CFGKEY_COMMON, CFG_ProtoImportPath, newPath);
}

void CProtoCompileToolDlg::OnCbnSelchangeComboProtocLang()
{
	int protocLangIndex = _comboboxProtocLang.GetCurSel();
	_config->SetInt(CFGKEY_COMMON, CFG_ProtocLang, protocLangIndex);
	_protocLang = (ProtocLang)protocLangIndex;

	// 重新读取gRPC插件地址
	CString pluginPath;
	switch (_protocLang)
	{
	case ProtocLang::CPP:
	{
		pluginPath = _config->GetString(CFGKEY_CPP, CFG_PluginPath);
		if (pluginPath.IsEmpty())
		{
			AppendMsg(L"gRPC插件路径 例如：vcpkg\\installed\\x64-windows\\tools\\gRPC\\grpc_cpp_plugin.exe");
		}
	}
	break;
	case ProtocLang::Java:
	{
		pluginPath = _config->GetString(CFGKEY_Java, CFG_PluginPath);
		if (pluginPath.IsEmpty())
		{
			AppendMsg(L"gRPC插件路径 例如：protoc-gen-grpc-java.exe");
		}
	}
	break;
	}
	_editPluginPath.SetWindowText(pluginPath);

	// 重新读取生成代码文件的存放路径
	OnCbnSelchangeComboSaveType();
}

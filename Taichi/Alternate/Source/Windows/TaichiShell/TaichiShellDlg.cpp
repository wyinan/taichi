
// TaichiShellDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CmdRun.h"
#include "RapidJsonUtil.h"
#include "TaichiShell.h"
#include "TaichiShellDlg.h"
#include "afxdialogex.h"


#define INIDC_TAICHI_PANEL	1408


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CTaichiShellDlg 对话框



CTaichiShellDlg::CTaichiShellDlg(CString csJsonPath, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAICHISHELL_DIALOG, pParent)
	, m_csJsonPath(csJsonPath)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTaichiShellDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTaichiShellDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DROPFILES()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CTaichiShellDlg 消息处理程序

BOOL CTaichiShellDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetDropMsgFilter();

	MoveWindow(0, 0, 200, 200);
	
	CenterWindow();

	LoadJson();

	InitCtrl();
	
	m_tcPanel.SetWindowText(m_csName);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CTaichiShellDlg::InitCtrl()
{
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcPanel(rcClient);
	
	BOOL bRet = m_tcPanel.Create(_T(""),
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | SS_CENTER,
		rcPanel, this, INIDC_TAICHI_PANEL);

	ASSERT(bRet);
}


void CTaichiShellDlg::LoadJson()
{
	using namespace rapidjson;

	if (m_csJsonPath.IsEmpty())
	{
		return;
	}

	std::string strContent;
	Document document;

	BOOL bRet = CRapidJsonUtil::LoadDocument(m_csJsonPath, strContent, document);
	if (!bRet)
	{
		return;
	}

	/* 字典 */
	CSCouple mapDict;
	(VOID)CRapidJsonUtil::LoadMap(document, mapDict);

	m_csName = mapDict[_T("name")];
	m_csFunction = mapDict[_T("function")];

	return;
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTaichiShellDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTaichiShellDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTaichiShellDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pOldMousePos = point;
	(VOID)::SetCapture(m_hWnd);
	
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CTaichiShellDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	(VOID)::ReleaseCapture();
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CTaichiShellDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (::GetCapture() == m_hWnd)
	{
		(VOID)SetCursor(LoadCursor(NULL, IDC_SIZEALL));

		int nDiffX = point.x - m_pOldMousePos.x;
		int nDiffY = point.y - m_pOldMousePos.y;
		CRect rcWin;
		(VOID)GetWindowRect(rcWin);
		(VOID)SetWindowPos(&wndTop, rcWin.left + nDiffX,
			rcWin.top + nDiffY, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CTaichiShellDlg::SetDropMsgFilter() const
{
	typedef BOOL(WINAPI  *ChangeWindowMessageFilterEx)(
		__in         HWND hWnd,
		__in         UINT message,
		__in         DWORD action,
		__inout_opt  PCHANGEFILTERSTRUCT pChangeFilterStruct
		);

	static ChangeWindowMessageFilterEx pMessFilter = (ChangeWindowMessageFilterEx)
		GetProcAddress(GetModuleHandle(_T("user32.dll")),
			"ChangeWindowMessageFilterEx");
	if (pMessFilter == NULL)
	{
		return;
	}

	// 解决win7下，特殊情况下拖拉文件到导出窗口无效的问题	
	(void)pMessFilter(m_hWnd, WM_DROPFILES, TRUE, NULL);

	// win7才有的消息类型，VS2008还没有这个宏定义
	(void)pMessFilter(m_hWnd, 0x49, TRUE, NULL);

	return;
}


void CTaichiShellDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString csFilePath;
	UINT uFileCnt = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (UINT nIndex = 0; nIndex < uFileCnt; ++nIndex)
	{
		UINT dwMaxCch = DragQueryFile(hDropInfo, nIndex, NULL, 0) + 1;

		(VOID)DragQueryFile(hDropInfo, nIndex,
			csFilePath.GetBufferSetLength(dwMaxCch), dwMaxCch);
		csFilePath.ReleaseBuffer();
		
		DWORD dwRet = GetFileAttributes(csFilePath);
		if (!(dwRet &= FILE_ATTRIBUTE_DIRECTORY))
		{
			m_csJsonPath = csFilePath;
			LoadJson();
			m_tcPanel.SetWindowText(m_csName);
		}

		break;
	}

	//完成拖入文件操作，让系统释放缓冲区   
	DragFinish(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}


static BOOL GUExtractSubString(CString& rString, LPCTSTR lpszFullString,
	int iSubString, TCHAR chSep)
{
	if (lpszFullString == NULL)
		return FALSE;

	while (iSubString--)
	{
		lpszFullString = _tcschr(lpszFullString, chSep);
		if (lpszFullString == NULL)
		{
			// 使用默认值
			// rString.Empty();        // return empty string as well 
			return FALSE;
		}
		lpszFullString++;       // point past the separator
	}
	LPCTSTR lpchEnd = _tcschr(lpszFullString, chSep);
	int nLen = (lpchEnd == NULL) ?
		static_cast<int>(_tcslen(lpszFullString)) : (int)(lpchEnd - lpszFullString);
	ASSERT(nLen >= 0);
	if (nLen == 0)
	{
		// 如果值为空，使用默认值
		return 2;
	}
	Checked::memcpy_s(rString.GetBufferSetLength(nLen), nLen * sizeof(TCHAR),
		lpszFullString, nLen * sizeof(TCHAR));
	rString.ReleaseBuffer();	// Need to call ReleaseBuffer 
								// after calling GetBufferSetLength
	return TRUE;
}



void CTaichiShellDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_csName.IsEmpty() && !m_csFunction.IsEmpty())
	{
		int nIndex = 0;
		do
		{
			CString csFunction;
			(VOID)GUExtractSubString(csFunction, (LPCTSTR)m_csFunction, nIndex++, ',');
			if (!csFunction.IsEmpty())
			{
				CString csCmd;
				csCmd.Format(_T("TaichiShell.exe %s.json"), csFunction);
				(VOID)CmdRun::RunCommandProcessor(csCmd);
			}
			else
			{
				break;
			}
		} while (nIndex);
	}
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

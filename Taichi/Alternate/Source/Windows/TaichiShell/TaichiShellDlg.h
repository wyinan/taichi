
// TaichiShellDlg.h: 头文件
//

#pragma once

#include "TaichiPanel.h"

// CTaichiShellDlg 对话框
class CTaichiShellDlg : public CDialogEx
{
// 构造
public:
	CTaichiShellDlg(CString csJsonPath, CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAICHISHELL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	void SetDropMsgFilter() const;

// 实现
protected:
	HICON m_hIcon;
	CPoint m_pOldMousePos;
	CTaichiPanel m_tcPanel;
	CString m_csJsonPath;
	CString m_csName;
	CString m_csFunction;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void InitCtrl();
	void LoadJson();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

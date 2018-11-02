#pragma once
#include <afx.h>
#include <afxext.h>
#include <afxcmn.h>

// CTaichiPanel

class CTaichiPanel : public CStatic
{
	DECLARE_DYNAMIC(CTaichiPanel)

public:
	CTaichiPanel();
	virtual ~CTaichiPanel();

protected:
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg void OnPaint();

protected:
	DECLARE_MESSAGE_MAP()
};



// BacklightControl.h : main header file for the BACKLIGHTCONTROL application
//

#if !defined(AFX_BACKLIGHTCONTROL_H__6A85D9C1_905F_4F3A_9D44_21D6C1E32CDA__INCLUDED_)
#define AFX_BACKLIGHTCONTROL_H__6A85D9C1_905F_4F3A_9D44_21D6C1E32CDA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBacklightControlApp:
// See BacklightControl.cpp for the implementation of this class
//

class CBacklightControlApp : public CWinApp
{
public:
	CBacklightControlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBacklightControlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBacklightControlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BACKLIGHTCONTROL_H__6A85D9C1_905F_4F3A_9D44_21D6C1E32CDA__INCLUDED_)

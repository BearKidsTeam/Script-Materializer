// TestEditor.h : main header file for the EDITOR DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class TestEditorDlg;
class TestToolbarDlg;

extern TestEditorDlg*		g_Editor;
extern TestToolbarDlg*		g_Toolbar;

//plugin interface for communication with Virtools Dev
extern PluginInterface*		s_Plugininterface;

/////////////////////////////////////////////////////////////////////////////
// CEditorApp
// See Editor.cpp for the implementation of this class
//

class CTestEditorApp : public CWinApp
{

public:
	CTestEditorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TestCEditorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(TestCEditorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

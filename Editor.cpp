// Editor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Editor.h"
#include "Callback.h"

#ifdef _MFCDEBUGNEW
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

TestEditorDlg*		g_Editor			= NULL;
TestToolbarDlg*		g_Toolbar			= NULL;

//----------------------------------------
//The Plugin Info structure that must be filled for Virtools Dev to load effectively the plugin
PluginInfo g_PluginInfo0;

//Returns the number of plugin contained in this dll
//this function must be exported (have a .def file with its name or use __declspec( dllexport )
int GetVirtoolsPluginInfoCount()
{
	return 1;
}

//returns the ptr of the (index)th plugininfo structure of this dll
//this function must be exported (have a .def file with its name or use __declspec( dllexport )
PluginInfo* GetVirtoolsPluginInfo(int index)
{
	switch(index)
	{
	case 0:
		return &g_PluginInfo0;
	}
	return NULL;
}

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CTestEditorApp

BEGIN_MESSAGE_MAP(CTestEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CTestEditorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestEditorApp construction

CTestEditorApp::CTestEditorApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTestEditorApp object

CTestEditorApp theApp;

BOOL CTestEditorApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	strcpy(g_PluginInfo0.m_Name,"Script Materializer");
	g_PluginInfo0.m_PluginType		= PluginInfo::PT_EDITOR;
	g_PluginInfo0.m_PluginType		= (PluginInfo::PLUGIN_TYPE)(g_PluginInfo0.m_PluginType | PluginInfo::PTF_RECEIVENOTIFICATION);
	g_PluginInfo0.m_PluginCallback	= PluginCallback;

	return CWinApp::InitInstance();
}

int CTestEditorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::ExitInstance();
}

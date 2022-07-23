#pragma once
#ifndef __MAIN_H
#define __MAIN_H

class CRoboApp : public CWinApp
{
protected:
	HINSTANCE hSpinLib;
public:
	CRoboApp();
	~CRoboApp();
	int Run(void);
	BOOL InitInstance(void);
	int RunOne(void);
// Implementation

	//{{AFX_MSG(CRoboApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

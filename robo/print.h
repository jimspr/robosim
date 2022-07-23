#pragma once
#ifndef __PRINT_H
#define __PRINT_H

#include "resource.h"

class printer_t;

class print_cancel_dlg_t : public CDialog
{
	// Construction
public:
	print_cancel_dlg_t(printer_t& p, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PRINT };

// Implementation
protected:
	printer_t& _printer;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

class printer_t
{
private:
	HWND _hwnd_cancel_dlg = NULL;      // Handle to the cancel print dialog
	HANDLE hInitData{ NULL };   // handle to initialization data

	class print_cancel_dlg_t* _print_can_dlg = nullptr;
	bool _error = false;

public:
	bool _aborted = false;        // TRUE if the user has aborted the print job
	CPrintDialog printDlg;
	std::unique_ptr<CDC> _pdc = nullptr;
	std::string _document_name;  // Global pointer to job title

	printer_t() : printDlg(FALSE) {};

	bool start_job(char* szDocName);
	void end_job();

	friend BOOL FAR PASCAL EXPORT AbortProc(HDC, int);
};

#endif // __PRINT_H


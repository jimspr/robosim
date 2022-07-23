#include "stdafx.h"
#include "resource.h"
#include "print.h"
#include <memory>

using namespace std;

print_cancel_dlg_t::print_cancel_dlg_t(printer_t& p, CWnd* pParent /*=NULL*/) : _printer(p)
{
	VERIFY(Create(print_cancel_dlg_t::IDD, pParent));
}

void print_cancel_dlg_t::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(print_cancel_dlg_t, CDialog)
END_MESSAGE_MAP()

BOOL print_cancel_dlg_t::OnInitDialog()
{
	SetDlgItemText(IDD_PRINTDEVICE, _printer.printDlg.GetDeviceName());
	SetDlgItemText(IDD_PRINTPORT, _printer.printDlg.GetPortName());
	SetDlgItemText(IDD_PRINTTITLE, _printer._document_name.c_str());
	return TRUE;
}

void print_cancel_dlg_t::OnOK()
{
	_printer._aborted = true;
}

// Prepare the printer DC and open the Cancel dialog, ready for printing.
// The application's frame is disabled, mostly to protect the data while
// printing is in progress.
bool printer_t::start_job(char* szDocName)
{
	_aborted = false;
	_error = true; // Assume an error until done.
	_pdc = nullptr;
	_print_can_dlg = NULL;
	
	// Create the job title by loading the title string from STRINGTABLE.
	//
	
	std::string title{ "ROBOSIM II - " };
	title += szDocName;
	_document_name = szDocName;
	
	// Use standard PrintDlg to get printer DC
	//
	//If DoModalPrint returns 0 then user canceled or an error happened.
	if ( printDlg.DoModal() == IDCANCEL)
		return FALSE;

	auto pdc = make_unique<CDC>();
	ASSERT(printDlg.GetPrinterDC() != NULL);
	pdc->Attach(printDlg.GetPrinterDC());

	_print_can_dlg = new print_cancel_dlg_t(*this);
	
	_hwnd_cancel_dlg = _print_can_dlg->m_hWnd;
	
	// Disable the main application window and create the Cancel dialog.
	//
	AfxGetApp()->m_pMainWnd->EnableWindow(FALSE);

	_print_can_dlg->ShowWindow(SW_SHOW);
	_print_can_dlg->UpdateWindow();
	
	// Initialize the document.
	//
	if (pdc->StartDoc(title.c_str()) < 0)
		return false;

	this->_pdc = std::move(pdc);
	
	return true;
}

// end_job:
// Do a final page-eject, shut down the printer context, and re-enable the
// application.
//
void printer_t::end_job()
{
	DWORD dw;
	if (_pdc != nullptr)
	{
		if (_aborted || _pdc->EndDoc() < 0)
		{
			dw = GetLastError();
			_pdc->AbortDoc();
		}
		else
		{
			_error = false;
		}
		
		_pdc.reset();
	}

	if (_print_can_dlg != NULL)
	{
		AfxGetApp()->m_pMainWnd->EnableWindow(TRUE);
		
		_print_can_dlg->DestroyWindow();
		_print_can_dlg = NULL;
		_hwnd_cancel_dlg = NULL;
		
	}
	
	// Error? Make sure the user knows.
	//
	if (_error)
		AfxMessageBox("Printer Error",MB_OK|MB_ICONEXCLAMATION);
}


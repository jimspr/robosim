#pragma once
#ifndef __DIALOGS_H
#define __DIALOGS_H

#include <string>
#include "question.h"

class function;
class node_t;

class spin_button_wnd_t : public CWnd
{
public:
	spin_button_wnd_t() {}
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
	{
		return CWnd::Create("msctls_updown32", "", dwStyle, rect, pParentWnd, nID);
	}
};

class ask_dialog_t : public CDialog
{
public:
	ask_dialog_t(CWnd* p, function* pfn, std::vector<question>& q, const char* t);

	// Dialog Data
		//{{AFX_DATA(CAskDialog)
	enum { IDD = IDD_ASKDIALOG };
	// NOTE: the ClassWizard will add data members here
//}}AFX_DATA

	void OnOK();
	BOOL OnInitDialog();
	node_t* get_list();
	void on_edit_kill_focus(UINT);
	void on_delta_pos(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

	//{{AFX_MSG(CAskDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	const char* _title;
	char _buf[512];
	std::vector<question>& _questions;
	std::vector<CStatic> _prompts;
	std::vector<CEdit> _edits;
	std::vector<spin_button_wnd_t> _spin_buttons;
	CButton _ok;
	CButton _cancel;
	CStatic _statframe;
	CStatic _stattext;
	function* _func;
	CSize get_max_question_extent();
	bool verify();
	bool check_edit(int i);
	bool get_value(int idx, float& val);
	bool get_value(int idx, long& val);
	void set_value(int idx, float val);
	void set_value(int idx, long val);
	bool call_function();
	void fill_in_edit(int i);
	void do_spin(int idx, bool up);
};

#endif

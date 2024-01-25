#include "stdafx.h"
#include <sstream>

#include "resource.h"
#include "dialogs.h"
#include "argstack.h"
#include "package.h"
#include "lispenv.h"
#include "rexcept.h"
#include "node.h"
#include "lisp_engine.h"

#define new DEBUG_NEW

using namespace std;

node_t *question::get_node(void)
{
	switch (type)
	{
	case question_type_e::question_float:
		return new number_node_t(fresult);
	case question_type_e::question_long:
		return new number_node_t(lresult);
		break;
	case question_type_e::question_string:
		return new string_node_t(cresult.c_str());
	case question_type_e::question_symbol:
		return lisp_engine._package.get_symbol(cresult.c_str());
	}
	ASSERT(false);
	return nullptr;
}

BEGIN_MESSAGE_MAP(ask_dialog_t, CDialog)
	ON_WM_CREATE()
	ON_CONTROL_RANGE(EN_KILLFOCUS, 100, 199, on_edit_kill_focus)
	ON_NOTIFY_RANGE(UDN_DELTAPOS, 200, 299, on_delta_pos)
END_MESSAGE_MAP()


ask_dialog_t::ask_dialog_t(CWnd *p, function_t* pfn, std::vector<question>& q,const char *t) :
	CDialog(ask_dialog_t::IDD,p), _questions(q), _prompts(q.size()), _edits(q.size()), _spin_buttons(q.size()), _title(t), _func(pfn)
{
}

int ask_dialog_t::OnCreate(LPCREATESTRUCT lpcs)
{
	return CDialog::OnCreate(lpcs);
}

void ask_dialog_t::on_edit_kill_focus(UINT nID)
{
	int idx = nID-100;
	if (idx >=0 && idx < (int)_questions.size())
		if (_edits[idx].GetModify())
			verify();
}

void ask_dialog_t::on_delta_pos(UINT nID, NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int idx = nID - 200;
	if (idx >= 0 && idx < (int)_questions.size()) // spin button
	{
		do_spin(idx, pNMUpDown->iDelta < 0);
		verify();
	}
	*pResult = 0;
}

void ask_dialog_t::do_spin(int idx, bool up)
{
	switch (_questions[idx].type)
	{
	case question_type_e::question_float: // float
		if (!get_value(idx,_questions[idx].fresult))
			_questions[idx].fresult = 0.F;
		_questions[idx].fresult +=  up ? _questions[idx].incr : -_questions[idx].incr;
		break;
	case question_type_e::question_long: // int
		if (!get_value(idx,_questions[idx].lresult))
			_questions[idx].lresult = 0;
		_questions[idx].lresult += up ? (int)_questions[idx].incr : -(int)_questions[idx].incr;
		break;
	default:
		return;
	}
	fill_in_edit(idx);
}

CSize ask_dialog_t::get_max_question_extent()
{
	CDC dc;
	CSize size(0,0);
	dc.CreateCompatibleDC(NULL);
	dc.SaveDC();
	dc.SelectObject(GetFont());
	for (auto& quest : _questions)
	{
		const char *pt = quest.question_text.c_str();
		CSize s = dc.GetTextExtent(pt,lstrlen(pt));
		if (s.cx > size.cx)
			size.cx = s.cx;
		if (s.cy > size.cy)
			size.cy = s.cy;
	}
	dc.RestoreDC(-1);
	return size;
}

#define VMARGIN 8
#define HMARGIN 12
#define COLSEP 8
#define EHEIGHT 10
#define EWIDTH 64
#define VSPACING 4
#define BUTHEIGHT 14
#define BUTWIDTH 40
#define SFHEIGHT 10
#define STHEIGHT 8

BOOL ask_dialog_t::OnInitDialog(void)
{
	int i, width;
	int xbase = LOWORD(GetDialogBaseUnits());
	int ybase = HIWORD(GetDialogBaseUnits());
	int vmargin = (VMARGIN*ybase)/8;
	int hmargin = (HMARGIN*xbase)/4;
	int colsep = (COLSEP*xbase)/4;
	int eheight = (EHEIGHT*ybase)/8;
	int ewidth = (EWIDTH*xbase)/4;
	int vspacing = (VSPACING*ybase)/8;
	int butheight = (BUTHEIGHT*ybase)/8;
	int butwidth = (BUTWIDTH*xbase)/4;
	int sfheight = (SFHEIGHT*ybase)/8;
	int stheight = (STHEIGHT*ybase)/8;
	CRect rc;
	CDC dc;
	CFont *pf = GetFont();
	
	CSize maxsize = get_max_question_extent();
	CPoint ptl(hmargin,vmargin);
	CPoint ptr(hmargin+maxsize.cx+colsep,vmargin);

	SetWindowText(_title);
	dc.CreateCompatibleDC(NULL);
	dc.SaveDC();
	dc.SelectObject(pf);
	for (i=0;i< (int) _questions.size();i++)
	{
		const char *pt = _questions[i].question_text.c_str();
		CSize s = dc.GetTextExtent(pt,lstrlen(pt));
		int maxheight = (s.cy > eheight) ? s.cy : eheight;
		rc = CRect(ptl+CSize(0,(maxheight-s.cy)/2),CSize(maxsize.cx,s.cy));
		_prompts[i].Create(pt,WS_CHILD|WS_VISIBLE|SS_LEFT,rc,this);
		_prompts[i].SetFont(pf);

		rc = CRect(ptr+CSize(0,(maxheight-eheight)/2),CSize(ewidth,eheight));
		_edits[i].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL,rc,this,100+i);
		_edits[i].SetFont(pf);
		fill_in_edit(i);
		switch (_questions[i].type)
		{
		case question_type_e::question_float:
		case question_type_e::question_long:
			rc = CRect(ptr+CSize(ewidth,(maxheight-eheight)/2),CSize(eheight,eheight));
			_spin_buttons[i].Create(WS_CHILD|WS_VISIBLE|WS_BORDER| UDS_ALIGNRIGHT | UDS_AUTOBUDDY | UDS_ARROWKEYS,rc,this,200+i);
			break;
		}
		
		ptl.y += maxheight+vspacing;
		ptr.y += maxheight+vspacing;
	}
	width = maxsize.cx+colsep+ewidth+eheight;
	if (butwidth+colsep+butheight > width)
		width = 2*butwidth+colsep;
	
	ptl.x += (width/2-colsep/2-butwidth)/2;
	rc = CRect(ptl,CSize(butwidth,butheight));
	_ok.Create("Ok",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_DEFPUSHBUTTON,rc,this,1);
	_ok.SetFont(pf);

	ptl.x += width/2+colsep/2;
	rc = CRect(ptl,CSize(butwidth,butheight));
	_cancel.Create("Cancel",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON,rc,this,2);
	_cancel.SetFont(pf);

	width += 2*hmargin;
	ptl.x = 0;
	ptl.y += butheight+vspacing;

	rc = CRect(ptl,CSize(width-2,sfheight));
	_statframe.Create("",WS_CHILD|WS_VISIBLE|SS_BLACKFRAME,rc,this);
	_statframe.SetFont(pf);
	rc = CRect(ptl+CSize(1,(sfheight-stheight)/2),CSize(width-4,stheight));
	_stattext.Create("",WS_CHILD|WS_VISIBLE|SS_CENTER,rc,this);
	_stattext.SetFont(pf);

	rc = CRect(0,0,width+1,ptl.y+sfheight+1);
	AdjustWindowRect(rc,DS_MODALFRAME | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,FALSE);
	SetWindowPos(NULL,0,0,rc.Width(),rc.Height(),SWP_NOZORDER|SWP_NOMOVE);
	dc.RestoreDC(-1);
	verify();
	return TRUE;
}

void ask_dialog_t::fill_in_edit(int i)
{
	switch (_questions[i].type)
	{
	case question_type_e::question_float:
		_gcvt_s(_buf, _questions[i].fresult,6);
		_edits[i].SetWindowText(_buf);
		break;
	case question_type_e::question_long:
		SetDlgItemInt(100+i,_questions[i].lresult,TRUE);
		break;
	default:
		_edits[i].SetWindowText(_questions[i].cresult.c_str());
		break;
	}
}

void ask_dialog_t::OnOK(void)
{
	if (verify())
		CDialog::OnOK();
}

bool ask_dialog_t::verify(void)
{
	bool bStatus = true;
	_stattext.SetWindowText("");
	size_t numq = _questions.size();
	for (size_t i=0;i<numq;i++)
	{
		_edits[i].SetModify(FALSE);
		if (!check_edit((int)i))
			return false;
	}
	if (_func)
		bStatus = call_function();
	return bStatus;
}

bool ask_dialog_t::check_edit(int i)
{
	int len;
	switch (_questions[i].type)
	{
	case question_type_e::question_float: // float
		if (!get_value(i,_questions[i].fresult))
			return false;
		break;
	case question_type_e::question_long: // int
		if (!get_value(i,_questions[i].lresult))
			return false;
		break;
	case question_type_e::question_string:  //String
		len = _edits[i].GetWindowTextLength();
		_questions[i].cresult.resize(len);
		_edits[i].GetWindowText(&_questions[i].cresult[0],len+1);
		break;
	case question_type_e::question_symbol:  //Symbol
		len = _edits[i].GetWindowTextLength();
		if (len==0)
		{
			wsprintf(_buf,"%s must not be empty",_questions[i].question_text.c_str());
			_stattext.SetWindowText(_buf);
			return false;
		}
		_questions[i].cresult.resize(len);
		_edits[i].GetWindowText(&_questions[i].cresult[0],len+1);
		break;
	}
	return true;
}

bool ask_dialog_t::get_value(int idx,float &val)
{
	char *t;
	double tval;
	int len;
	_edits[idx].GetWindowText(_buf,50);
	len = lstrlen(_buf);
	tval = strtod(_buf,&t);
	if (t != &_buf[len] || len==0)
	{
		wsprintf(_buf,"%s must be a floating point value",_questions[idx].question_text.c_str());
		_stattext.SetWindowText(_buf);
		return false;
	}
	val = (float)tval;
	return true;
}

bool ask_dialog_t::get_value(int idx,long &val)
{
	char *t;
	long tval;
	int len;
	_edits[idx].GetWindowText(_buf,50);
	len = lstrlen(_buf);
	tval = strtol(_buf,&t,10);
	if (t != &_buf[len] || len==0)
	{
		wsprintf(_buf,"%s must be an integer",_questions[idx].question_text.c_str());
		_stattext.SetWindowText(_buf);
		return false;
	}
	val = tval;
	return true;
}

void ask_dialog_t::set_value(int idx,float val)
{
	char buf[30];
	_gcvt_s(buf, (double)val, 6);
	_edits[idx].SetWindowText(buf);
}

void ask_dialog_t::set_value(int idx,long val)
{
	char buf[20];
	_ltoa_s(val, buf, 10);
	_edits[idx].SetWindowText(buf);
}

node_t *ask_dialog_t::get_list(void)
{
	cons_t *base = (cons_t *) nil;
	size_t n = _questions.size();
	while (n--)
		base = new cons_t( _questions[n].get_node() , base);
	return base;
}

bool ask_dialog_t::call_function(void)
{
// push args and call function
	node_t *res = nil;
	frame_stack_state_t state(lisp_engine._frame_stack); /* Restore _frame_stack at end. */
	try
	{
		for (auto& q: _questions)
			lisp_engine._frame_stack.push(q.get_node());
		int numq = (int)_questions.size();
		res = _func->eval(numq, lisp_engine._frame_stack.get_base(numq));
	}
	catch(eval_exception_t* e)
	{
		string str = "Error - ";
		str += get_rlerror_msg(*e);
		AfxMessageBox(str.c_str(), MB_OK|MB_ICONSTOP);
		e->Delete();
	}
	catch(block_return_exception_t* e)
	{
		robosim_exception_t re(UNKNOWN_BLOCK_NAME, lisp_engine._env._readtable._line_cnt);
		ostringstream ostr;
		ostr << "Error - \"";
		e->_block->print(ostr);
		ostr << "\" " << get_rlerror_msg(re) << endl << '\0';;
		AfxMessageBox(ostr.str().c_str(), MB_OK | MB_ICONSTOP);
		e->Delete();
	}
	catch(interrupt_exception_t* e)
	{
		e->Delete();
	}
	catch(read_exception_t* e)
	{
		string str = "Error - ";
		str += get_rlerror_msg(*e);
		AfxMessageBox(str.c_str(), MB_OK|MB_ICONSTOP);
		e->Delete();
	}
	catch(other_exception_t* e)
	{
		AfxMessageBox("Error - Uncaught throw", MB_OK|MB_ICONINFORMATION);
		e->Delete();
	}
	catch(robosim_exception_t* e)
	{
		AfxMessageBox("Error - robosim_exception_t", MB_OK|MB_ICONINFORMATION);
		e->Delete();
	}
	catch (base_exception_t* e)
	{
		AfxMessageBox("Error - base_exception_t Caught", MB_OK | MB_ICONINFORMATION);
		e->Delete();
	}
	catch(CException* e)
	{
		AfxMessageBox("Error - MFC Exception Caught", MB_OK|MB_ICONINFORMATION);
		e->Delete();
	}

	if (res->is_a(TYPE_STRING))
	{
		_stattext.SetWindowText(((string_node_t*)res)->data());
		return false;
	}
	return true;
}

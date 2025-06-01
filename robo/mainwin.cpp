#include "stdafx.h"
#include <memory>
#include <sstream>
#include "resource.h"
#include "lispwin.h"
#include "mainwin.h"
#include "view.h"
#include "print.h"
#include "dialogs.h"
#include "grob.h"
#include "host.h"
#include "lisp_engine.h"

using namespace std;

extern void main_win_mark_in_use();

main_window_t* simulation = nullptr;

simulation_t& get_sim()
{
	ASSERT(simulation != nullptr);
	return *simulation;
}

struct host_t : public lisp_host_t
{
	void periodic_notify() final
	{
		static MSG keymsg;
		if (PeekMessage(&keymsg, NULL, WM_KEYDOWN, WM_KEYDOWN, PM_NOREMOVE))
			if (AfxMessageBox("Do you want to stop?", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDYES)
				throw interrupt_exception_t();
		AfxGetApp()->OnIdle(0); // this makes sure cleanup occurs during long processing
	}
	void mark_in_use() final
	{
		main_win_mark_in_use();
	}
	void add_busy() final
	{
		if (!count)
		{
			hCursor = SetCursor(hHourGlass);
			ShowCursor(TRUE);
		}
		SetCursor(hHourGlass);
		count++;
	}
	virtual void release_busy() final
	{
		count--;
		if (!count)
		{
			ShowCursor(FALSE);
			SetCursor(hCursor);
			hCursor = NULL;
		}
	}
	virtual void exit() final
	{
		// The Lisp interpreter is ready to exit, but the app may be in the message loop.
		// Close the main window to force the exit
		AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
	}

	HCURSOR hCursor{ NULL };
	HCURSOR hHourGlass = LoadCursor(NULL, IDC_WAIT);
	int count = 0;
};

host_t host;

BEGIN_MESSAGE_MAP(main_window_t, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

main_window_t::~main_window_t()
{
	simulation = nullptr;
}

main_window_t::main_window_t()
{
	lisp_engine._host = &host;
	simulation = this;
	VERIFY(LoadAccelTable("ACCEL"));
	_view_wnd = NULL;
	_lisp_window = make_shared<lisp_window_t>();
	string className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
		::LoadCursor(NULL, IDC_CROSS), (HBRUSH)(COLOR_WINDOW + 1),
		LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME))
	);
	Create(className.c_str(), "ROBOSIM II", WS_OVERLAPPEDWINDOW, rectDefault, NULL, MAKEINTRESOURCE(IDR_MAINFRAME));
	_lisp_window->Create(16 /*16 lines*/, this);
	_lisp_window->ShowWindow(SW_SHOW);
	_lisp_window->SetFocus();
	ask_form = NULL;
	m_bAutoMenuEnable = false;
}

int main_window_t::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int nRes = CFrameWnd::OnCreate(lpCreateStruct);
	if (nRes == -1)
		return -1;

	ASSERT(_view_wnd == NULL);
	auto view_wnd = new view_wnd_t(*this);
	_view_wnd = view_wnd;
	if (!view_wnd)
		return -1;
	if (!view_wnd->Create(this))
	{
		delete view_wnd;
		_view_wnd = nullptr;
		return -1;
	}
	view_wnd->ShowWindow(SW_SHOWNA);
	view_wnd->UpdateWindow();

	return 0;
}

BOOL main_window_t::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int idx = LOWORD(wParam);
	switch (idx)
	{
	case IDM_EXIT:
		PostMessage(WM_CLOSE);
		return TRUE;
	case IDM_HELP:
	{
		auto ident = _lisp_window->get_current_ident();
		if (ident.empty() || !isalpha(ident[0]))
			display_help("robo.chm");
		else
			display_help("robo.chm", ident);
		return TRUE;
	}
	}
	if (_menu_funcs.size() == 0)
		return FALSE;
	if (idx >= (int)_menu_funcs.size())
		return CFrameWnd::OnCommand(wParam, lParam);
	busy_t t;

	try
	{
		node_t* res = _menu_funcs[idx]->eval();
	}
	catch (const interrupt_exception_t&)
	{
	}
	catch (const read_exception_t& e)
	{
		string str = "Error - ";
		str += get_rlerror_msg(e, {});
		AfxMessageBox(str.c_str(), MB_OK | MB_ICONSTOP);
	}
	catch (const eval_exception_t& e)
	{
		string str = "Error - ";
		str += get_rlerror_msg(e, {});
		AfxMessageBox(str.c_str(), MB_OK | MB_ICONSTOP);
	}
	catch (block_return_exception_t&)
	{
		AfxMessageBox("Error - Uncaught block_return_exception_t", MB_OK | MB_ICONINFORMATION);
	}
	catch (const lisp_exception_t&)
	{
		AfxMessageBox("Error - Uncaught throw in lisp code", MB_OK | MB_ICONINFORMATION);
	}
	catch (CException* e)
	{
		AfxMessageBox("Error - MFC Exception Caught", MB_OK | MB_ICONINFORMATION);
		e->Delete();
	}
	catch (...)
	{
		assert(false);
		AfxMessageBox("Error - unknown exception type", MB_OK | MB_ICONINFORMATION);
		throw;
	}
	return TRUE;
}

bool main_window_t::copy_to_clibboard()
{
	view_wnd_t* pw = static_cast<view_wnd_t*>(_view_wnd);
	if (!pw)
		return false;
	pw->copy_to_clipboard();
	return true;
}

void main_window_t::mark_in_use()
{
	if (ask_form)
		ask_form->mark_in_use();
	for (auto& node : _menu_funcs)
		node->mark_in_use();
	for (auto& ob : _objects)
		ob->mark_in_use();
}

static void makemenu(CMenu& pm, cons_t* base, int& id, std::vector<node_t*>& parr)
{
	CMenu menu;
	while (base->is_a(TYPE_CONS))
	{
		node_t* sub = base->Car();
		if (sub->is_a(TYPE_CONS))
		{
			auto title = ((cons_t*)sub)->Car()->as<string_node_t>();
			menu.CreatePopupMenu();
			makemenu(menu, ((cons_t*)sub)->CdrCONS(), id, parr);
			pm.AppendMenu(MF_POPUP, (uintptr_t)menu.Detach(), title->data());
		}
		else if (sub == nil)
			pm.AppendMenu(MF_SEPARATOR);
		else
		{
			sub->check_arg_type(TYPE_STRING);
			base = base->CdrCONS();
			base->check_arg_type(TYPE_CONS);
			node_t* res = base->Car();
			pm.AppendMenu(MF_STRING, id++, ((string_node_t*)sub)->data());
			parr.push_back(res);
		}
		base = base->CdrCONS();
	}
}


void main_window_t::set_lisp_menu(cons_t* cons)
{
	int id = 0;
	CMenu menu;
	std::vector<node_t*> pfuncarray;
	menu.CreateMenu();
	makemenu(menu, cons, id, pfuncarray);
	_menu_funcs = std::move(pfuncarray);
	SetMenu(&menu);
	DrawMenuBar();
	menu.Detach();
}

void main_window_t::set_console_height(int lines)
{
	_lisp_window->set_lines(lines);
	RecalcLayout();
}

void main_window_t::maximize_window(bool max)
{
	ShowWindow(max ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
}

void main_window_t::show_about()
{
	CModalDialog about(IDD_ABOUTDIALOG, this);
	about.DoModal();
}

void main_window_t::set_font_dialog()
{
	if (_lisp_window->pick_font())
		RecalcLayout();
}

void main_window_t::new_simulation()
{
	ASSERT(_view_wnd != NULL);
	reset_simulation();
	_view_wnd->invalidate();
}

bool main_window_t::display_help(const string& str)
{
	return ::HtmlHelp(::GetDesktopWindow(), str.c_str(), HH_DISPLAY_TOPIC, 0) ? true : false;
}

bool main_window_t::display_help(const std::string& file, const std::string& topic)
{
	string item = file + "::/html\\" + topic + ".htm";
	return ::HtmlHelp(::GetDesktopWindow(), item.c_str(), HH_DISPLAY_TOPIC, 0) ? true : false;
}

void main_window_t::print()
{
	char szDocName[256];
	int status = 1, cnt = 0;

	GetWindowText(szDocName, sizeof(szDocName));
	view_wnd_t* pw = static_cast<view_wnd_t*>(_view_wnd);
	if (!pw)
		return;

	if (!_the_printer.start_job(szDocName))
		return;

	while (status)
	{
		auto& dc = _the_printer._pdc;
		if (dc->StartPage() < 0 || _the_printer._aborted)
			goto endprint;
		status = pw->paint_printer(*(dc), cnt++);
		if (dc->EndPage() < 0 || _the_printer._aborted)
			goto endprint;
	}

	if (_the_printer._aborted)
		goto endprint;

	_the_printer.end_job();

endprint:

	return;
}

void main_window_t::show_view_dialog()
{
	view_wnd_t* pw = static_cast<view_wnd_t*>(_view_wnd);
	pw->_view_dialog->create();
}

node_t* main_window_t::ask(function_t* pfn, std::vector<question>& pq, const char* title)
{
	node_t* result;
	ask_form = pfn;
	ask_dialog_t askd(this, pfn, pq, title);

	try
	{
		result = (askd.DoModal() == IDOK) ? askd.get_list() : nil;
		ask_form = NULL;
	}
	catch (const base_exception_t&) // TODO - handle specific exceptions
	{
		ask_form = NULL;
		throw;
	}
	catch (CException*)
	{
		ask_form = NULL;
		throw;
	}
	ask_form = NULL;
	return result;
}

void main_window_t::OnSetFocus(CWnd* pOldWnd)
{
	if (_lisp_window != NULL)
		_lisp_window->SetFocus();
}

void main_window_t::OnDestroy()
{
	CFrameWnd::OnDestroy();
	delete _view_wnd;
}

#pragma once
#ifndef __MAINWIN_H
#define __MAINWIN_H

#include <memory>
#include <string>
#include "print.h"
#include "simula.h"

class lisp_window_t;
class question;

class main_window_t : public CFrameWnd, public simulation_t
{
protected:
	node_t* ask_form = nullptr;
	std::vector<node_t*> _menu_funcs;
public:
	std::shared_ptr<lisp_window_t> _lisp_window;
	printer_t _the_printer;

	main_window_t();
	~main_window_t();

	bool copy_to_clibboard() override;
	void mark_in_use(void) override;
	void new_simulation(void) override;
	bool display_help(const std::string& file) override;
	bool display_help(const std::string& file, const std::string& topic) override;
	void print() override;
	void set_font_dialog() override;
	void show_about(void) override;
	void show_view_dialog() override;
	void set_lisp_menu(cons_t* cons) override;
	void set_console_height(int lines) override;
	void maximize_window(bool max) override;
	node_t* ask(function* pfn, std::vector<question>& pq, const char* title) override;

	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void RecalcLayout(BOOL bNotify = TRUE) override
	{
		CFrameWnd::RecalcLayout(bNotify);
	}

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

#endif

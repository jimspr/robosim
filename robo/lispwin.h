#pragma once
#ifndef __LISPWIN_H__
#define __LISPWIN_H__

#include <streambuf>
#include "ttywin.h"

class lisp_window_t : public tty_window_t, public std::streambuf
{
public:
	bool _is_full = false;
	void messageloop(void);

	char _input[KEYBUFSIZE];
	char _output[KEYBUFSIZE];
	lisp_window_t(void);
	~lisp_window_t();

	int out_waiting();

	virtual void process();
	virtual int overflow(int = EOF);
	virtual int underflow();
	virtual int sync();

	afx_msg int OnCreate(LPCREATESTRUCT lpCS);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	std::streambuf* old_cin = nullptr;
	std::streambuf* old_cout = nullptr;
	std::streambuf* old_cerr = nullptr;
};

#endif

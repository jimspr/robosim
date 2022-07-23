#pragma once
#ifndef __TTYWIN_H
#define __TTYWIN_H

#include "wstdio.h"

#define KEYBUFSIZE 1000

class tty_window_t : public stdio_window_t
{
protected:
	char _active[KEYBUFSIZE + 1]{};
	int _active_pos = 0; // current position in _active (there may be _num_left_chars to the right)

	static constexpr size_t max_history_size = 30; // Maximum history items to maintain
	std::vector<std::string> _history = { "" };
	int _cur_history_item = 1; // Tracks current history item as you scroll through.
public:

	void stop();
	void delete_char();
	void add_char(char ch);
	virtual void process();
	int get_buffer(char* probuf);
	int get_buffer_len();

	void add_line_to_history();
	void add_line_from_history(int index);
	void erase_current_line();
	bool cursor_right();
	bool cursor_left();
	bool cursor_end();
	bool cursor_home();

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
};

#endif

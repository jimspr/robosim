#pragma once
#ifndef __WSTDIO_H
#define __WSTDIO_H

#include <vector>
#include <string>

class stdio_window_t : public CWnd
{
private:
	static HCURSOR _cursor_handle;
	static std::string _window_class_name;

protected:
	static constexpr int _max_buffer_size = 32; // Remember last 32 lines so if buffer is resized can show more.
	int _chars_per_line = 0; // Maximum number of chars that will fit on line given current width of window and font.

	std::vector<std::string> _buffer{ "" }; //Start with one line
	int _cur_pos = 0;		// Current cursor position of current line.
	int _num_left_char = 0; // Number of chars to right of caret on current line.

	int _lines = 8;		// Number of displayable lines.

	CFont _font;
	int _char_width;// width and height of Stdio font chars
	int _char_height;
	int _char_descent;
	DWORD _bk_color;			  // background color
	DWORD _text_color;			  // text color

	int lines_per_string(const std::string& str) const;
	int line_of_string_for_position(int pos) const;

	void new_line();
	void back_space(CDC& dc);
	void put_char(CDC& dc, char ch);
	void init_font();
	int paint_line(CDC& dc, int line, const std::string& str);

	void move_right(int c);
	void move_left(int c);

public:

	stdio_window_t();

	void setup_stdio_dc(CDC& dc);
	void put_stdio(CDC& dc, int len, const char* str);
	void set_lines(int lines);
	int get_line_count() { return _lines; }
	void convert_point_log_to_dev(CPoint& cpt);
	void convert_point_dev_to_log(CPoint& cpt);
	void convert_rect_log_to_dev(CRect& rc);
	void convert_rect_dev_to_log(CRect& rc);
	void puts(const char* str, int len = -1);
	void putc(char ch);
	void set_caret();
	void set_caret(CDC& dc);
	int pick_font();
	void verify_caret();

	BOOL Create(int lines, CWnd* pParentWnd);

	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSysColorChange();
	afx_msg void OnPaint();	  // repaint the text in the Stdio
	afx_msg void OnSetFocus(CWnd*);
	afx_msg void OnKillFocus(CWnd*);
	afx_msg void OnSize(UINT nType, int x, int y);
	DECLARE_MESSAGE_MAP()
};

#endif

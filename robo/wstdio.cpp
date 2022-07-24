#include "stdafx.h"
#include "resource.h"
#include <memory>
#include "wstdio.h"
#include "dialogs.h"
#define new DEBUG_NEW

using namespace std;

HCURSOR stdio_window_t::_cursor_handle;
string stdio_window_t::_window_class_name;

BEGIN_MESSAGE_MAP(stdio_window_t, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

stdio_window_t::stdio_window_t() : CWnd()
{
	_font.CreateFont(0, 0, 0, 0, 400, FALSE, FALSE, FALSE, 0,
		0, 0, 0, FIXED_PITCH | FF_DONTCARE, NULL);
	init_font();
}

BOOL stdio_window_t::Create(int h, CWnd* pParentWnd)
{
	_lines = h;
	if (_window_class_name.empty())
	{
		_cursor_handle = LoadCursor(NULL, IDC_ARROW);
		_window_class_name = AfxRegisterWndClass(0, _cursor_handle,
			(HBRUSH)(COLOR_BTNFACE + 1), 0);
	}

	return CWnd::CreateEx(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE, _window_class_name.c_str(), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 100, 100, pParentWnd->GetSafeHwnd(), (HMENU)(AFX_IDW_PANE_FIRST + 1));
}

int stdio_window_t::OnCreate(LPCREATESTRUCT lpcs)
{
	set_lines(_lines);
	return CWnd::OnCreate(lpcs);
}

void stdio_window_t::setup_stdio_dc(CDC& dc)
{
	dc.SetBkMode(TRANSPARENT);
	dc.SaveDC();
	dc.SelectObject(&_font);
}

void stdio_window_t::new_line()
{
	_buffer.emplace_back();
	if ((int)_buffer.size() > _max_buffer_size)
		_buffer.erase(_buffer.begin());
	auto& line_buffer = _buffer.back();
	CRect rc(0, 0, _chars_per_line, _lines);
	convert_rect_log_to_dev(rc);
	InvalidateRect(rc, TRUE);
}

void stdio_window_t::back_space(CDC& dc)
{
	auto& line_buffer = _buffer.back(); // pointer to current line
	if (_cur_pos > 0)
	{
		--_cur_pos;
		line_buffer.erase(line_buffer.begin() + _cur_pos);
//		CRect rc(_cur_pos, max_line, _cur_pos + _num_left_char + 1, max_lines);
//		convert_rect_log_to_dev(rc);
		CRect rc;
		GetClientRect(&rc);
		InvalidateRect(rc, TRUE);
	}
	else
		MessageBeep(MB_ICONEXCLAMATION);
}

void stdio_window_t::put_char(CDC& dc, char ch)
{
	// put the character in the screen buffer
	auto& line_buffer = _buffer.back(); // pointer to current line
	line_buffer.insert(line_buffer.begin() + _cur_pos, ch);
	++_cur_pos;

	// calculate area to invalidate

	// only need to invalidate the area if it is on the last line
	CRect rc;
	GetClientRect(&rc);
	InvalidateRect(rc, TRUE);
}

void stdio_window_t::put_stdio(CDC& dc, const char* str, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		switch (str[i])
		{
		case '\n':
			new_line();
			_cur_pos = 0;
			break;
		case '\r':
			_cur_pos = 0;    // reset the current position in the line
			break;
		case '\b': // backspace
			back_space(dc);
			break;
		case '\t':
			put_stdio(dc, "    ", 4);
			break;
		default:
			//add char to buffer
			put_char(dc, str[i]);
			break;
		}
	}

	set_caret(dc);
}

//========================================================================
// initialize variables used by stdio

void stdio_window_t::set_caret()
{
	CClientDC dc(this);
	setup_stdio_dc(dc);
	set_caret(dc);
	dc.RestoreDC(-1);
}

int stdio_window_t::lines_per_string(const string& str) const
{
	// Alwasy at least one line.
	if (str.empty())
		return 1;
	return ((int)str.size() + _chars_per_line - 1) / _chars_per_line;
}

/* Returns line offset for given position in string. */
int stdio_window_t::line_of_string_for_position(int pos) const
{
	if (pos == 0)
		return 1;
	return (pos + _chars_per_line - 1) / _chars_per_line;
}

void stdio_window_t::set_caret(CDC& dc)
{
	int column = _cur_pos % _chars_per_line;
	auto& str = _buffer.back();

	CPoint pt(column, _lines - lines_per_string(str) + line_of_string_for_position(_cur_pos) - 1);
	convert_point_log_to_dev(pt);
	SetCaretPos(pt);
}

void stdio_window_t::init_font()
{
	// get the text metrics for the font we are using
	TEXTMETRIC Metrics;
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	dc.SaveDC();
	dc.SelectObject(&_font);
	dc.GetTextMetrics(&Metrics);
	dc.RestoreDC(-1);

	// calculate the height and width of the font
	_char_width = Metrics.tmAveCharWidth;
	_char_height = Metrics.tmHeight + Metrics.tmExternalLeading;
	_char_descent = Metrics.tmDescent;

	// get the background and forground colors we are going to use
	_bk_color = GetSysColor(COLOR_WINDOW); // background color
	_text_color = GetSysColor(COLOR_WINDOWTEXT); // text color
}

void stdio_window_t::puts(const char* str, size_t len)
{
	CClientDC dc(this);

	setup_stdio_dc(dc);
	put_stdio(dc, str, len);
	dc.RestoreDC(-1);
}

void stdio_window_t::putc(char ch)
{
	CClientDC dc(this);
	setup_stdio_dc(dc);
	put_stdio(dc, &ch, 1);
	dc.RestoreDC(-1);
}

void stdio_window_t::OnSysColorChange()
{
	_bk_color = GetSysColor(COLOR_WINDOW); // background color
	_text_color = GetSysColor(COLOR_WINDOWTEXT); // text color
	CWnd::OnSysColorChange();
}

void stdio_window_t::set_lines(int lines)
{
	_lines = lines;
	if (_lines == 0)
		ShowWindow(SW_HIDE);
	else
		ShowWindow(SW_SHOWNORMAL);
	GetParentFrame()->RecalcLayout();
	Invalidate(TRUE);
}

int stdio_window_t::paint_line(CDC& dc, int line, const std::string& str)
{
	if (str.empty())
		return 1;
	// Print partial line
	int lines_to_paint = ((int)str.size() + _chars_per_line - 1)/ _chars_per_line;
	ASSERT(lines_to_paint > 0);
	auto start = str.begin();
	for (int i = 1; i <= lines_to_paint; ++i)
	{
		// Print at most _chars_per_line.
		CPoint pt(0, line - lines_to_paint + i);
		convert_point_log_to_dev(pt);
		auto end = str.end();
		if ((end - start) > _chars_per_line)
			end = start + _chars_per_line;
		dc.TextOut(pt.x, pt.y, &*start, (int)(end - start));
		start = end;
	}
	return lines_to_paint;
}

afx_msg void stdio_window_t::OnPaint()     // repaint the text in the Stdio
{
	CRect rcUpdate;
	CPaintDC dc(this);

	setup_stdio_dc(dc);

	rcUpdate = dc.m_ps.rcPaint;
	convert_rect_dev_to_log(rcUpdate);

	// _chars_per_line - max chars per line
	// rcUpdate.bottom - number of lines that can fit

	// print lines in reverse order
	int line = _lines - 1;
	for (auto iter = _buffer.rbegin(); iter < _buffer.rend(); ++iter)
	{
		if (line < 0)
			break;
		CPoint pt(0, line);
		convert_point_log_to_dev(pt);
		line -= paint_line(dc, line, *iter);
	}

	dc.RestoreDC(-1);
}

void stdio_window_t::OnSetFocus(CWnd* pWnd)
{
	CWnd::OnSetFocus(pWnd);
	CreateSolidCaret(0, _char_height);
	ShowCaret();
	set_caret();
}

void stdio_window_t::OnKillFocus(CWnd* pWnd)
{
	HideCaret();
	::DestroyCaret();
	CWnd::OnKillFocus(pWnd);
}

void stdio_window_t::OnSize(UINT nType, int x, int y)
{
	HideCaret();
	CWnd::OnSize(nType, x, y);
	_chars_per_line = x / _char_width;
	set_caret();
	Invalidate(TRUE);
	ShowCaret();
}

int stdio_window_t::pick_font()
{
	CFontDialog fd((LPLOGFONT)NULL, CF_FIXEDPITCHONLY | CF_SCREENFONTS | CF_FORCEFONTEXIST);
	if (fd.DoModal() == IDCANCEL)
		return 0;
	_font.DeleteObject();
	LOGFONT* p = &fd.m_lf;
	_font.CreateFontIndirect(p);
	init_font();
	CRect rc;
	GetClientRect(&rc);
	_chars_per_line = rc.Width() / _char_width;

	set_lines(_lines);
	verify_caret();
	return 1;
}

void stdio_window_t::verify_caret()
{
	if (GetFocus() == this)
	{
		HideCaret();
		::DestroyCaret();
		CreateSolidCaret(0, _char_height);
		set_caret();
		ShowCaret();
	}
}

void stdio_window_t::convert_point_log_to_dev(CPoint& cpt)
{
	CRect rc;
	GetClientRect(rc);
	cpt.x = cpt.x * _char_width;
	cpt.y = rc.bottom - (_lines - cpt.y) * _char_height;
}

void stdio_window_t::convert_point_dev_to_log(CPoint& cpt)
{
	CRect rc;
	GetClientRect(rc);
	cpt.x = cpt.x / _char_width;
	cpt.y = (cpt.y - rc.bottom) / _char_height + _lines;
}

void stdio_window_t::convert_rect_log_to_dev(CRect& rc)
{
	convert_point_log_to_dev(rc.TopLeft());
	convert_point_log_to_dev(rc.BottomRight());
}

void stdio_window_t::convert_rect_dev_to_log(CRect& rc)
{
	convert_point_dev_to_log(rc.TopLeft());
	convert_point_dev_to_log(rc.BottomRight());
	rc.BottomRight().Offset(1, 1);
}

void stdio_window_t::move_left(int c)
{
	CClientDC dc(this);
	setup_stdio_dc(dc);
	ASSERT(_cur_pos >= c);
	_cur_pos -= c;
	_num_left_char += c;
	set_caret(dc);
	dc.RestoreDC(-1);
}

void stdio_window_t::move_right(int c)
{
	CClientDC dc(this);
	setup_stdio_dc(dc);
	ASSERT(_num_left_char >= c);
	_cur_pos += c;
	_num_left_char -= c;
	set_caret(dc);
	dc.RestoreDC(-1);
}

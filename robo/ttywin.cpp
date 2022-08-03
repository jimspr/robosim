#include "stdafx.h"
#include "ttywin.h"
#include "lispenv.h"
#define new DEBUG_NEW

BEGIN_MESSAGE_MAP(tty_window_t, stdio_window_t)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void tty_window_t::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ASSERT(_active[KEYBUFSIZE] == (char)0);
	switch (nChar)
	{
	case VK_ESCAPE:
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	case VK_BACK:
		delete_char();
		break;
	case VK_RETURN:
		_active_pos = _active_pos + _num_left_char;
		_num_left_char = 0;

		if (_active_pos == KEYBUFSIZE)
		{
			MessageBeep(MB_ICONEXCLAMATION);
			return;
		}

		add_line_to_history();
		add_char('\n');
		process();
		break;
	default:
		add_char((char)nChar);
	}
}

void tty_window_t::add_line_to_history()
{
	if (_active_pos == 0)
		return;
	// Remove item from history if too big.
	if (_history.size() > max_history_size)
		_history.erase(_history.begin());
	// Terminate string in _active
	_active[_active_pos] = 0;
	_history.push_back(std::string{ _active });
	_cur_history_item = (int)_history.size();
}

void tty_window_t::delete_char()
{
	int i;
	if (_active_pos && _cur_pos)
	{
		_active_pos--;
		putc(VK_BACK);
		for (i = 0; i <= _num_left_char; i++)
			*(_active + _active_pos + i) = *(_active + _active_pos + i + 1);

	}
	else
		MessageBeep(MB_ICONEXCLAMATION);
}

void tty_window_t::add_char(char ch)
{
	int i;
	for (i = _num_left_char; i > 0; i--)
		_active[_active_pos + i] = _active[_active_pos + i - 1];

	_active[_active_pos++] = ch;
	if (m_hWnd != NULL)
		putc(ch);
}

/* Put an EOF in. */
void tty_window_t::stop()
{
	/* Get rid of any characters remaining. */
	_active_pos = 0;

	_active[_active_pos++] = EOF;
	process();
}

int tty_window_t::get_buffer(char* probuf)
{
	_active[_active_pos] = 0;
	lstrcpy(probuf, _active);
	return lstrlen(probuf);
}

int tty_window_t::get_buffer_len()
{
	_active[_active_pos] = 0;
	return _active_pos;
}

void tty_window_t::process()
{
	_active_pos = 0;
}

void tty_window_t::add_line_from_history(int index)
{
	for (auto ch : _history[index])
		add_char(ch);
}

void tty_window_t::erase_current_line()
{
	cursor_end();
	for (int j = 0; j = _active_pos; j++)
		delete_char();
}

bool tty_window_t::cursor_left()
{
	if (_active_pos && _cur_pos)
	{
		move_left(1);
		_active_pos--;
		return true;
	}
	return false;
}

bool tty_window_t::cursor_right()
{
	if (_num_left_char > 0)
	{
		move_right(1);
		_active_pos++;
		return true;
	}
	return false;
}

bool tty_window_t::cursor_end()
{
	if (_num_left_char > 0)
	{
		int cnt = _num_left_char;
		move_right(cnt);
		_active_pos += cnt;
		return true;
	}
	return false;
}

bool tty_window_t::cursor_home()
{
	if (_active_pos && _cur_pos)
	{
		move_left(_active_pos);
		_active_pos = 0;
		return true;
	}
	return false;
}

void tty_window_t::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_ESCAPE:
		erase_current_line();
		break;
	case VK_END:
		if (!cursor_end())
			MessageBeep(MB_ICONEXCLAMATION);
		break;
	case VK_HOME:
		if (!cursor_home())
			MessageBeep(MB_ICONEXCLAMATION);
		break;
	case VK_LEFT:
		if (!cursor_left())
			MessageBeep(MB_ICONEXCLAMATION);
		break;
	case VK_RIGHT:
		if (!cursor_right())
			MessageBeep(MB_ICONEXCLAMATION);
		break;
	case VK_UP:
		erase_current_line();
		if (--_cur_history_item == -1)
			_cur_history_item = (int)_history.size() - 1;
		add_line_from_history(_cur_history_item);
		break;
	case VK_DOWN:
		erase_current_line();
		if (++_cur_history_item >= (int)_history.size())
			_cur_history_item = 0;
		add_line_from_history(_cur_history_item);
		break;
	case VK_DELETE:
		if (_num_left_char <= 0)
		{
			MessageBeep(MB_ICONEXCLAMATION);
			return;
		}
		else if (!cursor_right())
		{
			MessageBeep(MB_ICONEXCLAMATION);
			return;
		}
		delete_char();
		break;
	}
	stdio_window_t::OnKeyDown(nChar, nRepCnt, nFlags);
}

std::string tty_window_t::get_current_ident()
{
	int beg = _active_pos;
	int end = _active_pos;
	/* Look backwards and forwards to find identifier. */
	auto type = lisp_env._readtable._type;
	while ((beg > 0) && (type[_active[beg - 1]] == CONSTITUENT))
		--beg;
	while (type[_active[end]] == CONSTITUENT)
		++end;
	return std::string{ &_active[beg], &_active[end] };
}

#include "stdafx.h"
#include "afxpriv.h"
#include "lispwin.h"
#include "robo.h"

BEGIN_MESSAGE_MAP(lisp_window_t, tty_window_t)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
END_MESSAGE_MAP()

lisp_window_t::lisp_window_t(void) : tty_window_t(), std::streambuf()
{
	setg(_input, _input + KEYBUFSIZE, _input + KEYBUFSIZE);
	setp(_output, _output + KEYBUFSIZE);

	old_cin = std::cin.rdbuf(this);
	old_cout = std::cout.rdbuf(this);
	old_cerr = std::cerr.rdbuf(this);
}

lisp_window_t::~lisp_window_t()
{
	std::cin.rdbuf(old_cin);
	std::cout.rdbuf(old_cout);
	std::cerr.rdbuf(old_cerr);
}

int lisp_window_t::OnCreate(LPCREATESTRUCT lpCS)
{
	if (tty_window_t::OnCreate(lpCS))
		return -1;
	SetFocus();
	return 0;
}

LRESULT lisp_window_t::OnSizeParent(WPARAM, LPARAM lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

	// align the control bar
	CRect rect;
	rect.CopyRect(&lpLayout->rect);

	CSize sizeAvail = rect.Size();  // maximum size available

	CSize size(32000, _lines * _char_height);
	if (_lines != 0)
	{
		size.cy += _char_descent;
		CRect rectAdjust(CPoint(0,0), size);
		CalcWindowRect(&rectAdjust);
		size = rectAdjust.Size();
	}

	size.cx = min(size.cx, sizeAvail.cx);
	size.cy = min(size.cy, sizeAvail.cy);

	lpLayout->sizeTotal.cy += size.cy;
	lpLayout->sizeTotal.cx = max(lpLayout->sizeTotal.cx, size.cx);
	rect.top = rect.bottom - size.cy;
	lpLayout->rect.bottom -= size.cy;

	rect.right = rect.left + size.cx;
	rect.bottom = rect.top + size.cy;

	// only resize the window if doing layout and not just rect query
	if (lpLayout->hDWP != NULL)
		AfxRepositionWindow(lpLayout, m_hWnd, &rect);
	return 0;
}

void lisp_window_t::process(void)
{
	int len = get_buffer_len();
	setg(eback(), egptr() - len, egptr());
	get_buffer(gptr());
	tty_window_t::process();
	_is_full = true;
}

int lisp_window_t::out_waiting()
{
	return pptr() - pbase();
}

int lisp_window_t::overflow(int ch)
{
	if (out_waiting())
	{
		if (m_hWnd != NULL)
			puts(pbase(), out_waiting());
		setp(pbase(), epptr());
	}
	if ((ch != EOF) && (m_hWnd != NULL))
		putc(ch);
	return 0;
}

int lisp_window_t::underflow()
{
	sync();
	if (!in_avail())
		messageloop();
	return (int)(gptr()[0]);
}

int lisp_window_t::sync()
{
	overflow(EOF);
	return 0;
}

void lisp_window_t::messageloop(void)
{
	_is_full = false;
	while (!_is_full)
		((CRoboApp *)afxCurrentWinApp)->RunOne();
}

void lisp_window_t::OnLButtonDown(UINT flags,CPoint pt)
{
	CWnd::OnLButtonDown(flags,pt);
	SetFocus();
}

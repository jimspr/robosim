/******************************************************************************\
*
*  MODULE:      SPIN.C
*
*  FUNCTIONS:   DLLMain()            - Registers spin class when a
*                                      process loads this DLL.
*               SpinWndProc()    - Window procedure for spin
*                                      control.
*
\******************************************************************************/

#define STRICT
#include <windows.h>

/******************************************************************************\
*                               SYMBOLIC CONSTANTS
\******************************************************************************/

#define SPINCLASS        "SpinButton"
#define SPINDESCRIPTION  "Spin Button"

#define SPIN_EXTRA       sizeof(intptr_t)   // number of extra bytes for spin class

#define SPIN_TIMER1           1
#define SPIN_TIMER2           2

#define GWL_SPINDATA     0 // offset of control's instance data

/******************************************************************************\
*                                    TYPEDEFs
\******************************************************************************/
#define SPIN_CLEAR 0
#define SPIN_UP 1
#define SPIN_DOWN 2

typedef struct
{
	int buttonstate;
	int timerstate;
} SPININFO, * PSPININFO;

/******************************************************************************\
*                                    GLOBALs
\******************************************************************************/

HBRUSH hFace;
HBRUSH hTextBrush;
HPEN hShadowPen;
HPEN hTextPen;
HPEN hHighPen;




LRESULT CALLBACK SpinWndProc(HWND, UINT, WPARAM, LPARAM);
void Paint(HWND);
int GetButton(HWND hWnd, LPARAM lp);


BOOL ProcAttach(HANDLE hDLL)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC | CS_GLOBALCLASS;
	wc.lpfnWndProc = (WNDPROC)SpinWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = SPIN_EXTRA;
	wc.hInstance = (HINSTANCE)hDLL;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = (LPSTR)NULL;
	wc.lpszClassName = (LPSTR)SPINCLASS;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, (LPCTSTR)"DLLEntryPoint(): RegisterClass() failed",
			(LPCTSTR)"Err! - SPIN.DLL",
			MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	hFace = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	hTextBrush = CreateSolidBrush(GetSysColor(COLOR_BTNTEXT));
	hShadowPen = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));
	hTextPen = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNTEXT));
	hHighPen = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNHIGHLIGHT));
	return TRUE;
}

BOOL ProcDetach()
{
	DeleteObject(hFace);
	DeleteObject(hTextBrush);
	DeleteObject(hShadowPen);
	DeleteObject(hTextPen);
	DeleteObject(hHighPen);
	return TRUE;
}

/******************************************************************************\
*
*  FUNCTION:    DllMain
*
*  INPUTS:      hDLL       - DLL module handle
*               dwReason   - reason being called (e.g. process attaching)
*               lpReserved - reserved
*
*  RETURNS:     (Only used when dwReason == DLL_PROCESS_ATTACH)
*               TRUE if initialization passed, or
*               FALSE if initialization failed.
*
*  COMMENTS:    On DLL_PROCESS_ATTACH registers the SPINCLASS
*
\******************************************************************************/

BOOL WINAPI DllMain(HANDLE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		return ProcAttach(hDLL);
	else if (dwReason == DLL_PROCESS_DETACH)
		return ProcDetach();
	return TRUE;
}


/******************************************************************************\
*
*  FUNCTION:    SpinWndProc (standard window procedure INPUTS/RETURNS)
*
*  COMMENTS:    This is the window procedure for our custom control. At
*               creation we alloc a SPININFO struct, initialize it,
*               and associate it with this particular control. We also
*               start a timer which will invalidate the window every so
*               often; this causes a repaint, and the gets drawn in
*               a new position. Left button clicks will toggle the motion
*               state of the control (and turn the timer on/off).
*
\******************************************************************************/

int GetButton(HWND hWnd, LPARAM lp)
{
	int y;
	RECT rc;
	y = HIWORD(lp);
	GetClientRect(hWnd, &rc);
	return (y > rc.bottom / 2) ? SPIN_DOWN : SPIN_UP;
}

LRESULT CALLBACK SpinWndProc(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
	PSPININFO pSCI = (PSPININFO)GetWindowLongPtr(hWnd, GWL_SPINDATA);
	switch (msg)
	{
	case WM_CREATE:
		pSCI = (PSPININFO)LocalAlloc(LPTR, sizeof(SPININFO));
		pSCI->buttonstate = SPIN_CLEAR;
		pSCI->timerstate = 0;
		SetWindowLongPtr(hWnd, GWL_SPINDATA, (intptr_t)pSCI);
		break;
	case WM_PAINT:
		Paint(hWnd);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case SPIN_TIMER1:
			KillTimer(hWnd, SPIN_TIMER1);
			SetTimer(hWnd, SPIN_TIMER2, 100, NULL);
			pSCI->timerstate = SPIN_TIMER2;
		case SPIN_TIMER2:
			PostMessage(GetParent(hWnd),
				WM_COMMAND,
				MAKELONG(GetDlgCtrlID(hWnd), (pSCI->buttonstate == SPIN_UP) ? BN_CLICKED : BN_DOUBLECLICKED),
				(LPARAM)hWnd);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		pSCI->buttonstate = GetButton(hWnd, lParam);
		SetFocus(hWnd);
		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
		PostMessage(GetParent(hWnd), WM_COMMAND,
			MAKELONG(GetDlgCtrlID(hWnd), (pSCI->buttonstate == SPIN_UP) ? BN_CLICKED : BN_DOUBLECLICKED),
			(LPARAM)hWnd);
		SetTimer(hWnd, SPIN_TIMER1, 250, NULL);
		pSCI->timerstate = SPIN_TIMER1;
		SetCapture(hWnd);
		break;
	case WM_LBUTTONUP:
		if (pSCI->buttonstate)
		{
			pSCI->buttonstate = SPIN_CLEAR;
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
			ReleaseCapture();
			KillTimer(hWnd, pSCI->timerstate);
			pSCI->timerstate = 0;
		}
		break;
	case WM_DESTROY:
		LocalFree(pSCI);
		break;
	default:
		return (DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return ((LONG)TRUE);
}

/******************************************************************************\
*
*  FUNCTION:     Paint
*
*  INPUTS:       hWnd - Handle of the window to draw into.
*
\******************************************************************************/

void DrawButton(HDC hDC, int x, int y, int nWidth, int nHeight, int nDir, BOOL bPress)
{
	POINT pts[3];
	int w, h;

	SaveDC(hDC);
	SetWindowOrgEx(hDC, -x, -y, NULL);
	SelectObject(hDC, hFace);
	SelectObject(hDC, hShadowPen);

	Rectangle(hDC, 0, 0, nWidth, nHeight);
	if (!bPress)
	{
		MoveToEx(hDC, 1, nHeight - 1, NULL);
		LineTo(hDC, nWidth - 2, nHeight - 1);
		LineTo(hDC, nWidth - 2, 1);
	}
	if (!bPress)
		SelectObject(hDC, hHighPen);
	MoveToEx(hDC, 1, nHeight - 2, NULL);
	LineTo(hDC, 1, 1);
	LineTo(hDC, nWidth - 1, 1);
	w = nWidth - 6;
	h = nHeight - 5;
	w = (w + 1) / 2;
	h = (h > w) ? w : h;
	w = 2 * h - 1;
	SetWindowOrgEx(hDC,
		-(x + (nWidth - w) / 2 + ((bPress) ? 1 : 0)),
		-(y + (nHeight - h) / 2 + ((bPress) ? 2 : 1)),
		NULL);
	if (nDir == 1)
	{
		pts[0].x = 0;
		pts[0].y = h - 1;
		pts[1].x = h - 1;
		pts[1].y = 0;
		pts[2].x = w - 1;
		pts[2].y = h - 1;
	}
	else
	{
		pts[0].x = 0;
		pts[0].y = 0;
		pts[1].x = h - 1;
		pts[1].y = h - 1;
		pts[2].x = w - 1;
		pts[2].y = 0;
	}
	SelectObject(hDC, hTextBrush);
	SelectObject(hDC, hTextPen);
	Polygon(hDC, pts, 3);
	RestoreDC(hDC, -1);
}

void Paint(HWND hWnd)
{
	PSPININFO  pSCI;
	RECT           rect;
	PAINTSTRUCT    ps;
	int nHeight, nWidth;

	pSCI = (PSPININFO)GetWindowLongPtr(hWnd, GWL_SPINDATA);
	GetClientRect(hWnd, &rect);

	BeginPaint(hWnd, &ps);

	nHeight = rect.bottom - rect.top;
	nWidth = rect.right - rect.left;
	nHeight -= nHeight % 2;

	SaveDC(ps.hdc);
	DrawButton(ps.hdc, 0, 0, nWidth, nHeight / 2, 1, (pSCI->buttonstate == SPIN_UP));
	DrawButton(ps.hdc, 0, nHeight / 2, nWidth, nHeight / 2, 0, (pSCI->buttonstate == SPIN_DOWN));
	RestoreDC(ps.hdc, -1);

	EndPaint(hWnd, &ps);
}

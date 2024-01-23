#include "stdafx.h"
#include <algorithm>
#include <afxdlgs.h>

#include "resource.h"
#include "view.h"
#include "mainwin.h"
#include "grob.h"
#include "lightpage.h"

using namespace std;

unsigned char threeto8[8] =
{
	0, 0111 >> 1, 0222 >> 1, 0333 >> 1, 0444 >> 1, 0555 >> 1, 0666 >> 1, 0377
};

unsigned char twoto8[4] =
{
	0, 0x55, 0xaa, 0xff
};

unsigned char oneto8[2] =
{
	0, 255
};

static int defaultOverride[13] =
{
	0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] =
{
	{ 0,   0,   0,    0 },
	{ 0x80,0,   0,    0 },
	{ 0,   0x80,0,    0 },
	{ 0x80,0x80,0,    0 },
	{ 0,   0,   0x80, 0 },
	{ 0x80,0,   0x80, 0 },
	{ 0,   0x80,0x80, 0 },
	{ 0xC0,0xC0,0xC0, 0 },

	{ 192, 220, 192,  0 },
	{ 166, 202, 240,  0 },
	{ 255, 251, 240,  0 },
	{ 160, 160, 164,  0 },

	{ 0x80,0x80,0x80, 0 },
	{ 0xFF,0,   0,    0 },
	{ 0,   0xFF,0,    0 },
	{ 0xFF,0xFF,0,    0 },
	{ 0,   0,   0xFF, 0 },
	{ 0xFF,0,   0xFF, 0 },
	{ 0,   0xFF,0xFF, 0 },
	{ 0xFF,0xFF,0xFF, 0 }
};

/**************************************************************
***************************************************************
********************  view_dialog_t ***************************
***************************************************************
**************************************************************/

int view_dialog_t::id[NUM_EDIT] = { IDE_FX,IDE_FY,IDE_FZ,IDE_TX,IDE_TY,IDE_TZ,IDE_NEAR,IDE_FAR,IDE_ZOOM, IDE_ELEV, IDE_ROT, IDE_DIST };

BEGIN_MESSAGE_MAP(view_dialog_t, CDialog)
	ON_WM_CLOSE()

	ON_CONTROL(EN_KILLFOCUS, IDE_ELEV, elev_kill)
	ON_CONTROL(EN_KILLFOCUS, IDE_ROT, rot_kill)
	ON_CONTROL(EN_KILLFOCUS, IDE_DIST, dist_kill)
	ON_CONTROL(EN_KILLFOCUS,IDE_FX,fxkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_FY,fykill)
	ON_CONTROL(EN_KILLFOCUS,IDE_FZ,fzkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_TX,txkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_TY,tykill)
	ON_CONTROL(EN_KILLFOCUS,IDE_TZ,tzkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_FAR,farkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_NEAR,nearkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_ZOOM,zoomkill)

	ON_NOTIFY(UDN_DELTAPOS, IDB_ELEV, &view_dialog_t::on_delta_elev)
	ON_NOTIFY(UDN_DELTAPOS, IDB_ROT, &view_dialog_t::on_delta_rot)
	ON_NOTIFY(UDN_DELTAPOS, IDB_DIST, &view_dialog_t::on_delta_dist)

	ON_NOTIFY(UDN_DELTAPOS, IDB_FX, &view_dialog_t::on_delta_fx)
	ON_NOTIFY(UDN_DELTAPOS, IDB_FY, &view_dialog_t::on_delta_fy)
	ON_NOTIFY(UDN_DELTAPOS, IDB_FZ, &view_dialog_t::on_delta_fz)

	ON_NOTIFY(UDN_DELTAPOS, IDB_TX, &view_dialog_t::on_delta_tx)
	ON_NOTIFY(UDN_DELTAPOS, IDB_TY, &view_dialog_t::on_delta_ty)
	ON_NOTIFY(UDN_DELTAPOS, IDB_TZ, &view_dialog_t::on_delta_tz)

	ON_NOTIFY(UDN_DELTAPOS, IDB_FAR, &view_dialog_t::on_delta_far)
	ON_NOTIFY(UDN_DELTAPOS, IDB_NEAR, &view_dialog_t::on_delta_near)
	ON_NOTIFY(UDN_DELTAPOS, IDB_ZOOM, &view_dialog_t::on_delta_zoom)
END_MESSAGE_MAP()

view_dialog_t::view_dialog_t(view_wnd_t* pw)
{
	fill(begin(_valid_edit), end(_valid_edit), true);
	_view_wnd = pw;
	update_data_from_view();
}

bool view_dialog_t::create()
{
	if (m_hWnd == NULL)
		return CDialog::Create(IDD_VIEWMDIALOG) ? true : false;
	else
		return false;
}

void view_dialog_t::update_data_from_view()
{
	_cam = _view_wnd->_main_camera_view;
	_near_plane = _view_wnd->_near_plane;
	_far_plane = _view_wnd->_far_plane;
	if (m_hWnd)
		fill_dialog_edits();
}

void view_dialog_t::kill(int idx,float &f)
{
	CEdit *pe = (CEdit *)GetDlgItem(id[idx]);
	if ( pe->GetModify())
	{
		pe->SetModify(FALSE);
		_valid_edit[idx] = get_value(id[idx], f);
		check_valid();
		notify_view();
	}
}

void view_dialog_t::disp_error(char *msg)
{
	SetDlgItemText(IDT_MSG,msg);
}

void view_dialog_t::check_valid()
{
	int i;
	_is_valid = true;
	for (i=0;i<NUM_EDIT;i++)
		if (!_valid_edit[i])
		{
			_is_valid = false;
			disp_error("Only integer numbers are valid");
		}
	if (_is_valid)
	{
		disp_error("");
		if (_near_plane <= 0.f || _far_plane <= 0.f)
		{
			disp_error("Clipping planes must be greater than 0");
			_is_valid = false;
		}
		if (_far_plane <= _near_plane)
		{
			disp_error("Far plane must be greater than near plane");
			_is_valid = false;
		}
		if (_cam.focal_length < 1.)
		{
			disp_error("Focal length must be greater than 0");
			_is_valid = false;
		}
		if (_cam.zoom < 1.)
		{
			disp_error("Zoom must be greater than 0");
			_is_valid = false;
		}
		float dx = _cam.to.x-_cam.from.x;
		float dy = _cam.to.y-_cam.from.y;
		float dz = _cam.to.z-_cam.from.z;
		float d = dx*dx+dy*dy+dz*dz;
		if (d < 1.F)
		{
			disp_error("Distance between from and to point should be greater than 1");
			_is_valid = false;
		}
	}
}

void view_dialog_t::notify_view()
{
	if (_is_valid)
	{
		_view_wnd->set_camera(_cam);
		_view_wnd->set_clip_plane(_near_plane, _far_plane);
		_view_wnd->UpdateWindow();
	}
}

void view_dialog_t::procb(int idx,float &f,float t)
{
	f = (get_value(id[idx],f)) ? (f + t) : 0.F;
	set_dlg_item_float_to_int(id[idx], f);
	_valid_edit[idx] = true;
	check_valid();
	notify_view();
}

void view_dialog_t::procb_sync_minmax(int idx, float& f, float t, float min, float max, bool wrap)
{
	f = (get_value(id[idx], f)) ? (f + t) : 0.F;
	if (f < min)
		f = wrap ? max : min;
	if (f > max)
		f = wrap ? min : max;
	set_dlg_item_float_to_int(id[idx], f);
	_valid_edit[idx] = true;
	switch (idx)
	{
	case IDX_FX:
	case IDX_FY:
	case IDX_FZ:
		update_spherical();
		// Update elev/rot/dist
		break;
	case IDX_ELEV:
	case IDX_ROT:
	case IDX_DIST:
		update_cartesian();
		// Update fx/fy/fz
		break;
	}
	check_valid();
	notify_view();
}

void view_dialog_t::procb_sync(int idx, float& f, float t)
{
	f = (get_value(id[idx], f)) ? (f + t) : 0.F;
	set_dlg_item_float_to_int(id[idx], f);
	_valid_edit[idx] = true;
	switch (idx)
	{
	case IDX_FX:
	case IDX_FY:
	case IDX_FZ:
		update_spherical();
		// Update elev/rot/dist
		break;
	case IDX_ELEV:
	case IDX_ROT:
	case IDX_DIST:
		update_cartesian();
		// Update fx/fy/fz
		break;
	}
	check_valid();
	notify_view();
}

/* Compute cartesian coordinates from spherical. */
void view_dialog_t::update_cartesian()
{
	point3d_t sph;
	get_value(IDE_ELEV, sph.y);
	get_value(IDE_ROT, sph.x);
	get_value(IDE_DIST, sph.z);
	sph.y = DEG2RAD(sph.y);
	sph.x = DEG2RAD(sph.x);
	auto mat = mat44::sph(sph.x, sph.y, sph.z);
	point3d_t cart;
	mat.get_rect(cart);
	point3d_t pt = _cam.to;
	_cam.from = _cam.to + cart;
	set_dlg_item_float_to_int(IDE_FX, _cam.from.x);
	set_dlg_item_float_to_int(IDE_FY, _cam.from.y);
	set_dlg_item_float_to_int(IDE_FZ, _cam.from.z);
}

/* Compute spherical coordinates from "from" and "to" camera. */
void view_dialog_t::update_spherical()
{
	auto vec = _cam.from - _cam.to;
	auto mat = mat44::trans(vec);
	point3d_t sph;
	mat.get_sph(sph);
	set_dlg_item_float_to_int(IDE_ELEV, RAD2DEG(sph.y));
	set_dlg_item_float_to_int(IDE_ROT, RAD2DEG(sph.x));
	set_dlg_item_float_to_int(IDE_DIST, sph.z);
}

void view_dialog_t::fill_dialog_edits()
{
	set_dlg_item_float_to_int(IDE_FX, _cam.from.x);
	set_dlg_item_float_to_int(IDE_FY, _cam.from.y);
	set_dlg_item_float_to_int(IDE_FZ, _cam.from.z);
	set_dlg_item_float_to_int(IDE_TX, _cam.to.x);
	set_dlg_item_float_to_int(IDE_TY, _cam.to.y);
	set_dlg_item_float_to_int(IDE_TZ, _cam.to.z);
	set_dlg_item_float_to_int(IDE_FAR, _far_plane);
	set_dlg_item_float_to_int(IDE_NEAR, _near_plane);
	set_dlg_item_float_to_int(IDE_ZOOM, _cam.zoom);
	update_spherical();
	check_valid();
}

BOOL view_dialog_t::OnInitDialog()
{
	fill_dialog_edits();
	return CDialog::OnInitDialog();
}

void view_dialog_t::OnClose()
{
	DestroyWindow();
}

void view_dialog_t::OnOK()
{
}

void view_dialog_t::OnCancel()
{
	DestroyWindow();
}

bool view_dialog_t::get_value(int nID,float &val)
{
	BOOL trans;
	val = (float)(int)GetDlgItemInt(nID,&trans,TRUE);
	return trans ? true : false;
}

void view_dialog_t::set_dlg_item_float_to_int(int nID, float f)
{
	SetDlgItemInt(nID, (int)round(f), TRUE);
}

void view_dialog_t::on_delta_elev(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb_sync_minmax(IDX_ELEV, _cam.from.x, elev_incr * delta, 1.f, 180.f);
	*pResult = 0;
}

void view_dialog_t::on_delta_rot(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb_sync_minmax(IDX_ROT, _cam.from.y, rot_incr * delta, -179.f, 179.f, true/*wrap*/);
	*pResult = 0;
}

void view_dialog_t::on_delta_dist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb_sync_minmax(IDX_DIST, _cam.from.z, dist_incr * delta, 1.0f, 1.0e20f);
	*pResult = 0;
}

void view_dialog_t::on_delta_fx(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb_sync(IDX_FX, _cam.from.x, fy_incr * delta);
	*pResult = 0;
}

void view_dialog_t::on_delta_fy(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb_sync(IDX_FY, _cam.from.y, fy_incr * delta);
	*pResult = 0;
}

void view_dialog_t::on_delta_fz(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb_sync(IDX_FZ, _cam.from.z, fz_incr * delta);
	*pResult = 0;
}

void view_dialog_t::on_delta_tx(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb(IDX_TX, _cam.to.x, tx_incr * delta);
	*pResult = 0;
}

void view_dialog_t::on_delta_ty(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb(IDX_TY, _cam.to.y, ty_incr * delta);
	*pResult = 0;
}

void view_dialog_t::on_delta_tz(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb(IDX_TZ, _cam.to.z, tz_incr * delta);
	*pResult = 0;
}

void view_dialog_t::on_delta_far(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb(IDX_FAR, _far_plane, far_incr * delta);
	*pResult = 0;
}

void view_dialog_t::on_delta_near(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb(IDX_NEAR, _near_plane, near_incr * delta);
	*pResult = 0;
}

void view_dialog_t::on_delta_zoom(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float delta = (float)-pNMUpDown->iDelta;
	procb(IDX_ZOOM, _cam.zoom, zoom_incr * delta);
	*pResult = 0;
}

/**************************************************************
***************************************************************
*********************  View Window ****************************
***************************************************************
**************************************************************/

BEGIN_MESSAGE_MAP(view_wnd_t, CWnd)
	//{{AFX_MSG_MAP(CViewWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

view_wnd_t::view_wnd_t(simulation_t& rs) : _sim(rs)
{
	_lights.resize(8);
	_lights[0]._is_enabled = true;
	_lights[0]._is_spot_enabled = false;
	ASSERT(rs._view_wnd == nullptr);
	//if (rs._view_wnd != nullptr)
	//	rs._view_wnd->DestroyWindow();
	rs._view_wnd = this;
	_view_dialog = make_unique<view_dialog_t>(this);
	if (_class_name.empty())
		_class_name = AfxRegisterWndClass(CS_BYTEALIGNCLIENT | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW), (HBRUSH)BLACK_BRUSH, LoadIcon(AfxGetInstanceHandle(), "wroboicon"));
}

BOOL view_wnd_t::Create(CFrameWnd* p)
{
	return CWnd::CreateEx(0, _class_name.c_str(), "View", WS_CHILD | WS_VISIBLE,
		0, 0, 200, 200, p->GetSafeHwnd(), (HMENU)AFX_IDW_PANE_FIRST);
}

view_wnd_t::~view_wnd_t()
{
	_view_dialog.reset();
	ASSERT(_sim._view_wnd == this);
	_sim._view_wnd = nullptr;
}

void view_wnd_t::redraw(bool bErase)
{
	Invalidate(bErase);
	UpdateWindow();
}

void view_wnd_t::update_data()
{
	// if the view dialog is showing and this window is current set it again to update
	if (_view_dialog)
		_view_dialog->update_data_from_view();
}

void view_wnd_t::invalidate(bool erase)
{
	Invalidate(erase);
}

void view_wnd_t::update_window()
{
	UpdateWindow();
}

void view_wnd_t::adjust_lights()
{
	CPropertySheet sheet(_T("Lights"));
	unique_ptr<light_page_t> pages[8];
	unique_ptr<ambient_light_page_t> ambient_page = make_unique<ambient_light_page_t>("Global Ambient", _ambient_light, *this);
	sheet.AddPage(ambient_page.get());
	for (int i = 0; i < 8; i++)
	{
		char buf[20];
		sprintf_s(buf, "Light %d", i);
		pages[i] = make_unique<light_page_t>(buf, _lights[i], *this);
		sheet.AddPage(pages[i].get());
	}
	if (sheet.DoModal() == IDOK)
	{
		for (int i = 0; i < 8; i++)
			_lights[i] = pages[i]->_light;
	}
	Invalidate();
}

COLORREF view_wnd_t::_colorref_back = RGB(0, 0, 0);
string view_wnd_t::_class_name;

int view_wnd_t::OnCreate(LPCREATESTRUCT lpcs)
{
	static GLfloat obAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	static GLfloat obDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	static GLfloat obSpecular[] = { .4f, .4f, .4f, 1.0f };

	if (CWnd::OnCreate(lpcs) == -1)
		return -1;

	HDC         hdc = ::GetDC(m_hWnd);
	HGLRC		hrc;

	setup_pixel_format(hdc);
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	CRect rect;
	GetClientRect(&rect);

	glClearColor((float)GetRValue(_colorref_back) / 255.f, (float)GetGValue(_colorref_back) / 255.f,
		(float)GetBValue(_colorref_back) / 255.f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);

	return 0;
}

void view_wnd_t::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	Invalidate();
}

BOOL view_wnd_t::PreCreateWindow(CREATESTRUCT& cs)
{
	//	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CWnd::PreCreateWindow(cs);
}

bool view_wnd_t::setup_pixel_format(HDC hDC)
{
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,	// dwFlags
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ((pixelformat = ChoosePixelFormat(hDC, &pfd)) == 0)
	{
		AfxMessageBox("ChoosePixelFormat failed");
		return false;
	}

	if (!SetPixelFormat(hDC, pixelformat, &pfd))
	{
		AfxMessageBox("SetPixelFormat failed");
		return false;
	}

	create_rgb_palette(hDC);

	return true;
}

bool view_wnd_t::setup_pixel_format_for_bitmap(HDC hDC)
{
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_SUPPORT_OPENGL | PFD_DRAW_TO_BITMAP | PFD_SUPPORT_GDI,// dwFlags
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	HBITMAP hbm = (HBITMAP)::GetCurrentObject(hDC, OBJ_BITMAP);
	BITMAP bmInfo;
	::GetObject(hbm, sizeof(BITMAP), &bmInfo);

	ASSERT(bmInfo.bmPlanes == 1);
	ASSERT((bmInfo.bmBitsPixel == 8) || (bmInfo.bmBitsPixel == 16) ||
		(bmInfo.bmBitsPixel == 24));
	pfd.cColorBits = (BYTE)bmInfo.bmBitsPixel;

	int pixelformat;
	if ((pixelformat = ChoosePixelFormat(hDC, &pfd)) == 0)
	{
		AfxMessageBox("ChoosePixelFormat failed");
		return false;
	}

	if (!SetPixelFormat(hDC, pixelformat, &pfd))
	{
		AfxMessageBox("SetPixelFormat failed");
		return false;
	}

	create_rgb_palette(hDC);

	return true;
}

unsigned char view_wnd_t::component_for_index(int i, UINT nbits, UINT shift)
{
	unsigned char val;

	val = (unsigned char)(i >> shift);
	switch (nbits) {

	case 1:
		val &= 0x1;
		return oneto8[val];
	case 2:
		val &= 0x3;
		return twoto8[val];
	case 3:
		val &= 0x7;
		return threeto8[val];

	default:
		return 0;
	}
}

void view_wnd_t::create_rgb_palette(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE* pPal;
	int n, i;
	CDC dc;

	n = ::GetPixelFormat(hDC);
	::DescribePixelFormat(hDC, n, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	if (pfd.dwFlags & PFD_NEED_PALETTE)
	{
		n = 1 << pfd.cColorBits;
		pPal = (PLOGPALETTE)malloc(sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY));
		pPal->palVersion = 0x300;
		pPal->palNumEntries = n;
		for (i = 0; i < n; i++)
		{
			pPal->palPalEntry[i].peRed =
				component_for_index(i, pfd.cRedBits, pfd.cRedShift);
			pPal->palPalEntry[i].peGreen =
				component_for_index(i, pfd.cGreenBits, pfd.cGreenShift);
			pPal->palPalEntry[i].peBlue =
				component_for_index(i, pfd.cBlueBits, pfd.cBlueShift);
			pPal->palPalEntry[i].peFlags = 0;
		}

		/* fix up the palette to include the default GDI palette */
		if ((pfd.cColorBits == 8) &&
			(pfd.cRedBits == 3) && (pfd.cRedShift == 0) &&
			(pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
			(pfd.cBlueBits == 2) && (pfd.cBlueShift == 6)
			)
		{
			for (i = 1; i <= 12; i++)
				pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
		}

		dc.Attach(hDC);

		_palette.CreatePalette(pPal);
		free(pPal);
		CPalette* pOldPal = dc.SelectPalette(&_palette, FALSE);
		dc.RealizePalette();
		dc.Detach();
	}
}

void view_wnd_t::copy_to_clipboard()
{
	CRect rect;
	GetClientRect(rect);

	HDC hOldDC = ::wglGetCurrentDC();
	HGLRC hOldRC = ::wglGetCurrentContext();
	wglMakeCurrent(NULL, NULL);

	HDC hDC = ::CreateCompatibleDC(NULL);
	ASSERT(hDC != NULL);

	// Create a BITMAPINFOHEADER structure to describe the DIB
	BITMAPINFOHEADER bih;
	memset(&bih, 0, sizeof(bih));

	// Fill in the header info.
	bih.biSize = sizeof(bih);
	bih.biWidth = rect.Width();
	bih.biHeight = rect.Height();
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;

	// Create the DIB section.
	void* pv;
	HBITMAP hbm = CreateDIBSection(hDC, (BITMAPINFO*)&bih, DIB_PAL_COLORS, &pv, NULL, 0);
	ASSERT(hbm != NULL);
	HBITMAP hbmOld = (HBITMAP)::SelectObject(hDC, hbm);

	setup_pixel_format_for_bitmap(hDC);
	HGLRC hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	//	wglMakeCurrent(hDC, hOldRC);

	glClearColor((float)GetRValue(_colorref_back) / 255.f, (float)GetGValue(_colorref_back) / 255.f,
		(float)GetBValue(_colorref_back) / 255.f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	//	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
	//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_lights[0].globalambientlight);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);

	paint_window(rect);

	::SelectObject(hDC, hbmOld);
	::GetDIBits(hDC, hbm, 0, 0, NULL, (BITMAPINFO*)&bih, DIB_PAL_COLORS);
	ASSERT(bih.biBitCount == 24);
	int nSize = sizeof(bih) + bih.biSizeImage;
	HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nSize);
	ASSERT(h != NULL);
	BYTE* pb = (BYTE*)GlobalLock(h);
	memcpy(pb, &bih, sizeof(bih));
	pb += sizeof(bih);
	VERIFY(::GetDIBits(hDC, hbm, 0, bih.biHeight, pb, (BITMAPINFO*)&bih, DIB_PAL_COLORS));
	GlobalUnlock(h);
	::DeleteObject(hbm);
	::DeleteDC(hDC);

	if (OpenClipboard())
	{
		if (EmptyClipboard())
			VERIFY(SetClipboardData(CF_DIB, (HANDLE)h));
		CloseClipboard();
	}

	wglMakeCurrent(hOldDC, hOldRC);
}

void view_wnd_t::paint_window(CRect& rect)
{
	static bool bBusy = false;
	if (bBusy)
		return;
	bBusy = true;

	float fAspect = (GLfloat)rect.right / rect.bottom;
	float fov = 45.f;

	int nWidth = MulDiv(rect.right, (int)_main_camera_view.zoom, 100);
	int nHeight = MulDiv(rect.bottom, (int)_main_camera_view.zoom, 100);
	if (_cull)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
		glDisable(GL_CULL_FACE);
	glShadeModel(_smooth_shading ? GL_SMOOTH : GL_FLAT);
	if (_lighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, _wire_frame ? GL_LINE : GL_FILL);
	glViewport(rect.right / 2 - nWidth / 2, rect.bottom / 2 - nHeight / 2, nWidth, nHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, fAspect, _near_plane, _far_plane);
	//	gluLookAt(mainview.from.x, mainview.from.y, mainview.from.z,
	//		mainview.to.x, mainview.to.y, mainview.to.z,
	//		0.f, 0.f, 1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(_main_camera_view.from.x, _main_camera_view.from.y, _main_camera_view.from.z,
		_main_camera_view.to.x, _main_camera_view.to.y, _main_camera_view.to.z,
		0.f, 0.f, 1.f);
	for (int i = 0; i < (int)_lights.size(); i++)
	{
		light_t& light = _lights[i];
		light.turn_on(GL_LIGHT0 + i);
		if (light._is_enabled)
			glEnable(GL_LIGHT0 + i);
		else
			glDisable(GL_LIGHT0 + i);
		light.set_pos(GL_LIGHT0 + i);
	}
	if (_ambient_light._is_enabled)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, _ambient_light.light);
	else
	{
		GLfloat ambient[4]{};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	}

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	_sim.draw();
	glPopMatrix();

	glFinish();
	SwapBuffers(wglGetCurrentDC());

	bBusy = false;
}

void view_wnd_t::OnDestroy()
{
	HDC     hdc;
	HGLRC	hrc;

	hdc = ::wglGetCurrentDC();
	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent(NULL, NULL);

	if (hrc)
		::wglDeleteContext(hrc);
	if (hdc)
		::ReleaseDC(m_hWnd, hdc);

	CWnd::OnDestroy();
}

// cases
// 1. paint to window -- color and same page
// 2. paint to metafile -- no color and same page
// 3. paint to printer -- no color and separate pages

int view_wnd_t::paint_printer(CDC& dc, int page)
{
	CRect rectWnd, rectPrinter;
	rectPrinter.left = rectPrinter.top = 0;
	rectPrinter.right = dc.GetDeviceCaps(HORZRES);
	rectPrinter.bottom = dc.GetDeviceCaps(VERTRES);
	GetClientRect(&rectWnd);

	HDC hOldDC = ::wglGetCurrentDC();
	HGLRC hOldRC = ::wglGetCurrentContext();
	wglMakeCurrent(NULL, NULL);

	HDC hDC = ::CreateCompatibleDC(dc.m_hDC);
	ASSERT(hDC != NULL);

	// Create a BITMAPINFOHEADER structure to describe the DIB
	BITMAPINFOHEADER bih;
	memset(&bih, 0, sizeof(bih));

	// Fill in the header info.
	bih.biSize = sizeof(bih);
	bih.biWidth = rectWnd.Width();
	bih.biHeight = rectWnd.Height();
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;

	// Create the DIB section.
	void* pv;
	HBITMAP hbm = CreateDIBSection(hDC, (BITMAPINFO*)&bih, DIB_PAL_COLORS, &pv, NULL, 0);
	ASSERT(hbm != NULL);
	HBITMAP hbmOld = (HBITMAP)::SelectObject(hDC, hbm);

	setup_pixel_format_for_bitmap(hDC);
	HGLRC hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	//	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
	//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalambientlight);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);

	paint_window(rectWnd);
	// copy from bitmap to printer
//	::BitBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(), hDC, 0, 0, SRCCOPY);
	float r1 = (float)rectPrinter.Width() / (float)rectWnd.Width();
	float r2 = (float)rectPrinter.Height() / (float)rectWnd.Height();
	float r = min(r1, r2);
	int nWidth = (int)((float)rectWnd.Width() * r);
	int nHeight = (int)((float)rectWnd.Height() * r);
	::StretchBlt(dc.m_hDC, 0, 0, nWidth, nHeight,
		hDC, 0, 0, rectWnd.Width(), rectWnd.Height(), SRCCOPY);

	::SelectObject(hDC, hbmOld);
	::DeleteObject(hbm);
	::DeleteDC(hDC);

	wglMakeCurrent(hOldDC, hOldRC);
	return 0;
}

void view_wnd_t::OnPaint(void)
{
	CPaintDC dc(this);
	glClearColor((float)GetRValue(_colorref_back) / 255.f, (float)GetGValue(_colorref_back) / 255.f,
		(float)GetBValue(_colorref_back) / 255.f, 1.0f);
	CRect rect;
	GetClientRect(&rect);
	paint_window(rect);
}

BOOL view_wnd_t::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void view_wnd_t::set_background_color(COLORREF cr)
{
	_colorref_back = cr;
}

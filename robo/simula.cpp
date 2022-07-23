#include "stdafx.h"
#include "resource.h"
#include "view.h"
#include "mainwin.h"
#include "lightpage.h"
#include "grob.h"
#include "package.h"

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

COLORREF view_wnd_t::_colorref_back = RGB(0, 0, 0);
string view_wnd_t::_class_name;


/**************************************************************
***************************************************************
*********************  Simulation *****************************
***************************************************************
**************************************************************/

void simulation_t::reset_simulation(void)
{
	for (auto obj : _objects)
		obj->set_in_env(false);
	_objects.clear();
}

void simulation_t::draw()
{
	for (auto obj : _objects)
		obj->Draw();
}

void simulation_t::redraw(bool bErase)
{
	MSG keymsg;
	if (PeekMessage(&keymsg, NULL, WM_KEYDOWN, WM_KEYDOWN, PM_NOREMOVE))
		if (AfxMessageBox("Do you want to stop?", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDYES)
			throw_interrupt_exception();
	if (!_update)
		return;
	if (_view_wnd != NULL)
		_view_wnd->redraw(bErase);
	check_collision();
}

bool simulation_t::add_object(simob_t* psobj)
{
	for (auto& obj : _objects)
	{
		if (obj == psobj)
			return false;
	}
	psobj->set_in_env(true);
	_objects.push_back(psobj);
	return true;
}

bool simulation_t::remove_object(simob_t* psobj, bool bDeleteViews)
{
	for (auto iter = _objects.begin(); iter != _objects.end(); ++iter)
	{
		if (*iter == psobj)
		{
			if (bDeleteViews)
				(*iter)->set_in_env(false);
			_objects.erase(iter);
			return true;
		}
	}
	return false;
}

bool simulation_t::remove_all_objects(void)
{
	bool hasItems = !_objects.empty();
	for (auto obj : _objects)
		obj->set_in_env(false);
	_objects.clear();
	return hasItems;
}

bool simulation_t::check_collision(void)
{
	if (!_is_collision_on)
		return false; /* collision detection not on */

	int i, j, n = (int)_objects.size();
	simob_t* ps;
	for (i = 0; i < n; i++)
	{
		ps = _objects[i];
		//		if (ps->CheckSelfCollision())
		//			return 1;
		for (j = i + 1; j < n; j++)
		{
			if (simob_t::check_collide(ps, _objects[j]))
				throw_other_exception(current_package->get_symbol("COLLISION"), nil); // out of reach exception
//				throw_eval_exception(COLLISION_ERROR);
//				return TRUE;
		}
	}
	return false;
}

void simulation_t::set_update(bool b)
{
	_update = b;
	if (b)
		redraw(true);
}

bool simulation_t::set_look_from(const point3d_t& from)
{
	if (!_view_wnd)
		return false;
	_view_wnd->set_look_from(from);
	_view_wnd->UpdateWindow();
	return true;
}

bool simulation_t::get_look_from(point3d_t& from)
{
	if (!_view_wnd)
		return false;
	from = _view_wnd->get_camera().from;
	return true;
}

bool simulation_t::set_look_at(const point3d_t& at)
{
	if (!_view_wnd)
		return false;
	_view_wnd->set_look_from(at);
	_view_wnd->UpdateWindow();
	return true;
}

bool simulation_t::get_look_at(point3d_t& at)
{
	if (!_view_wnd)
		return false;
	at = _view_wnd->get_camera().to;
	return true;
}

bool simulation_t::set_camera(float focal, float twist, float zoom)
{
	if (!_view_wnd)
		return false;
	_view_wnd->SetCam(focal, twist, zoom);
	_view_wnd->UpdateWindow();
	return true;
}

bool simulation_t::get_camera(float& focal, float& twist, float& zoom)
{
	if (!_view_wnd)
		return false;
	auto& cam = _view_wnd->get_camera();
	focal = cam.focal_length;
	twist = cam.twist;
	zoom = cam.zoom;
	return true;
}

bool simulation_t::set_clip_plane(float near_plane, float far_plane)
{
	if (!_view_wnd)
		return false;
	_view_wnd->set_clip_plane(near_plane, far_plane);
	_view_wnd->Invalidate();
	return true;
}

bool simulation_t::get_light(int index, light_t& light)
{
	if (!_view_wnd)
		return false;
	light = _view_wnd->_lights[index];
	return true;

}

bool simulation_t::set_light(int index, const light_t& light)
{
	if (!_view_wnd)
		return false;
	_view_wnd->_lights[index] = light;
	redraw(true);
	return true;
}

bool simulation_t::adjust_lights()
{
	if (!_view_wnd)
		return nil;
	_view_wnd->adjust_lights();
	return true;
}

bool simulation_t::enable_solid(bool solid)
{
	if (!_view_wnd)
		return nil;
	_view_wnd->set_wire_frame(!solid);
	_view_wnd->Invalidate();
	return true;
}

bool simulation_t::enable_lighting(bool lighting)
{
	if (!_view_wnd)
		return nil;
	_view_wnd->set_lighting(lighting);
	_view_wnd->Invalidate();
	return true;
}

bool simulation_t::enable_cull(bool cull)
{
	if (!_view_wnd)
		return nil;
	_view_wnd->set_cull(cull);
	_view_wnd->Invalidate();
	return true;

}

bool simulation_t::enable_smooth_shading(bool smooth)
{
	if (!_view_wnd)
		return nil;
	_view_wnd->set_smooth_shading(smooth);
	_view_wnd->Invalidate();
	return true;
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

view_wnd_t::view_wnd_t(simulation_t& rs) : sim(rs)
{
	_near_plane = 50.0f;
	_far_plane = 4000.0f;
	_wire_frame = true;
	_lighting = false;
	_cull = false;
	_smooth_shading = false;
	_lights.resize(8);
	_lights[0]._is_enabled = true;
	_lights[0]._is_spot_enabled = false;
	ASSERT(rs._view_wnd == NULL);
	if (rs._view_wnd != NULL)
		rs._view_wnd->DestroyWindow();
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
	ASSERT(sim._view_wnd == this);
	sim._view_wnd = nullptr;
}

void view_wnd_t::redraw(bool bErase)
{
	Invalidate(bErase);
	UpdateWindow();
}

void view_wnd_t::set_camera(camera_t& cam)
{
	_main_camera_view = cam;
	Invalidate();
}

camera_t& view_wnd_t::get_camera(void)
{
	return _main_camera_view;
}

void view_wnd_t::set_look_from(const point3d_t& pt)
{
	_main_camera_view.from = pt;
	set_camera(_main_camera_view);
	// if the view dialog is showing and this window is current set it again to update
	if (_view_dialog)
		_view_dialog->update_data_from_view();
}

void view_wnd_t::set_look_at(const point3d_t& pt)
{
	_main_camera_view.to = pt;
	set_camera(_main_camera_view);
	// if the view dialog is showing and this window is current set it again to update
	if (_view_dialog)
		_view_dialog->update_data_from_view();
}

void view_wnd_t::SetCam(float f, float t, float z)
{
	_main_camera_view.focal_length = f;
	_main_camera_view.twist = t;
	_main_camera_view.zoom = z;
	set_camera(_main_camera_view);
	// if the view dialog is showing and this window is current set it again to update
	if (_view_dialog)
		_view_dialog->update_data_from_view();
}

void view_wnd_t::set_clip_plane(float near_plane, float far_plane)
{
	_near_plane = near_plane;
	_far_plane = far_plane;
	// if the view dialog is showing and this window is current set it again to update
	if (_view_dialog)
		_view_dialog->update_data_from_view();
	Invalidate();
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

light_t::light_t()
{
	_is_enabled = false;
	_ambient_properties[0] = _ambient_properties[1] = _ambient_properties[2] = .3f;
	_ambient_properties[3] = 1.f;
	_diffuse_properties[0] = _diffuse_properties[1] = _diffuse_properties[2] = .6f;
	_diffuse_properties[3] = 1.f;
	_specular_properties[0] = _specular_properties[1] = _specular_properties[2] = _specular_properties[3] = .5f;
	_position[0] = 50.f;
	_position[1] = 30.f;
	_position[2] = 500.f;
	_position[3] = 1.f;
	constant_att = 1.f;
	linear_att = 0.f;
	quadratic_att = 0.f;
	_is_spot_enabled = false;
	_spotlight_direction[0] = 0.f;
	_spotlight_direction[1] = 0.f;
	_spotlight_direction[2] = 0.f;
	_spotlight_exponent = 0.f;
	_spotlight_cutoff = 180.f;
}

void light_t::turn_on(GLenum id)
{
	glLightfv(id, GL_AMBIENT, _ambient_properties);
	glLightfv(id, GL_DIFFUSE, _diffuse_properties);
	glLightfv(id, GL_SPECULAR, _specular_properties);
	glLightf(id, GL_CONSTANT_ATTENUATION, constant_att);
	glLightf(id, GL_LINEAR_ATTENUATION, linear_att);
	glLightf(id, GL_QUADRATIC_ATTENUATION, quadratic_att);
	if (_is_spot_enabled)
	{
		glLightfv(id, GL_SPOT_DIRECTION, _spotlight_direction);
		glLightf(id, GL_SPOT_EXPONENT, _spotlight_exponent);
		glLightf(id, GL_SPOT_CUTOFF, _spotlight_cutoff);
	}
}

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
		pPal = (PLOGPALETTE)LocalAlloc(LMEM_FIXED, sizeof(LOGPALETTE) +
			n * sizeof(PALETTEENTRY));
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

		CPalette* pOldPal;

		_palette.CreatePalette(pPal);
		LocalFree(pPal);
		pOldPal = dc.SelectPalette(&_palette, FALSE);
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
	sim.draw();
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

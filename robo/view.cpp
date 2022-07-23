#include "stdafx.h"
#include <afxdlgs.h>
#include "resource.h"
#include "view.h"
#include "mainwin.h"
#include <algorithm>
#include "grob.h"

using namespace std;
int view_dialog_t::id[NUM_EDIT] = {IDE_FX,IDE_FY,IDE_FZ,IDE_TX,IDE_TY,IDE_TZ,IDE_FOCAL,IDE_TWIST,IDE_ZOOM, IDE_ELEV, IDE_ROT, IDE_DIST};

BEGIN_MESSAGE_MAP(view_dialog_t, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDB_ELEV, elevu)
	ON_BN_DOUBLECLICKED(IDB_ELEV, elevd)
	ON_BN_CLICKED(IDB_ROT, rotu)
	ON_BN_DOUBLECLICKED(IDB_ROT, rotd)
	ON_BN_CLICKED(IDB_DIST, distu)
	ON_BN_DOUBLECLICKED(IDB_DIST, distd)

	ON_BN_CLICKED(IDB_FX,fxu)
	ON_BN_DOUBLECLICKED(IDB_FX,fxd)
	ON_BN_CLICKED(IDB_FY,fyu)
	ON_BN_DOUBLECLICKED(IDB_FY,fyd)
	ON_BN_CLICKED(IDB_FZ,fzu)
	ON_BN_DOUBLECLICKED(IDB_FZ,fzd)

	ON_BN_CLICKED(IDB_TX,txu)
	ON_BN_DOUBLECLICKED(IDB_TX,txd)
	ON_BN_CLICKED(IDB_TY,tyu)
	ON_BN_DOUBLECLICKED(IDB_TY,tyd)
	ON_BN_CLICKED(IDB_TZ,tzu)
	ON_BN_DOUBLECLICKED(IDB_TZ,tzd)

	ON_BN_CLICKED(IDB_TWIST,twistu)
	ON_BN_DOUBLECLICKED(IDB_TWIST,twistd)
	ON_BN_CLICKED(IDB_FOCAL,focalu)
	ON_BN_DOUBLECLICKED(IDB_FOCAL,focald)
	ON_BN_CLICKED(IDB_ZOOM,zoomu)
	ON_BN_DOUBLECLICKED(IDB_ZOOM,zoomd)

	ON_CONTROL(EN_KILLFOCUS, IDE_ELEV, elev_kill)
	ON_CONTROL(EN_KILLFOCUS, IDE_ROT, rot_kill)
	ON_CONTROL(EN_KILLFOCUS, IDE_DIST, dist_kill)
	ON_CONTROL(EN_KILLFOCUS,IDE_FX,fxkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_FY,fykill)
	ON_CONTROL(EN_KILLFOCUS,IDE_FZ,fzkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_TX,txkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_TY,tykill)
	ON_CONTROL(EN_KILLFOCUS,IDE_TZ,tzkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_TWIST,twistkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_FOCAL,focalkill)
	ON_CONTROL(EN_KILLFOCUS,IDE_ZOOM,zoomkill)
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
	auto mat = mat44::SPH(sph.x, sph.y, sph.z);
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
	auto mat = mat44::TRANS(vec);
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
	set_dlg_item_float_to_int(IDE_TWIST, _far_plane);
	set_dlg_item_float_to_int(IDE_FOCAL, _near_plane);
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


#pragma once
#ifndef __VIEW_H__
#define __VIEW_H__
#include "simula.h"

#define IDX_FX 0
#define IDX_FY 1
#define IDX_FZ 2
#define IDX_TX 3
#define IDX_TY 4
#define IDX_TZ 5
#define IDX_FOCAL 6
#define IDX_TWIST 7
#define IDX_ZOOM 8
#define IDX_ELEV 9
#define IDX_ROT 10
#define IDX_DIST 11
#define NUM_EDIT 12

class view_dialog_t : public CDialog
{
public:
	view_dialog_t(view_wnd_t* pw);
	enum { IDD = IDD_VIEWMDIALOG };

	bool create();
	void update_data_from_view();

protected:
	void notify_view();
	void fill_dialog_edits();
	void procb(int idx,float &f,float t = 5.f);
	void procb_sync(int idx, float& f, float t = 5.f);
	void procb_sync_minmax(int idx, float& f, float t, float min, float max, bool wrap = false);
	void kill(int idx,float &f);
	void check_valid();
	void disp_error(char *msg);
	void update_cartesian();
	void update_spherical();

	BOOL OnInitDialog() override;
	virtual void OnOK() override;
	virtual void OnCancel() override;

	afx_msg void OnClose();
	afx_msg void elevu() { procb_sync_minmax(IDX_ELEV, _cam.from.x, (float)5., 1.f, 180.f); }
	afx_msg void rotu() { procb_sync_minmax(IDX_ROT, _cam.from.y, 5., -179.f, 179.f, true/*wrap*/); }
	afx_msg void distu() { procb_sync_minmax(IDX_DIST, _cam.from.z, (float)5., 1.0f, 1.0e20f); }
	afx_msg void fxu() {procb_sync(IDX_FX,_cam.from.x,(float)5.);}
	afx_msg void fyu() {procb_sync(IDX_FY,_cam.from.y,(float)5.);}
	afx_msg void fzu() {procb_sync(IDX_FZ,_cam.from.z,(float)5.);}
	afx_msg void txu() {procb(IDX_TX,_cam.to.x,(float)5.);}
	afx_msg void tyu() {procb(IDX_TY,_cam.to.y,(float)5.);}
	afx_msg void tzu() {procb(IDX_TZ,_cam.to.z,(float)5.);}
	afx_msg void twistu() {procb(IDX_TWIST,_far_plane,(float)5.);}
	afx_msg void focalu() {procb(IDX_FOCAL,_near_plane,(float)5.);}
	afx_msg void zoomu() {procb(IDX_ZOOM,_cam.zoom,(float)5.);}

	afx_msg void elevd() { procb_sync_minmax(IDX_ELEV, _cam.from.x, -5.f, 1.f, 180.f); }
	afx_msg void rotd() { procb_sync_minmax(IDX_ROT, _cam.from.y, -5., -179.f, 179.f, true/*wrap*/); }
	afx_msg void distd() { procb_sync_minmax(IDX_DIST, _cam.from.z, -5.f, 1.0f, 1.0e20f); }
	afx_msg void fxd() {procb_sync(IDX_FX,_cam.from.x,-(float)5.);}
	afx_msg void fyd() {procb_sync(IDX_FY,_cam.from.y,-(float)5.);}
	afx_msg void fzd() {procb_sync(IDX_FZ,_cam.from.z,-(float)5.);}
	afx_msg void txd() {procb(IDX_TX,_cam.to.x,-(float)5.);}
	afx_msg void tyd() {procb(IDX_TY,_cam.to.y,-(float)5.);}
	afx_msg void tzd() {procb(IDX_TZ,_cam.to.z,-(float)5.);}
	afx_msg void twistd() {procb(IDX_TWIST,_far_plane,-(float)5.);}
	afx_msg void focald() {procb(IDX_FOCAL,_near_plane,-(float)5.);}
	afx_msg void zoomd() {procb(IDX_ZOOM,_cam.zoom,-(float)5.);}

	afx_msg void elev_kill() { kill(IDX_FX, _cam.from.x); }
	afx_msg void rot_kill() { kill(IDX_FY, _cam.from.y); }
	afx_msg void dist_kill() { kill(IDX_FZ, _cam.from.z); }
	afx_msg void fxkill() {kill(IDX_FX,_cam.from.x);}
	afx_msg void fykill() {kill(IDX_FY,_cam.from.y);}
	afx_msg void fzkill() {kill(IDX_FZ,_cam.from.z);}
	afx_msg void txkill() {kill(IDX_TX,_cam.to.x);}
	afx_msg void tykill() {kill(IDX_TY,_cam.to.y);}
	afx_msg void tzkill() {kill(IDX_TZ,_cam.to.z);}
	afx_msg void twistkill() {kill(IDX_TWIST,_far_plane);}
	afx_msg void focalkill() {kill(IDX_FOCAL,_near_plane);}
	afx_msg void zoomkill() {kill(IDX_ZOOM,_cam.zoom);}
	DECLARE_MESSAGE_MAP()

	void set_dlg_item_float_to_int(int nID, float f);
	bool get_value(int nID,float &val);
	bool _valid_edit[NUM_EDIT];
	bool _is_valid;
	static int id[NUM_EDIT];
	view_wnd_t *_view_wnd;
	camera_t _cam;
	float _near_plane;
	float _far_plane;
};
#endif

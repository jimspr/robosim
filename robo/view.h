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
#define IDX_NEAR 6
#define IDX_FAR 7
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

	static constexpr float elev_incr = 5.0f;
	static constexpr float rot_incr = 5.0f;
	static constexpr float dist_incr = 5.0f;
	static constexpr float fx_incr = 5.0f;
	static constexpr float fy_incr = 5.0f;
	static constexpr float fz_incr = 5.0f;
	static constexpr float tx_incr = 5.0f;
	static constexpr float ty_incr = 5.0f;
	static constexpr float tz_incr = 5.0f;
	static constexpr float far_incr = 5.0f;
	static constexpr float near_incr = 5.0f;
	static constexpr float zoom_incr = 5.0f;

	afx_msg void elev_kill() { kill(IDX_FX, _cam.from.x); }
	afx_msg void rot_kill() { kill(IDX_FY, _cam.from.y); }
	afx_msg void dist_kill() { kill(IDX_FZ, _cam.from.z); }
	afx_msg void fxkill() {kill(IDX_FX,_cam.from.x);}
	afx_msg void fykill() {kill(IDX_FY,_cam.from.y);}
	afx_msg void fzkill() {kill(IDX_FZ,_cam.from.z);}
	afx_msg void txkill() {kill(IDX_TX,_cam.to.x);}
	afx_msg void tykill() {kill(IDX_TY,_cam.to.y);}
	afx_msg void tzkill() {kill(IDX_TZ,_cam.to.z);}
	afx_msg void farkill() {kill(IDX_FAR,_far_plane);}
	afx_msg void nearkill() {kill(IDX_NEAR,_near_plane);}
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
public:
	afx_msg void on_delta_elev(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_rot(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_dist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_fx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_fy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_fz(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_tx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_ty(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_tz(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_far(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_near(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void on_delta_zoom(NMHDR* pNMHDR, LRESULT* pResult);
};
#endif

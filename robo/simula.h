#pragma once
#ifndef __SIMULA_H
#define __SIMULA_H

#include "matrix.h"
#include "question.h"

class simulation_t;
class simob_t;

struct camera_t
{
	point3d_t from{ 100.F,100.F,100.F };
	point3d_t to{ 0.F,0.F,0.F };
	float focal_length{ 50.f };
	float twist{ 0.f };
	float zoom{ 100.f };
	float eyeoff{ 0.f };
};

struct ambient_light_t
{
	int _is_enabled = 1;
	GLfloat light[4]{.2f, .2f, .2f, 1.0f};
};

class light_t
{
public:
	light_t();
	int _is_enabled;
	GLfloat _ambient_properties[4];
	GLfloat _diffuse_properties[4];
	GLfloat _specular_properties[4];
	GLfloat constant_att;
	GLfloat linear_att;
	GLfloat quadratic_att;
	int _is_spot_enabled;
	GLfloat _spotlight_direction[3];
	GLfloat _spotlight_exponent;
	GLfloat _spotlight_cutoff;
	GLfloat _position[4];
	void turn_on(GLenum id);
	void set_pos(GLenum id) { glLightfv(id, GL_POSITION, _position); }
};



class view_dialog_t;
class view_wnd_t : public CWnd
{
public:
	ambient_light_t _ambient_light;
	std::vector<light_t> _lights;
	float _near_plane;
	float _far_plane;
	camera_t _main_camera_view;
	std::unique_ptr<view_dialog_t> _view_dialog;

protected:
	static std::string _class_name;
	static COLORREF _colorref_back;

	CPalette _palette;
	simulation_t& sim;
	bool _wire_frame;
	bool _lighting;
	bool _cull;
	bool _smooth_shading;

public:
	view_wnd_t(simulation_t& rs);
	virtual ~view_wnd_t();
	virtual void set_camera(camera_t& cam);
	virtual int paint_printer(CDC& dc,int page);
	virtual void paint_window(CRect& rect);

	bool setup_pixel_format(HDC hDC);
	bool setup_pixel_format_for_bitmap(HDC hDC);
	unsigned char component_for_index(int i, UINT nbits, UINT shift);
	void create_rgb_palette(HDC hDC);
	void set_look_from(const point3d_t& pt);
	void set_look_at(const point3d_t& pt);
	void SetCam(float f,float t,float z);
	void set_clip_plane(float near_plane, float far_plane);
	camera_t& get_camera();
	void redraw(bool bErase);
	void set_double_buffer(bool b);
	void set_wire_frame(bool b) {_wire_frame = b;}
	void set_lighting(bool b) {_lighting = b;}
	void set_cull(bool b) { _cull = b;}
	void set_smooth_shading(bool b) {_smooth_shading = b;}
	static void set_background_color(COLORREF cr);
	void adjust_lights();
	void copy_to_clipboard();

	virtual BOOL Create(CFrameWnd* p);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	//{{AFX_MSG(CViewWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/* Base class for a simulation. */
class simulation_t
{
protected:
// simulation time
// actual time
// status window
// time resolution
// spatial resolution
	bool _is_collision_on = false;
	bool _update = true;
public:
	bool show_frames = false;
	std::vector<simob_t*> _objects;
	view_wnd_t* _view_wnd = nullptr;

	void redraw(bool erase);
	bool add_object(simob_t *psobj);
	bool remove_object(simob_t *psobj,bool delete_views=true);
	bool remove_all_objects();
	void set_collision_check(bool b) {_is_collision_on = b;}
	bool check_collision();
	void reset_simulation();
	void set_update(bool b);
	void draw();

	bool set_look_from(const point3d_t& from);
	bool get_look_from(point3d_t& from);
	bool set_look_at(const point3d_t& from);
	bool get_look_at(point3d_t& at);
	bool set_camera(float focal, float twist, float zoom);
	bool get_camera(float& focal, float& twist, float& zoom);
	bool set_clip_plane(float near_plane, float far_plane);
	bool get_light(int index, light_t& light);
	bool set_light(int index, const light_t& light);

	bool adjust_lights();
	bool enable_solid(bool solid);
	bool enable_lighting(bool lighting);
	bool enable_cull(bool cull);
	bool enable_smooth_shading(bool smooth);

	virtual bool copy_to_clibboard() = 0;
	virtual void mark_in_use() = 0;
	virtual void new_simulation() = 0;
	virtual bool display_help(const std::string& file) = 0;
	virtual bool display_help(const std::string& file, const std::string& topic) = 0;
	virtual void print() = 0;
	virtual void set_font_dialog() = 0;
	virtual void show_about(void) = 0;
	virtual void show_view_dialog() = 0;
	virtual void set_lisp_menu(cons_t* cons) = 0;
	virtual void set_console_height(int lines) = 0;
	virtual void maximize_window(bool max) = 0;
	virtual node_t* ask(function* pfn, std::vector<question>& pq, const char* title) = 0;
};

extern simulation_t& get_sim();

#endif

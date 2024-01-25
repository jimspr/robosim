#pragma once
#ifndef __SIMULA_H
#define __SIMULA_H

#include "matrix.h"
#include "question.h"

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

class view_options_t;

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
	bool _show_frames = false;
	std::vector<simob_t*> _objects;
	view_options_t* _view_wnd = nullptr;

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
	virtual node_t* ask(function_t* pfn, std::vector<question>& pq, const char* title) = 0;
};

extern simulation_t& get_sim();

class view_options_t
{
public:
	ambient_light_t _ambient_light;
	std::vector<light_t> _lights;
	float _near_plane = 50.f;
	float _far_plane = 4000.f;
	camera_t _main_camera_view;

	bool _wire_frame = true;
	bool _lighting = false;
	bool _cull = false;
	bool _smooth_shading = false;

	virtual ~view_options_t()
	{
	}

	virtual void invalidate(bool erase = true) = 0;
	virtual void update_window() = 0;
	virtual void adjust_lights() = 0;
	virtual void update_data() = 0;

	void set_clip_plane(float near_plane, float far_plane)
	{
		_near_plane = near_plane;
		_far_plane = far_plane;
		update_data();
		invalidate();
	}
	void set_cam(float f, float t, float z)
	{
		_main_camera_view.focal_length = f;
		_main_camera_view.twist = t;
		_main_camera_view.zoom = z;
		set_camera(_main_camera_view);
		update_data();
	}

	void redraw(bool erase = true)
	{
		invalidate(erase);
		update_window();
	}

	void set_camera(camera_t& cam)
	{
		_main_camera_view = cam;
		invalidate();
	}

	camera_t& get_camera()
	{
		return _main_camera_view;
	}

	void set_look_from(const point3d_t& pt)
	{
		_main_camera_view.from = pt;
		set_camera(_main_camera_view);
		update_data();
	}
	void set_look_at(const point3d_t& pt)
	{
		_main_camera_view.to = pt;
		set_camera(_main_camera_view);
		update_data();
	}
};

#endif

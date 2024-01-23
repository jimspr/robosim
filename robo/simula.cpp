#include "stdafx.h"
#include "resource.h"
#include "simula.h"
#include "grob.h"
#include "lisp_engine.h"

using namespace std;

/**************************************************************
***************************************************************
********************  simulation_t ****************************
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
		obj->draw();
}

void simulation_t::redraw(bool bErase)
{
	MSG keymsg;
	if (PeekMessage(&keymsg, NULL, WM_KEYDOWN, WM_KEYDOWN, PM_NOREMOVE))
		if (AfxMessageBox("Do you want to stop?", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDYES)
			throw_interrupt_exception();
	if (!_update)
		return;
	if (_view_wnd != nullptr)
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
				throw_other_exception(lisp_engine._package.get_symbol("COLLISION"), nil); // out of reach exception
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
	_view_wnd->update_window();
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
	_view_wnd->set_look_at(at);
	_view_wnd->update_window();
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
	_view_wnd->set_cam(focal, twist, zoom);
	_view_wnd->update_window();
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
	_view_wnd->invalidate();
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
	_view_wnd->_wire_frame = !solid;
	_view_wnd->invalidate();
	return true;
}

bool simulation_t::enable_lighting(bool lighting)
{
	if (!_view_wnd)
		return nil;
	_view_wnd->_lighting = lighting;
	_view_wnd->invalidate();
	return true;
}

bool simulation_t::enable_cull(bool cull)
{
	if (!_view_wnd)
		return nil;
	_view_wnd->_cull = cull;
	_view_wnd->invalidate();
	return true;

}

bool simulation_t::enable_smooth_shading(bool smooth)
{
	if (!_view_wnd)
		return nil;
	_view_wnd->_smooth_shading = smooth;
	_view_wnd->invalidate();
	return true;
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

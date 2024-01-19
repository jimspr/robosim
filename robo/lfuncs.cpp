#include "stdafx.h"
#include <strstream>
#include <float.h>
#include "resource.h"
#include "mainwin.h"
#include "lispwin.h"
#include "dialogs.h"
#include "package.h"
#include "grob.h"
#include "mainwin.h"
#include "node.h"

extern symbol_t* key_optional;
extern symbol_t* key_rest;
extern symbol_t* key_key;
extern symbol_t* key_allow_other_keys;
extern symbol_t* key_aux;
extern symbol_t* key_body;
extern symbol_t* key_whole;
extern symbol_t* key_environment;
extern symbol_t* key_int;
extern symbol_t* key_float;
extern symbol_t* key_string;
extern symbol_t* key_symbol;
extern symbol_t* key_color;
extern symbol_t* key_relative_to;
extern symbol_t* key_classic;
extern symbol_t* key_min;
extern symbol_t* key_max;
extern symbol_t* key_position;
extern symbol_t* key_ambient;
extern symbol_t* key_diffuse;
extern symbol_t* key_specular;
extern symbol_t* key_shininess;

using namespace std;

static int deffac = 30;

static node_t *func_check(int numargs,node_t **args);
static node_t *func_make__primitive(int numargs,node_t **args);
static node_t *func_set__window__menu(int numargs,node_t **args);
static node_t *func_exit(int numargs,node_t **args);
static node_t *func_clear__simulation(int,node_t **);
static node_t *func_get__file__dialog(int,node_t **);
static node_t *func_printer__dialog(int,node_t **);
static node_t *func_new__view(int,node_t **);
static node_t *func_new__view3d(int,node_t **);
static node_t *func_set__view__dialog(int,node_t **);
static node_t *func_set__font__dialog(int,node_t **);
static node_t *func_cascade__windows(int,node_t **);
static node_t *func_tile__windows__horizontal(int,node_t **);
static node_t *func_tile__windows__vertical(int,node_t **);
static node_t *func_maximize__active__window(int,node_t **);
static node_t *func_arrange__icons(int,node_t **);
static node_t *func_about__dialog(int,node_t **);
static node_t *func_use__objects(int numargs,node_t **args);
static node_t *func_remove__objects(int numargs,node_t **args);
static node_t *func_make__box(int numargs,node_t **args);
static node_t *func_make__cylinder(int numargs,node_t **args);
static node_t *func_make__cone(int numargs,node_t **args);
static node_t *func_make__truncated__cone(int numargs,node_t **args);
static node_t *func_make__dome(int numargs,node_t **args);
static node_t *func_make__sphere(int numargs,node_t **args);
static node_t *func_make__circle(int numargs,node_t **args);
static node_t *func_matmult(int numargs,node_t **args);
static node_t *func_make__extrude__surface(int numargs,node_t **args);
static node_t *func_make__revolve__surface(int numargs,node_t **args);
static node_t *func_rotatex(int numargs,node_t **args);
static node_t *func_rotatey(int numargs,node_t **args);
static node_t *func_rotatez(int numargs,node_t **args);
static node_t *func_euler(int numargs,node_t **args);
static node_t *func_rpy(int numargs,node_t **args);
static node_t *func_translate(int numargs,node_t **args);
static node_t *func_scale(int numargs, node_t **args);
static node_t *func_set__object(int numargs,node_t **args);
static node_t *func_dh(int numargs,node_t **args);
static node_t *func_get__dh(int numargs,node_t **args);
static node_t *func_add__objects(int numargs,node_t **args);
static node_t *func_copy__object(int numargs,node_t **args);
static node_t *func_make__fixed__link(int numargs,node_t **args);
static node_t *func_make__revolute__link(int numargs,node_t **args);
static node_t *func_make__prismatic__link(int numargs,node_t **args);
static node_t *func_set__double__buffer(int numargs,node_t **args);
static node_t *func_set__background__color(int numargs,node_t **args);
static node_t *func_set__command__lines(int numargs,node_t **args);
static node_t *func_invert__transform(int numargs,node_t **args);
static node_t *func_ask__dialog(int numargs,node_t **args);
static node_t *func_message__box(int numargs,node_t **args);
static node_t *func_check__stream(int numargs,node_t **args);
static node_t *func_show__frames(int numargs,node_t **args);
static node_t *func_get__child(int numargs,node_t **args);
static node_t *func_check__collision(int numargs,node_t **args);
static node_t *func_make__serial__agent(int numargs,node_t **args);
static node_t *func_make__parallel__agent(int numargs,node_t **args);
static node_t *func_set__agent__joints(int numargs,node_t **args);
static node_t *func_get__agent__joints(int numargs,node_t **args);
static node_t *func_set__position(int numargs,node_t **args);
static node_t *func_offset__position(int numargs,node_t **args);
static node_t *func_get__position(int numargs,node_t **args);
static node_t *func_grasp(int numargs,node_t **args);
static node_t *func_release(int numargs,node_t **args);
static node_t *func_set__num__facets(int numargs,node_t **args);
static node_t *func_get__num__facets(int numargs,node_t **args);
static node_t *func_set__look__from(int numargs,node_t **args);
static node_t *func_set__look__at(int numargs,node_t **args);
static node_t *func_set__camera(int numargs,node_t **args);
static node_t *func_get__look__from(int numargs,node_t **args);
static node_t *func_get__look__at(int numargs,node_t **args);
static node_t *func_get__camera(int numargs,node_t **args);
static node_t *func_object__in__use__p(int numargs,node_t **args);
static node_t *func_drive__agent(int numargs,node_t **args);
static node_t *func_get__solution(int numargs,node_t **args);
static node_t *func_get__solutions(int numargs,node_t **args);
static node_t *func_set__collision__check(int numargs,node_t **args);
static node_t *func_get__num__solutions(int numargs,node_t **args);
static node_t *func_move__straight__to(int numargs,node_t **args);
static node_t *func_move__inter__to(int numargs,node_t **args);
static node_t *func_set__update(int numargs,node_t **args);
static node_t *func_maximize(int numargs,node_t **args);
static node_t *func_append__objects(int numargs,node_t **args);

//static node_t *func_make__gripper(int numargs,node_t **args);
//static node_t *func_attach__object(int numargs,node_t **args);
//static node_t *func_set__agent__speed(int numargs,node_t **args);
//static node_t *func_set__agent__ik_func(int numargs,node_t **args);
//static node_t *func_get__agent__ik_status(int numargs,node_t **args);

static node_t *func_transformp(int numargs,node_t **args);
static node_t *func_simobp(int numargs,node_t **args);
static node_t *func_copy__window(int numargs,node_t **args);
static node_t *func_use__kinematics(int numargs,node_t **args);
static node_t *func_use__kinematics__lisp(int numargs,node_t **args);
static node_t *func_make__composite(int numargs,node_t **args);

static node_t *func_set__eye__separation(int numargs, node_t **args);
static node_t *func_display__help(int numargs, node_t **args);
static node_t *func_winexec(int numargs, node_t** args);

static node_t *func_tref(int numargs,node_t **args);

// light model funcs
static node_t *func_adjust__lights(int numargs, node_t** args);
static node_t *func_set__light(int numargs, node_t** args);
static node_t *func_set__clipplane(int numargs, node_t** args);
static node_t *func_enable__solid(int numags, node_t** args);
static node_t *func_enable__lighting(int numags, node_t** args);
static node_t *func_enable__cull(int numags, node_t** args);
static node_t *func_enable__smooth__shading(int numargs, node_t** args);

static void parmsub(simob_t *ps,cons_t *base);
static node_t* get_parmsub(simob_t *ps);
static void CheckMakeArgs(int numargs,node_t **args);
static void make_point_from_args(node_t **args,point3d_t &pt);

#define func(n,f,min,max) current_package->get_symbol(n)->set_form(new sysfunction_t(n,f,min,max))

void init_lfuncs(void)
{
	func("MAKE-BOX",	func_make__box,		3,3);
	func("MAKE-CIRCLE",	func_make__circle,	1,2);
	func("MAKE-CYLINDER",	func_make__cylinder,	2,3);
	func("MAKE-CONE",	func_make__cone,	2,3);
	func("MAKE-TRUNCATED-CONE",	func_make__truncated__cone,	3,4);
	func("MAKE-SPHERE",	func_make__sphere,	1,2);
	func("MAKE-DOME",	func_make__dome,	1,2);
	func("MAKE-EXTRUDE-SURFACE",func_make__extrude__surface,2,2);
	func("MAKE-REVOLVE-SURFACE",func_make__revolve__surface,1,2);
	func("CHECK",		func_check,		0,0);
	func("MAKE-PRIMITIVE",	func_make__primitive,	1,-1);
	func("SET-WINDOW-MENU",	func_set__window__menu,	1,2);
	func("CLEAR-SIMULATION",func_clear__simulation,	0,0);
	func("GET-FILE-DIALOG", func_get__file__dialog,	0,2);
	func("PRINTER-DIALOG",	func_printer__dialog,	0,0);
	func("NEW-VIEW",	func_new__view,		0,0);
	func("NEW-VIEW3D",	func_new__view3d,	0,1);
	func("SET-VIEW-DIALOG",	func_set__view__dialog,	0,0);
	func("SET-FONT-DIALOG",	func_set__font__dialog,	0,0);
	func("CASCADE-WINDOWS",	func_cascade__windows,	0,0);
	func("TILE-WINDOWS-H",	func_tile__windows__horizontal,0,0);
	func("TILE-WINDOWS-V",	func_tile__windows__vertical,0,0);
	func("ARRANGE-ICONS",	func_arrange__icons,		0,0);
	func("ABOUT-DIALOG",	func_about__dialog,		0,0);
	func("EXIT",		func_exit,		0,0);
	func("USE-OBJECTS",	func_use__objects,	1,-1);
	func("REMOVE-OBJECTS",	func_remove__objects,	0,-1);
	func("MATMULT",		func_matmult,		0,-1);
	func("TRANSLATE",	func_translate,		3,3);
	func("SCALE",		func_scale,			3,3);
	func("ROTATEX",		func_rotatex,		1,1);
	func("ROTATEY",		func_rotatey,		1,1);
	func("ROTATEZ",		func_rotatez,		1,1);
	func("SET-OBJECT",	func_set__object,	1,-1);
	func("EULER",		func_euler,		3,3);
	func("RPY",		func_rpy,		3,3);
	func("D-H",		func_dh,		4,4);
	func("GET-D-H",		func_get__dh,		1,-1);
	func("ADD-OBJECTS",	func_add__objects,	1,-1);
	func("COPY-OBJECT",	func_copy__object,	1,1);
	func("MAKE-FIXED-LINK",	func_make__fixed__link,	0,-1);
	func("MAKE-REVOLUTE-LINK",func_make__revolute__link,0,-1);
	func("MAKE-PRISMATIC-LINK",func_make__prismatic__link,0,-1);
	func("MAKE-SERIAL-AGENT",	func_make__serial__agent,	1,-1);
	func("MAKE-PARALLEL-AGENT",	func_make__parallel__agent,	0,-1);
	func("MAXIMIZE-ACTIVE-WINDOW",func_maximize__active__window,0,0);
	func("SET-DOUBLE-BUFFER",func_set__double__buffer,1,1);
	func("SET-BACKGROUND-COLOR",func_set__background__color,1,1);
	func("SET-COMMAND-LINES",func_set__command__lines,1,1);
	func("INVERT-TRANSFORM",func_invert__transform,1,1);
	func("SET-AGENT-JOINTS",func_set__agent__joints,2,2);
	func("GET-AGENT-JOINTS",func_get__agent__joints,1,1);
	func("ASK-DIALOG",	func_ask__dialog,		3,-1);
	func("MESSAGE-BOX",	func_message__box,		1,-1);
	func("CHECK-STREAM",	func_check__stream,	1,1);
	func("CHECK-COLLISION", func_check__collision,		0,-1);
	func("SET-POSITION",	func_set__position,		2,4);
	func("OFFSET-POSITION",	func_offset__position,		2,4);
	func("GET-POSITION",	func_get__position,		1,3);
	func("GRASP",		func_grasp,			2,2);
	func("RELEASE",		func_release,			1,1);
	func("APPEND-OBJECTS",	func_append__objects,		1,-1);
	func("SET-NUM-FACETS",	func_set__num__facets,		1,1);
	func("GET-NUM-FACETS",	func_get__num__facets,		0,0);
	func("SET-LOOK-FROM",	func_set__look__from,		3,3);
	func("SET-LOOK-AT",	func_set__look__at,		3,3);
	func("SET-CAMERA",	func_set__camera,		3,3);
	func("GET-LOOK-FROM",	func_get__look__from,		0,0);
	func("GET-LOOK-AT",	func_get__look__at,		0,0);
	func("GET-CAMERA",	func_get__camera,		0,0);
	func("OBJECT-IN-USE-P",	func_object__in__use__p,	1,1);
	func("DRIVE-AGENT",	func_drive__agent,		3,3);
	func("SHOW-FRAMES",	func_show__frames,		1,1);
	func("GET-CHILD",	func_get__child,		1,1);
	func("GET-SOLUTION",	func_get__solution,		3,3);
	func("GET-SOLUTIONS",	func_get__solutions,		2,2);
	func("SET-COLLISION-CHECK",func_set__collision__check,	1,1);
	func("GET-NUM-SOLUTIONS",func_get__num__solutions,	1,1);
	func("MOVE-STRAIGHT-TO",func_move__straight__to,	4,4);
	func("MOVE-INTER-TO",	func_move__inter__to,		4,4);
	func("SET-UPDATE",	func_set__update,		1,1);
	func("MAXIMIZE",	func_maximize,			1,1);

	func("TRANSFORMP",	func_transformp,		1,1);
	func("SIMOBP",		func_simobp,			1,1);
	func("COPY-WINDOW",	func_copy__window,		1,1);
	func("USE-KINEMATICS",	func_use__kinematics,		1,-1);
	func("USE-KINEMATICS-LISP",	func_use__kinematics__lisp,	4,4);
	func("MAKE-COMPOSITE",	func_make__fixed__link,		0,-1);
	func("SET-EYE-SEPARATION",func_set__eye__separation,	1,1);

	func("DISPLAY-HELP",	func_display__help,				1,1);
	func("WINEXEC",			func_winexec,					1,-1);
	func("TREF",			func_tref,						3,3);

	func("ADJUST-LIGHTS",	func_adjust__lights,				0,0);
	func("SET-LIGHT",		func_set__light,					2,10);
	func("SET-CLIPPLANE",	func_set__clipplane,				2,2);
	func("ENABLE-SOLID",	func_enable__solid,					1,1);
	func("ENABLE-LIGHTING",	func_enable__lighting,				1,1);
	func("ENABLE-CULL",		func_enable__cull,					1,1);
	func("ENABLE-SMOOTH-SHADING", func_enable__smooth__shading,	1,1);
}

///////////////////////////////////////////////////////////////////////////////
// Helpers

static void ParseFloatList1(float* pf, cons_t* p, float fmin, float fmax)
{
	p->check_arg_type(TYPE_CONS);
	if (p->get_num_items() != 3)
		throw_eval_exception(BAD_ARG_TYPE);
	float f1 = *p->Car()->as<number_node_t>();
	if (f1<fmin || f1>fmax)
		throw_eval_exception(DOMAIN_ERROR);
	pf[0] = f1;
}

static void ParseFloatList(float* pf, cons_t *p, float fmin, float fmax)
{
	p->check_arg_type(TYPE_CONS);
	if (p->get_num_items() != 3)
		throw_eval_exception(BAD_ARG_TYPE);
	float f1 = *p->Car()->as<number_node_t>();
	p = p->CdrCONS();
	float f2 = *p->Car()->as<number_node_t>();
	p = p->CdrCONS();
	float f3 = *p->Car()->as<number_node_t>();
	if (f1<fmin || f1>fmax || f2<fmin || f2>fmax || f3<fmin || f3>fmax)
		throw_eval_exception(DOMAIN_ERROR);
	pf[0] = f1;
	pf[1] = f2;
	pf[2] = f3;
}

static void ParseFloatList(float* pf, cons_t *p)
{
	p->check_arg_type(TYPE_CONS);
	if (p->get_num_items() != 3)
		throw_eval_exception(BAD_ARG_TYPE);
	float f1 = *p->Car()->as<number_node_t>();
	p = p->CdrCONS();
	float f2 = *p->Car()->as<number_node_t>();
	p = p->CdrCONS();
	float f3 = *p->Car()->as<number_node_t>();
	pf[0] = f1;
	pf[1] = f2;
	pf[2] = f3;
}

///////////////////////////////////////////////////////////////////////////////

static node_t *func_exit(int,node_t **)
{
	AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
	return pTrue;
}

static node_t *func_check(int,node_t **)
{
	return AfxCheckMemory() ? pTrue : nil;
}

static node_t *func_make__primitive(int numargs,node_t **args)
{
	int numvec = 0,i;
	args[0]->check_arg_type(TYPE_CONS);
	for (i=0;i<numargs;i++)
	{
		cons_t *veclist = (cons_t *)args[i];
		while (veclist->is_a(TYPE_CONS))
		{
			veclist->Car()->check_arg_type(TYPE_VECTOR);
			numvec++;
			veclist = veclist->CdrCONS();
		}
	}
	if (!numargs || !numvec)
		throw_eval_exception(INVALID_GRAPHIC_PRIMITIVE);
	simob_t *pg = new simob_t();
	pg->init(numvec,numargs);
	for (i=0;i<numargs;i++)
	{
		numvec=0;
		cons_t *veclist = (cons_t *)args[i];
		while (veclist->is_a(TYPE_CONS))
		{
			pg->add_vector(*(vector_t *)veclist->Car());
			numvec++;
			veclist = veclist->CdrCONS();
		}
		pg->new_poly(numvec);
	}
	pg->calc_bounding_box();
	return pg;
}

static node_t *func_set__window__menu(int numargs,node_t **args)
{
	int id=0;
	args[0]->check_arg_type(TYPE_CONS);
	get_sim().set_lisp_menu((cons_t*)args[0]);
	return pTrue;
}

void main_win_mark_in_use(void)
{
	get_sim().mark_in_use();
}

static node_t *func_clear__simulation(int,node_t **)
{
	get_sim().new_simulation();
	return pTrue;
}

static node_t *func_get__file__dialog(int numargs,node_t **args)
{
	const char *defext = "All Files (*.*)|*.*|";
	const char *filter = "All Files (*.*)|*.*|";
	if (numargs == 1)
		throw_eval_exception(TOO_FEW_ARGS);
	if (numargs)
	{
		args[0]->check_arg_type(TYPE_STRING);
		defext = ((string_node_t*)args[0])->data();
		args[1]->check_arg_type(TYPE_STRING);
		filter = ((string_node_t*)args[1])->data();
	}
	CFileDialog fd(TRUE,defext,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);

	if (fd.DoModal() == IDCANCEL)
		return nil;
	return new string_node_t(fd.GetPathName());
}

static node_t *func_printer__dialog(int,node_t **)
{
	get_sim().print();
	return pTrue;
}

static node_t *func_new__view(int,node_t **)
{
	return nil;
}

static node_t *func_new__view3d(int numargs,node_t **args)
{
	return nil;
}

static node_t *func_set__view__dialog(int,node_t **)
{
	get_sim().show_view_dialog();
	return pTrue;
}

static node_t *func_set__font__dialog(int,node_t **)
{
	get_sim().set_font_dialog();
	return pTrue;
}

static node_t *func_cascade__windows(int,node_t **)
{
	return nil;
}

static node_t *func_tile__windows__horizontal(int,node_t **)
{
	return nil;
}

static node_t *func_tile__windows__vertical(int,node_t **)
{
	return nil;
}

static node_t *func_arrange__icons(int,node_t **)
{
	return nil;
}

static node_t *func_about__dialog(int,node_t **)
{
	get_sim().show_about();
	return pTrue;
}

static node_t *func_use__objects(int numargs,node_t **args)
{
	int i;
	for (i=0;i<numargs;i++)
	{
		simob_t *ps = (simob_t *)args[i];
		ps->check_arg_type(TYPE_SIMOB);
		if (!ps->is_in_env())
			get_sim().add_object(ps);
	}
	get_sim().redraw(false);
	return pTrue;
}

static node_t *func_remove__objects(int numargs,node_t **args)
{
	int i;
	bool b = false;
	if (!numargs)
		b = get_sim().remove_all_objects();
	for (i=0;i<numargs;i++)
	{
		simob_t *ps = (simob_t *)args[0];
		ps->check_arg_type(TYPE_SIMOB);
		b |= get_sim().remove_object(ps);
	}
	if (b)
	{
		get_sim().redraw(true);
		return pTrue;
	}
	return nil;
}

static void CheckMakeArgs(int numargs,node_t **args)
{
	int i;
	for (i=0;i<numargs;i++)
	{
		args[i]->check_number();
		if ((float)(* (number_node_t *) args[i]) <= 0.)
			throw_eval_exception(ARG_SHOULD_BE_GREATER_THAN_ZERO);
	}
}

static node_t *func_make__box(int numargs,node_t **args)
{
	CheckMakeArgs(numargs,args);
	simob_t *ps = new simob_t();
	float x = (float)(* (number_node_t *) args[0]);
	float y = (float)(* (number_node_t *) args[1]);
	float z = (float)(* (number_node_t *) args[2]);
	ps->make_box(x,y,z);
	return ps;
}

static node_t *func_make__cylinder(int numargs,node_t **args)
{
	float r,h;
	int n = deffac;
	CheckMakeArgs(numargs,args);
	simob_t *ps = new simob_t();
	r = (float)(* (number_node_t *) args[0]);
	h = (float)(* (number_node_t *) args[1]);
	if (numargs == 3)
		n = (long)(* (number_node_t *) args[2]);
	if (n <= 0.)
		throw_eval_exception(ARG_SHOULD_BE_GREATER_THAN_ZERO);
	ps->make_closedcyl(r,h,n);
	return ps;
}

static node_t *func_make__circle(int numargs,node_t **args)
{
	float r;
	int n = deffac;
	CheckMakeArgs(numargs,args);
	simob_t *ps = new simob_t();
	r = (float)(* (number_node_t *) args[0]);
	if (numargs == 2)
		n = (long)(* (number_node_t *) args[1]);
	if (n <= 0.)
		throw_eval_exception(ARG_SHOULD_BE_GREATER_THAN_ZERO);
	ps->make_circle(r,n);
	return ps;
}

static node_t *func_make__cone(int numargs,node_t **args)
{
	float r,h;
	int n = deffac;
	CheckMakeArgs(numargs,args);
	simob_t *ps = new simob_t();
	r = (float)(* (number_node_t *) args[0]);
	h = (float)(* (number_node_t *) args[1]);
	if (numargs == 3)
		n = (long)(* (number_node_t *) args[2]);
	if (n <= 0.)
		throw_eval_exception(ARG_SHOULD_BE_GREATER_THAN_ZERO);
	ps->make_closedcone(r,h,n);
	return ps;
}

static node_t *func_make__truncated__cone(int numargs,node_t **args)
{
	float ru,rl,h;
	int n = deffac;
	CheckMakeArgs(numargs,args);
	simob_t *ps = new simob_t();
	ru = (float)(* (number_node_t *) args[0]);
	rl = (float)(* (number_node_t *) args[1]);
	h = (float)(* (number_node_t *) args[2]);
	if (numargs == 4)
		n = (long)(* (number_node_t *) args[3]);
	if (n <= 0.)
		throw_eval_exception(ARG_SHOULD_BE_GREATER_THAN_ZERO);
	ps->make_truncated_closedcone(ru,rl,h,n);
	return ps;
}

static node_t *func_make__sphere(int numargs,node_t **args)
{
	float r;
	int n = deffac;
	CheckMakeArgs(numargs,args);
	simob_t *ps = new simob_t();
	r = (float)(* (number_node_t *) args[0]);
	if (numargs == 2)
		n = (long)(* (number_node_t *) args[1]);
	if (n <= 0.)
		throw_eval_exception(ARG_SHOULD_BE_GREATER_THAN_ZERO);
	ps->make_sphere(r,n);
	return ps;
}

static node_t *func_make__dome(int numargs,node_t **args)
{
	float r;
	int n = deffac;
	CheckMakeArgs(numargs,args);
	simob_t *ps = new simob_t();
	r = (float)(* (number_node_t *) args[0]);
	if (numargs == 2)
		n = (long)(* (number_node_t *) args[1]);
	if (n <= 0.)
		throw_eval_exception(ARG_SHOULD_BE_GREATER_THAN_ZERO);
	ps->make_closeddome(r,n);
	return ps;
}

static node_t *func_matmult(int numargs,node_t **args)
{
	mat44 *res = new mat44();
	for (int i=0;i<numargs;i++)
	{
		args[i]->check_arg_type(TYPE_MAT44);
		res->postmultiply( * (mat44 *)args[i]);
	}
	return res;
}

static node_t *func_make__extrude__surface(int numargs,node_t **args)
{
	args[0]->check_arg_type(TYPE_SIMOB);
	args[1]->check_number();
	if ( ((simob_t *)args[0])->GetNumPoly() != 1)
		throw_eval_exception(BAD_ARG_TYPE);
	simob_t *pg = new simob_t();
	pg->make_extrusion((simob_t *)args[0],(float) *(number_node_t *)args[1]);
	return pg;
}

static node_t *func_make__revolve__surface(int numargs,node_t **args)
{
	int n = deffac;
	args[0]->check_arg_type(TYPE_SIMOB);
	args[1]->check_number();
	if ( ((simob_t *)args[0])->GetNumPoly() != 1)
		throw_eval_exception(BAD_ARG_TYPE);
	if (numargs == 2)
		n = (long)(* (number_node_t *) args[1]);
	if (n <= 0.)
		throw_eval_exception(ARG_SHOULD_BE_GREATER_THAN_ZERO);
	simob_t *pg = new simob_t();
	pg->make_revolution((simob_t *)args[0], n);
	return pg;
}

static node_t *func_rotatex(int numargs,node_t **args)
{
	auto pn = args[0]->as<number_node_t>();
	return new mat44(mat44::ROTATEX(DEG2RAD(*pn)));
}

static node_t *func_rotatey(int numargs,node_t **args)
{
	auto pn = args[0]->as<number_node_t>();
	return new mat44(mat44::ROTATEY(DEG2RAD(*pn)));
}

static node_t *func_rotatez(int numargs,node_t **args)
{
	auto pn = args[0]->as<number_node_t>();
	return new mat44(mat44::ROTATEZ(DEG2RAD(*pn)));
}

static node_t *func_translate(int numargs,node_t **args)
{
	auto px = args[0]->as<number_node_t>();
	auto py = args[1]->as<number_node_t>();
	auto pz = args[2]->as<number_node_t>();
	return new mat44(mat44::TRANS( (float)*px,(float)*py,(float)*pz));
}

static node_t *func_scale(int numargs, node_t **args)
{
	auto px = args[0]->as<number_node_t>();
	auto py = args[1]->as<number_node_t>();
	auto pz = args[2]->as<number_node_t>();
	return new mat44(mat44::SCALE( (float)*px, (float)*py, (float)*pz));
}

static COLORREF ParseColor(cons_t *p)
{
	p->check_arg_type(TYPE_CONS);
	if (p->get_num_items() != 3)
		throw_eval_exception(BAD_ARG_TYPE);
	int r = *p->Car()->as<number_node_t>();
	p = p->CdrCONS();
	int g = *p->Car()->as<number_node_t>();
	p = p->CdrCONS();
	int b = *p->Car()->as<number_node_t>();
	if (r<0 || r>255 || g<0 || g>255 || b<0 || b>255)
		throw_eval_exception(DOMAIN_ERROR);
	return RGB(r,g,b);
}

static node_t *func_set__object(int numargs,node_t **args)
{
	int i;
	auto ps = args[0]->as<simob_t>();
	if (numargs%2 == 0)
		throw_eval_exception(TOO_FEW_ARGS);
	for (i=1;i<numargs;i+=2)
	{
		args[i]->check_arg_type(TYPE_SYMBOL);
		GLfloat p[3];
		if (args[i] == key_color)
		{
			COLORREF cr = ParseColor((cons_t *)args[i+1]);
			p[0] = (float)GetRValue(cr)/255.f;
			p[1] = (float)GetGValue(cr)/255.f;
			p[2] = (float)GetBValue(cr)/255.f;
			ps->set_ambient(p);
		}
		else if (args[i] == key_ambient)
		{
			ParseFloatList(p, (cons_t *)args[i+1], -1.f, 1.f);
			ps->set_ambient(p);
		}
		else if (args[i] == key_diffuse)
		{
			ParseFloatList(p, (cons_t *)args[i+1], -1.f, 1.f);
			ps->set_diffuse(p);
		}
		else if (args[i] == key_specular)
		{
			ParseFloatList(p, (cons_t *)args[i+1], -1.f, 1.f);
			ps->set_specular(p);
		}
		else if (args[i] == key_shininess)
		{
			ParseFloatList1(p, (cons_t *)args[i+1], 0.f, 1000.f);
			ps->set_shininess(p);
		}
//		else if (args[i] == key_collision)
		else
			throw_eval_exception(BAD_ARG_TYPE);
	}
	if (numargs > 1)
		get_sim().redraw(true);
	return args[0];
}

static node_t *func_get__position(int numargs,node_t **args)
{
	if (numargs == 2)
		throw_eval_exception(TOO_FEW_ARGS);
	args[0]->check_arg_type(TYPE_SIMOB);
	if (numargs == 1)
		return new mat44( ((simob_t *)args[0])->GetPosition());
	if (args[1] != key_relative_to)
		throw_eval_exception(BAD_KEYWORD);
	args[2]->check_arg_type(TYPE_SIMOB);
	mat44 *pmat = new mat44( ((simob_t *)args[0])->GetPosition());
	pmat->premultiply( ((simob_t *)args[2])->GetPosition().fastinverse() );
	return pmat;
}

static node_t *func_set__position(int numargs,node_t **args)
{
	if (numargs == 3)
		throw_eval_exception(TOO_FEW_ARGS);
	auto ps = args[0]->as<simob_t>();
	mat44 tmat;
	if (args[1] != nil)
	{
		args[1]->check_arg_type(TYPE_MAT44);
		tmat =  *(mat44 *)args[1];
	}
	if (numargs == 4)
	{
		if (args[2] != key_relative_to)
			throw_eval_exception(BAD_KEYWORD);
		args[3]->check_arg_type(TYPE_SIMOB);
		tmat.premultiply( ((simob_t *)args[3])->GetPosition());
	}
	ps->SetPosition(tmat);
	get_sim().redraw(true);
	return args[0];
}

static node_t *func_offset__position(int numargs,node_t **args)
{
	if (numargs == 3)
		throw_eval_exception(TOO_FEW_ARGS);
	auto ps = args[0]->as<simob_t>();
	mat44 tmat;
	if (args[1] != nil)
	{
		args[1]->check_arg_type(TYPE_MAT44);
		tmat =  *(mat44 *)args[1];
	}
	if (numargs == 4)
	{
		if (args[2] != key_relative_to)
			throw_eval_exception(BAD_KEYWORD);
		args[3]->check_arg_type(TYPE_SIMOB);
		tmat.premultiply( ((simob_t *)args[3])->GetPosition());
		tmat.postmultiply( ((simob_t *)args[3])->GetPosition().fastinverse());
	}
	tmat.postmultiply(ps->GetPosition());
	ps->SetPosition(tmat);
	get_sim().redraw(true);
	return args[0];
}

static node_t *func_set__background__color(int numargs,node_t **args)
{
	COLORREF cr = ParseColor((cons_t *)args[0]);
	view_wnd_t::set_background_color(cr);
	get_sim().redraw(true);
	return pTrue;
}

static node_t *func_euler(int numargs,node_t **args)
{
	auto r1 = args[0]->as<number_node_t>();
	auto r2 = args[1]->as<number_node_t>();
	auto r3 = args[2]->as<number_node_t>();
	euler_zyz_t e{ DEG2RAD(*r1), DEG2RAD(*r2), DEG2RAD(*r3) };
	return new mat44(mat44::EULER(e));
}

static node_t *func_rpy(int numargs,node_t **args)
{
	auto r1 = args[0]->as<number_node_t>();
	auto r2 = args[1]->as<number_node_t>();
	auto r3 = args[2]->as<number_node_t>();
	euler_rpy_t rpy{ DEG2RAD(*r1), DEG2RAD(*r2), DEG2RAD(*r3) };
	return new mat44(mat44::RPY(rpy));
}

static node_t *func_dh(int numargs,node_t **args)
{
	auto theta = args[0]->as<number_node_t>();
	auto dz = args[1]->as<number_node_t>();
	auto da = args[2]->as<number_node_t>();
	auto alpha = args[3]->as<number_node_t>();
	return new mat44(mat44::DH(DEG2RAD(*theta),(float)*dz,(float)*da, DEG2RAD(*alpha)));
}

/* Get Denavit - Hartenberg parameters */
static node_t *func_get__dh(int numargs,node_t **args)
{
	dh_t dh;
	auto pmat = args[0]->as<mat44>();
	if (!pmat->get_dh(dh))
		return nil;
	return cons_t::make_list(
		new number_node_t(RAD2DEG(dh.theta)),
		new number_node_t(dh.dz),
		new number_node_t(dh.da),
		new number_node_t(RAD2DEG(dh.alpha)));
}

static node_t *func_copy__object(int numargs,node_t **args)
{
	args[0]->check_arg_type(TYPE_SIMOB);
	simob_t *ps = new simob_t(*(simob_t *)args[0]);
	return ps;
}

static node_t *func_add__objects(int numargs,node_t **args)
{
	int i;
	for (i=0;i<numargs;i++)
		if (args[i] != nil)
			args[i]->check_arg_type(TYPE_SIMOB);
	simob_t *ps = new simob_t();
	for (i=0;i<numargs;i++)
		if (args[i] != nil)
			ps->add_world(*(simob_t *)args[i]);
	return ps;
}

static node_t *func_append__objects(int numargs,node_t **args)
{
	int i;
	auto ps = args[0]->as<simob_t>();
	for (i=1;i<numargs;i++)
		if (args[i] != nil)
			args[i]->check_arg_type(TYPE_SIMOB);
	for (i=1;i<numargs;i++)
		if (args[i] != nil)
			ps->add_world(*(simob_t *)args[i]);
	return ps;
}

static node_t *func_make__fixed__link(int numargs,node_t **args)
{
	int i;
	mat44 tmat;
	if (numargs%2)
		throw_eval_exception(TOO_FEW_ARGS);
	for (i=0;i<numargs;i+=2)
	{
		args[i]->check_arg_type(TYPE_SIMOB);
		if (((simob_t *)args[i])->GetNumChildren())
			throw_eval_exception(NO_COMPLEX_IN_LINK);
		if (args[i+1] != nil)
			args[i+1]->check_arg_type(TYPE_MAT44);
	}
	simob_t *ps = new simob_t();
	for (i=0;i<numargs;i+=2)
	{
		simob_t t(*(simob_t *)args[i]);
		if (args[i+1] != nil)
		{
			tmat = t.GetPosition();
			t.SetPosition( (*(mat44 *)args[i+1]) * tmat);
			ps->add_world(t);
			t.SetPosition(tmat);
		}
		else
			ps->add_world(t);
	}
	return ps;
}

static node_t *func_make__revolute__link(int numargs,node_t **args)
{
	float fmin,fmax;
	if (args[0] == nil)
		fmin = -FLT_MAX;
	else
	{
		args[0]->check_number();
		fmin = (float)*(number_node_t *)args[0];
	}
	if (args[1] == nil)
		fmax = FLT_MAX;
	else
	{
		args[1]->check_number();
		fmax = (float)*(number_node_t *)args[1];
	}
	args += 2;
	numargs -= 2;

	simob_t *ps = (simob_t *)(func_make__fixed__link(numargs,args));
	ps->set_joint_func(mat44::ROTATEZDEG);
	ps->set_min_max(fmin,fmax);
	return ps;
}

static node_t *func_make__prismatic__link(int numargs,node_t **args)
{
	float fmin,fmax;
	if (args[0] == nil)
		fmin = -FLT_MAX;
	else
	{
		args[0]->check_number();
		fmin = (float)*(number_node_t *)args[0];
	}
	if (args[1] == nil)
		fmax = FLT_MAX;
	else
	{
		args[1]->check_number();
		fmax = (float)*(number_node_t *)args[1];
	}
	args += 2;
	numargs -= 2;

	simob_t *ps = (simob_t *)(func_make__fixed__link(numargs,args));
	ps->set_joint_func(mat44::TRANSZ);
	ps->set_min_max(fmin,fmax);
	return ps;
}

static void poshelp(simob_t *ps,node_t *pmat)
{
	if (pmat != nil)
	{
		mat44 posmat(ps->GetPosition());
		posmat.postmultiply(*(mat44 *)pmat);
		ps->SetPosition(posmat);
	}
}

static node_t *func_make__serial__agent(int numargs,node_t **args)
{
	int i;
	bool bClassic=false;
	if (args[0] == key_classic)
	{
		bClassic = true;
		args++;
		numargs--;
	}
	if (!numargs || numargs%2)
		throw_eval_exception(TOO_FEW_ARGS);
	for (i=0;i<numargs;i+=2)
	{
		args[i]->check_arg_type(TYPE_SIMOB);
		if (args[i+1] != nil)
			args[i+1]->check_arg_type(TYPE_MAT44);
	}
	simob_t *ps = new simob_t();
	simob_t *p = ps;
	for (i=0;i<numargs;i+=2)
	{
		p->set_classic(bClassic);
		p->Attach( (simob_t *)args[i]);
		p = (simob_t *)args[i];
		poshelp(p,args[i+1]);
	}
	ps->make_agent(p,numargs/2);
	return ps;
}

static node_t *func_make__parallel__agent(int numargs,node_t **args)
{
	int i;
	if (numargs==0)
		return nil;
	if (numargs%2)
		throw_eval_exception(TOO_FEW_ARGS);
	for (i=0;i<numargs;i+=2)
	{
		args[i]->check_arg_type(TYPE_SIMOB);
		if (args[i+1] != nil)
			args[i+1]->check_arg_type(TYPE_MAT44);
	}
	simob_t *ps = new simob_t();
	simob_t *p = (simob_t *)args[0];
	poshelp(p,args[1]);
	ps->Attach(p);
	for (i=2;i<numargs;i+=2)
	{
		p->Attach( (simob_t *)args[i]);
		poshelp((simob_t *)args[i],args[i+1]);
	}
	return ps;
}

static node_t *func_maximize__active__window(int,node_t **)
{
	return nil;
}

static node_t *func_set__double__buffer(int numargs,node_t **args)
{
	if (args[0]!=pTrue && args[0]!=nil)
		throw_eval_exception(BAD_ARG_TYPE);
	// Does nothing
	return args[0];
}

static node_t *func_set__command__lines(int numargs,node_t **args)
{
	int l = *args[0]->as<number_node_t>();
	if (l<0 || l>24)
		throw_eval_exception(DOMAIN_ERROR);
	get_sim().set_console_height(l);
	return args[0];
}

static node_t *func_invert__transform(int numargs,node_t **args)
{
	args[0]->check_arg_type(TYPE_MAT44);
	return new mat44( ((mat44 *)args[0])->fastinverse());
}

static node_t *func_set__agent__joints(int numargs,node_t **args)
{
	auto ps = args[0]->as<simob_t>();
	if (args[1] != nil)
	{
		args[1]->check_arg_type(TYPE_CONS);
		parmsub(ps,(cons_t *)args[1]);
		get_sim().redraw(true);
	}
	return args[0];
}

static void parmsub(simob_t *ps,cons_t *base)
{
	while (ps && base->is_a(TYPE_CONS))
	{
		if (ps->is_joint())
		{
			node_t* parm = base->Car();
			if (parm != nil)
			{
				parm->check_number();
				float fdef = (float)*(number_node_t *)parm;
				ps->set_parameter(fdef);
			}
			base = base->CdrCONS();
		}
		if (!base->is_a(TYPE_CONS))
			return;
		size_t cnt = ps->GetNumChildren();
		if (cnt > 1)
		{
			int idx=0;
			while (ps->GetChild(idx) && base->is_a(TYPE_CONS))
			{
				node_t* parm = base->Car();
				parm->check_arg_type(TYPE_CONS);
				parmsub(ps->GetChild(idx),(cons_t *)parm);
				idx++;
				base = base->CdrCONS();
			}
			if (!ps->GetChild(idx) && base->is_a(TYPE_CONS))
				throw_eval_exception(TOO_MANY_ARGS);
			return;
		}
		else
			ps = ps->GetChild();
	}
	if (!ps && base->is_a(TYPE_CONS))
		throw_eval_exception(TOO_MANY_ARGS);
}

static node_t *func_get__agent__joints(int numargs,node_t **args)
{
	auto ps = args[0]->as<simob_t>();
	return get_parmsub(ps);
}

static node_t* get_parmsub(simob_t *ps)
{
	cons_t* base = new cons_t(nil, nil);
	cons_t* cons = base;
	while (ps)
	{
		if (ps->is_joint())
			cons = cons->append_cons(new number_node_t(ps->get_parameter()));
		size_t cnt = ps->GetNumChildren();
		if (cnt > 1)
		{
			int idx=0;
			while (ps->GetChild(idx))
			{
				cons = cons->append_cons(get_parmsub(ps->GetChild(idx)));
				idx++;
			}
			return base->Cdr();
		}
		else
			ps = ps->GetChild();
	}
	return base->Cdr();
}

static node_t *func_ask__dialog(int numargs,node_t **args)
{
	int i;
	function *pfn;
	if (args[0] == nil)
		pfn = NULL;
	else
	{
		pfn = (function *) args[0];
		pfn->check_arg_type(TYPE_FUNCTION);
	}
	args++;
	numargs--;
	args[0]->check_arg_type(TYPE_STRING);
	auto title = ((string_node_t*)args[0])->data();
	args++;
	numargs--;

	std::vector<question> pq(numargs);
	for (i = 0; i < numargs; ++i)
	{
		auto& q = pq[i];
		args[i]->check_arg_type(TYPE_CONS);
		// Check list inside. First car is name, second is starting value, third is optional increment.
		auto cons = (cons_t*)args[i];
		auto count = cons->check_range_num_args(2, 3);

		auto desc = cons->Car();
		desc->check_arg_type(TYPE_STRING);
		q.question_text = ((string_node_t*)desc)->data();
		auto value = cons->Cadr();
		if (value->is_a(TYPE_FLOAT))
		{
			q.type = question_type_e::question_float;
			q.fresult = (float)*(number_node_t*)value;
		}
		else if (value->is_a(TYPE_LONG))
		{
			q.type = question_type_e::question_long;
			q.lresult = (long)*(number_node_t*)value;
		}
		else if (value->is_a(TYPE_STRING))
		{
			string_node_t* pstr = (string_node_t*)value;
			q.type = question_type_e::question_string;
			q.cresult = pstr->data();
		}
		else if (value->is_a(TYPE_SYMBOL))
		{
			symbol_t* psym = (symbol_t*)value;
			q.type = question_type_e::question_symbol;
			q.cresult = psym->get_name();
		}

		node_t* inc;
		if (count == 3)
		{
			inc = cons->CddrCONS()->Car();
			inc->check_arg_type(TYPE_FLOAT);
			q.incr = (float)*(number_node_t*)inc;
		}

	}

	return get_sim().ask(pfn,pq,title);
}

static node_t *func_message__box(int numargs,node_t **args)
{
	ostrstream ostr;
	int i;
	auto title = args[0]->as<string_node_t>();
	for (i=1;i<numargs;i++)
		args[i]->princ(ostr);
	ostr << (char)0;
	AfxGetMainWnd()->MessageBox(ostr.str(), title->data(), MB_OK);
	return args[numargs-1];
}

static node_t *func_check__stream(int numargs,node_t **args)
{
	auto stream = args[0]->as<stream_node_t>();
	stream->check_stream();
	node_t *p1,*p2,*p3,*p4;
	int state = stream->get_stream()->rdstate();
	p1 = (state & ios::goodbit) ? pTrue : nil;
	p2 = (state & ios::eofbit) ? pTrue : nil;
	p3 = (state & ios::failbit) ? pTrue : nil;
	p4 = (state & ios::badbit) ? pTrue : nil;
	return cons_t::make_list(p1, p2, p3, p4);
}

static node_t *func_check__collision(int numargs,node_t **args)
{
	int i,j;
	for (i=0;i<numargs;i++)
		args[i]->check_arg_type(TYPE_SIMOB);
	if (numargs == 0)
	{
		auto& arr = get_sim()._objects;
		int n = (int)arr.size();
		for (i = 0; i < n - 1; i++)
		{
			for (j = i + 1; j < n; j++)
			{
				if (simob_t::check_collide(arr[i], arr[j]))
					return cons_t::make_list(arr[i], arr[j]);
			}
		}
	}
	else
	{
		for (i = 0; i < numargs - 1; i++)
		{
			for (j = i + 1; j < numargs; j++)
			{
				if (simob_t::check_collide((simob_t*)args[i], (simob_t*)args[j]))
					return cons_t::make_list(args[i], args[j]);
			}
		}
	}
	return nil;
}

static node_t *func_grasp(int numargs,node_t **args)
{
	auto ps = args[0]->as<simob_t>();
	auto psa = args[1]->as<simob_t>();
	if (!ps->is_serial_agent())
		throw_eval_exception(NOT_SERIAL_AGENT);
	ps->check_in_env();
	psa->check_in_env();
	if (!ps->grasp(psa))
		return nil;
	get_sim().remove_object(psa, false); // the FALSE flags prevents losing views
	return args[1];
}

static node_t *func_release(int numargs,node_t **args)
{
	auto ps = args[0]->as<simob_t>();
	if (!ps->is_serial_agent())
		throw_eval_exception(NOT_SERIAL_AGENT);
	simob_t *psa = ps->release_grasp();
	if (psa)
	{
		if (!get_sim().add_object(psa))
			return nil;
	}
	return psa;
}

static node_t *func_set__num__facets(int numargs,node_t **args)
{
	int nf = *args[0]->as<number_node_t>();
	if (nf < 4)
		throw_eval_exception(DOMAIN_ERROR);
	deffac = nf;
	return args[0];
}

static node_t *func_get__num__facets(int numargs,node_t **args)
{
	return new number_node_t((long)deffac);
}

static void make_point_from_args(node_t **args,point3d_t &pt)
{
	args[0]->check_number();
	args[1]->check_number();
	args[2]->check_number();
	pt.x = (float) (* (number_node_t *)args[0]);
	pt.y = (float) (* (number_node_t *)args[1]);
	pt.z = (float) (* (number_node_t *)args[2]);
}

static node_t *func_set__look__from(int numargs,node_t **args)
{
	point3d_t pt;
	make_point_from_args(args,pt);
	return get_sim().set_look_from(pt) ? pTrue : nil;
}

static node_t *func_set__look__at(int numargs,node_t **args)
{
	point3d_t pt;
	make_point_from_args(args,pt);
	return get_sim().set_look_at(pt) ? pTrue : nil;
}

static node_t *func_set__eye__separation(int numargs,node_t **args)
{
	// Deprecated
	float es = *args[0]->as<number_node_t>();
	return nil;
}

static node_t *func_set__camera(int numargs,node_t **args)
{
	point3d_t pt;
	make_point_from_args(args,pt);
	return get_sim().set_camera(pt.x, pt.y, pt.z) ? pTrue : nil;
}

static node_t *func_get__look__from(int numargs,node_t **args)
{
	point3d_t pt;
	if (!get_sim().get_look_from(pt))
		return nil;
	return cons_t::make_list(
		new number_node_t(pt.x),
		new number_node_t(pt.y),
		new number_node_t(pt.z));
}

static node_t *func_get__look__at(int numargs,node_t **args)
{
	point3d_t pt;
	if (!get_sim().get_look_at(pt))
		return nil;
	return cons_t::make_list(
		new number_node_t(pt.x),
		new number_node_t(pt.y),
		new number_node_t(pt.z));
}

static node_t *func_get__camera(int numargs,node_t **args)
{
	float focal, twist, zoom;
	if (!get_sim().get_camera(focal, twist, zoom))
		return nil;
	return cons_t::make_list(
		new number_node_t(focal),
		new number_node_t(twist),
		new number_node_t(zoom));
}

static node_t *func_object__in__use__p(int numargs,node_t **args)
{
	args[0]->check_arg_type(TYPE_SIMOB);
	return ((simob_t *)args[0])->is_in_env() ? pTrue : nil;
}

class DriveStruct
{
public:
	simob_t *ps;
	float beg,end;
	DriveStruct(simob_t *p,float e)
	{
		ps = p;
		beg = ps->get_parameter();
		end = e;
	}
	DriveStruct(simob_t *p)
	{
		ps = p;
		beg = ps->get_parameter();
		end = 0.F;
	}
	void SetParm(float rat) {ps->set_parameter(beg + (end-beg)*rat);}
};

class DriveArray
{
public:
	std::vector<DriveStruct> vec;
//	void AddDriveStruct(unique_ptr<DriveStruct>&& pds) { vec.push_back(std::move(pds)); }
	void dparmsub(simob_t *ps,cons_t *base);
	void parmsub(simob_t *ps);
};

void DriveArray::dparmsub(simob_t *ps,cons_t *base)
{
	node_t *parm;
	while (ps && base->is_a(TYPE_CONS))
	{
		if (ps->is_joint())
		{
			parm = base->Car();
			if (parm != nil)
			{
				parm->check_number();
				vec.emplace_back(ps, (float)*(number_node_t *)parm);
			}
			base = base->CdrCONS();
		}
		if (!base->is_a(TYPE_CONS))
			return;
		if (ps->GetNumChildren() > 1)
		{
			int idx=0;
			while (ps->GetChild(idx) && base->is_a(TYPE_CONS))
			{
				parm = base->Car();
				parm->check_arg_type(TYPE_CONS);
				dparmsub(ps->GetChild(idx++),(cons_t *)parm);
				base = base->CdrCONS();
			}
			if (!ps->GetChild(idx) && base->is_a(TYPE_CONS))
				throw_eval_exception(TOO_MANY_ARGS);
			return;
		}
		else
			ps = ps->GetChild();
	}
	if (!ps && base->is_a(TYPE_CONS))
		throw_eval_exception(TOO_MANY_ARGS);
}

void DriveArray::parmsub(simob_t *ps)
{
	while (ps)
	{
		if (ps->is_joint())
			vec.emplace_back(ps);
		if (ps->GetNumChildren() > 1)
		{
			int idx=0;
			while (ps->GetChild(idx))
				parmsub(ps->GetChild(idx++));
			return;
		}
		else
			ps = ps->GetChild();
	}
}

static node_t *func_drive__agent(int numargs,node_t **args)
{
	int i;
	if (numargs == 1)
		return args[0];
	args[0]->check_arg_type(TYPE_SIMOB);
	args[1]->check_arg_type(TYPE_CONS);
	int steps = *args[2]->as<number_node_t>();
	DriveArray da;
	da.dparmsub((simob_t *)args[0],(cons_t *)args[1]);
	for (i=1;i<=steps;i++)
	{
		float rat = (float)i/(float)steps;
		for (auto& ds : da.vec)
			ds.SetParm(rat);
		get_sim().redraw(true);
	}
	return args[1];
}

static node_t *func_show__frames(int numargs,node_t **args)
{
	auto& sim = get_sim();
	bool b = (args[0] == nil) ? false : true;
	if ((b && !sim.show_frames) || (!b && sim.show_frames))
	{
		sim.show_frames = b;
		sim.redraw(true);
	}
	return args[0];
}

static node_t *func_get__child(int numargs,node_t **args)
{
	auto p = args[0]->as<simob_t>();
	p = p->GetChild();
	return (p) ? (node_t *)p : (node_t *)nil;
}

static node_t *func_get__solution(int numargs,node_t **args)
{
	solution *pq;
	node_t *pn = nil;
	auto ps = args[0]->as<simob_t>();

	if (!ps->is_serial_agent())
		throw_eval_exception(BAD_ARG_TYPE);
	args[1]->check_arg_type(TYPE_MAT44);
	args[2]->check_number();
	pq = ps->get_solution((mat44 *)args[1],(long)*(number_node_t *)args[2]);
	if (pq) // if !pq -> no solution
	{
		pn = cons_t::make_list(
			new number_node_t(pq[0][0]),
			new number_node_t(pq[0][1]),
			new number_node_t(pq[0][2]),
			new number_node_t(pq[0][3]),
			new number_node_t(pq[0][4]),
			new number_node_t(pq[0][5])
			);
	}
	return pn;
}

static node_t *func_get__solutions(int numargs,node_t **args)
{
	int i,j;
	solution *pq;
	node_t *pl= nil;
	auto ps = args[0]->as<simob_t>();

	if (!ps->is_serial_agent())
		throw_eval_exception(BAD_ARG_TYPE);
	args[1]->check_arg_type(TYPE_MAT44);
	pq = ps->get_solution((mat44 *)args[1],-1);
	if (pq) // if !pq -> no solutions
	{
		for (j = ps->get_num_solutions() - 1; j >= 0; j--)
		{
			auto pn = cons_t::make_list(
				new number_node_t(pq[j][0]),
				new number_node_t(pq[j][1]),
				new number_node_t(pq[j][2]),
				new number_node_t(pq[j][3]),
				new number_node_t(pq[j][4]),
				new number_node_t(pq[j][5])
			);
			pl = new cons_t(pn, pl);
		}
	}
	return pl;
}

static node_t *func_set__collision__check(int numargs,node_t **args)
{
	get_sim().set_collision_check((args[0] == nil) ? false : true);
	return args[0];
}

static node_t *func_get__num__solutions(int nuargs,node_t **args)
{
	auto ps = args[0]->as<simob_t>();
	if (!ps->is_serial_agent())
		throw_eval_exception(BAD_ARG_TYPE);
	return new number_node_t( (long)ps->get_num_solutions());
}

static node_t *func_move__inter__to(int numargs,node_t **args)
// agent,mat44,solution index,steps
{
	int i;
	solution *pqe;

	auto ps = args[0]->as<simob_t>();
	auto pmat = args[1]->as<mat44>();
	int solidx = *args[2]->as<number_node_t>();
	int steps = *args[3]->as<number_node_t>();

	if (!ps->is_serial_agent())
		throw_eval_exception(BAD_ARG_TYPE);
	if (steps <=0)
		throw_eval_exception(DOMAIN_ERROR);

	pqe = ps->get_solution(pmat,solidx);
	if (!pqe)
		throw_other_exception(current_package->get_symbol("REACH"),nil); // out of reach exception
	DriveArray da;
	da.parmsub(ps);
	for (size_t j = 0; j < da.vec.size(); ++j)
		da.vec[j].end = pqe[0][j];
	for (i=1;i<=steps;i++)
	{
		float rat = (float)i/(float)steps;
		for (auto& ds : da.vec)
			ds.SetParm(rat);
		get_sim().redraw(true);
	}
	return pTrue;
}

static node_t *func_move__straight__to(int numargs,node_t **args)
// agent,mat44,solution index,steps
// This move_straight commanded have been change to be able to set orientation
// by using Y-Z-Y euler angle.
{
	int i;
	solution *pqe;

	auto ps = args[0]->as<simob_t>();
	auto pmat = args[1]->as<mat44>();
	int solidx = *args[2]->as<number_node_t>();
	int steps = *args[3]->as<number_node_t>();
	mat44 cmat;
	
	if (!ps->is_serial_agent())
		throw_eval_exception(BAD_ARG_TYPE);
	if (steps <=0)
		throw_eval_exception(DOMAIN_ERROR);

	cmat = ps->get_end_position();
	point3d_t beg(cmat.m[0][3],cmat.m[1][3],cmat.m[2][3]);
	point3d_t end(pmat->m[0][3],pmat->m[1][3],pmat->m[2][3]);
	point3d_t res;
	euler_zyz_t euler = cmat.GetEulerZYZ();
	euler_zyz_t peuler = pmat->GetEulerZYZ();
	euler_zyz_t eres;
	DriveArray da;

	da.parmsub(ps);
	for (i=1;i<=steps;i++)
	{
		float rat = (float)i/(float)steps;
		res = beg + (end-beg)*rat;
		eres = euler + (peuler-euler)*rat;

		cmat = cmat.EULER(eres);
		cmat.m[0][3] = res.x;
		cmat.m[1][3] = res.y;
		cmat.m[2][3] = res.z;

		pqe = ps->get_solution(&cmat,solidx);
		if (!pqe)
			throw_other_exception(current_package->get_symbol("REACH"),nil); // out of reach exception
		for (size_t j=0;j<da.vec.size();j++)
			da.vec[j].ps->set_parameter(pqe[0][j]);
		get_sim().redraw(true);
	}
	return pTrue;
}

static node_t *func_set__update(int numargs,node_t **args)
{
	get_sim().set_update( (args[0]==nil) ? false : true);
	return args[0];
}

static node_t *func_maximize(int numargs,node_t **args)
{
	get_sim().maximize_window(args[0] != nil);
	return args[0];
}

static node_t *func_transformp(int numargs,node_t **args)
{
	return (args[0]->is_a(TYPE_MAT44)) ? pTrue : nil;
}

static node_t *func_simobp(int numargs,node_t **args)
{
	return (args[0]->is_a(TYPE_SIMOB)) ? pTrue : nil;
}

static node_t *func_copy__window(int numargs,node_t **args)
{
	return get_sim().copy_to_clibboard() ? pTrue : nil;
}

static node_t *func_use__kinematics(int numargs,node_t **args)
{
	int i;
	for (i=0;i<numargs;i++)
	{
		auto str = args[i]->as<string_node_t>();
		auto pik = make_unique<ik_dll_t>(str->data());
		if (pik->is_ok())
			invkin.push_back(std::move(pik));
		else
		{
			throw_eval_exception(IKLIB_NOT_VALID);
		}
	}
	return pTrue;
}

static node_t *func_use__kinematics__lisp(int numargs,node_t **args)
{
	auto f1 = args[0]->as<function>();
	auto f2 = args[1]->as<function>();
	auto f3 = args[2]->as<function>();
	auto f4 = args[3]->as<function>();
	auto pik = make_unique<ik_lisp_t>(f1, f2, f3, f4);
	if (pik->is_ok())
		invkin.push_back(std::move(pik));
	else
	{
		throw_eval_exception(IKLIB_NOT_VALID);
	}
	return pTrue;
}

static node_t *func_display__help(int numargs, node_t **args)
{
	auto str = args[0]->as<string_node_t>();
	return get_sim().display_help(str->str()) ? pTrue : nil;
}

static node_t *func_winexec(int numargs, node_t** args)
{
	if (args[0] == nil)
		return nil;
	args[0]->check_arg_type(TYPE_STRING);
	ostrstream ostr;
	for (int i=0; i< numargs;i++)
		args[i]->princ(ostr);
	ostr << (char)0;
	UINT uRes = WinExec(ostr.str(), SW_SHOWNORMAL);
	ostr.rdbuf()->freeze(0);
	return (uRes > 31) ? pTrue : nil;
}

static node_t *func_tref(int numargs,node_t **args)
{
	auto pmat = args[0]->as<mat44>();
	int nRow = *args[1]->as<number_node_t>();
	int nCol = *args[2]->as<number_node_t>();
	if (nRow <0 || nRow > 3 || nCol < 0 || nCol > 3)
		throw_eval_exception(DOMAIN_ERROR);

	return new number_node_t(pmat->m[nRow][nCol]);
}

static node_t *func_adjust__lights(int numargs, node_t** args)
{
	return get_sim().adjust_lights() ? pTrue : nil;
}

static node_t *func_set__light(int numargs,node_t **args)
{
	if (numargs % 2 != 0)
		throw_eval_exception(TOO_FEW_ARGS);
	int nLight = *args[0]->as<number_node_t>();
	if (nLight < 0 || nLight>7)
		throw_eval_exception(DOMAIN_ERROR);

	light_t light;
	if (!get_sim().get_light(nLight, light))
		return nil;
	light._is_enabled = (args[1] != nil);
	for (int i=2;i<numargs;i+=2)
	{
		args[i]->check_arg_type(TYPE_SYMBOL);
		if (args[i] == key_position)
			ParseFloatList(light._position, (cons_t *)args[i+1]);
		else if (args[i] == key_ambient)
			ParseFloatList(light._ambient_properties, (cons_t *)args[i+1], -1.f, 1.f);
		else if (args[i] == key_diffuse)
			ParseFloatList(light._diffuse_properties, (cons_t *)args[i+1], -1.f, 1.f);
		else if (args[i] == key_specular)
			ParseFloatList(light._specular_properties, (cons_t *)args[i+1], -1.f, 1.f);
		else
			throw_eval_exception(BAD_ARG_TYPE);
	}
	get_sim().set_light(nLight, light);
	return pTrue;
}

static node_t *func_set__clipplane(int numargs, node_t** args)
{
	float fNear = *args[0]->as<number_node_t>();
	float fFar = *args[1]->as<number_node_t>();
	return get_sim().set_clip_plane(fNear, fFar) ? pTrue : nil;
}

static node_t *func_enable__solid(int numags, node_t** args)
{
	bool solid = args[0] != nil;
	return get_sim().enable_solid(solid) ? pTrue : nil;
}

static node_t *func_enable__lighting(int numags, node_t** args)
{
	bool lighting = args[0] != nil;
	return get_sim().enable_lighting(lighting) ? pTrue : nil;
}

static node_t *func_enable__cull(int numags, node_t** args)
{
	bool cull = args[0] != nil;
	return get_sim().enable_cull(cull) ? pTrue : nil;
}

static node_t *func_enable__smooth__shading(int numargs, node_t** args)
{
	bool smooth = args[0] != nil;
	return get_sim().enable_smooth_shading(smooth) ? pTrue : nil;
}

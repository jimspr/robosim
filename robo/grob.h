#pragma once
#ifndef __GROB_H
#define __GROB_H

#include "matrix.h"
#include "ik.h"

static constexpr double RPI = 3.14159265358979323846;   // pi
inline constexpr float RAD2DEG(float rad)
{
	return (float)(rad * 180.0 / RPI);
}

inline constexpr float DEG2RAD(float deg)
{
	return (float)(deg * RPI / 180.);
}

typedef mat44(*PMAT44FUNC)(float);

class edge_t
{
public:
	point3d_t beg;
	point3d_t end;
	edge_t(const point3d_t& b, const point3d_t& e) { beg = b; end = e; }
	void set(const point3d_t& b, const point3d_t& e) { beg = b; end = e; }
	edge_t() {}
};

class penalty_t
{
public:
	point3d_t   normal;
	float     distance{ 0.f };
	float penalty(const point3d_t& p) const { return point3d_t::dot(normal, p) + distance; }
};

class polygon_t
{
public:
	point3d_t   norm;
	float     nd{ 0.f };
	const vertex3d_t* vec_ptr = nullptr;
	std::vector<penalty_t> penalties;
	float residue(const point3d_t& p) const { return point3d_t::dot(norm, p) + nd; }
	bool poly_int_box(const bounding_box_t& b) const { return (fabs(residue(b.c)) <= point3d_t::dipr(norm, b.hd)); }
	int point_in_polygon(const point3d_t& p);
	int edge_in_polygon(const edge_t& e);
	void make_poly(const vertex3d_t* p, int nv);
};

class Agent;

/* Represents a 3D simulation object. */
class simob_t : public node_t
{
	// _matpos is the absolute position of this link in world coordinates
	// pmatparent is a pointer to the parent link's _matpos
	// _matrel is the position of this link relative to the parent
	// _pjointfunc(_parameter) is the current position of this link relative to its home position
	// _pjointfunc can be set to any mat44 function that takes one argument and returns a mat44.
	// This allows prismatic and revolute joints to be handled the same way.
protected:
	mat44 _matpos;
	mat44 _matrel;
	mat44* pmatparent; // matparent * _matrel = _matpos
	std::vector<vertex3d_t> _gmodel;
	std::vector<vertex3d_t> _gworld;
	int _numvec;
	int _numpoly;
	std::vector<int> _polycnts;
	bounding_box_t _boxmodel;
	bool _invalidate_model{ false };
	bool _invalidate_world{ false };
	bool _invalidate_position{ false };
	bool _is_in_env{ false };
	bool _is_graspee{ false };
	std::vector<simob_t*> _children;
	GLfloat _ambient[4];
	GLfloat _diffuse[4];
	GLfloat _specular[4];
	GLfloat _shininess[1];
	std::vector<polygon_t>    _polygons; // array of polygon structures used in collision detection
// These Parameters used for movable links
	float _parameter;
	float _minparm;
	float _maxparm;
	PMAT44FUNC _pjointfunc;
	bool _classic{ false };
	// _classic is true when the axis of motion of a link is specified by the frame of the previous link
	// _classic is true -> T6 = Rot1(z,theta)*A1*Rot2(z,theta)*A2* ... *A6
	// where Ai is the matrix describing the coordinate frame of link i relative to i-1
	// and Roti(z,theta) is the result of moving link i.
	// _classic is false when the axis of motion of a link is specified by itself.
	// _classic is false -> T6 = A1*Rot1(z,theta)*A2*Rot2(z,theta)* ... *A6*Rot6(z,theta)

	/* _end used for serial agents. Points to last link. */
	simob_t* _end;
	//	char *invparms;
	std::unique_ptr<ik_interface> _inverse_kinematics;
	//	solution *psols;
	int _num_solns;

public:
	static bool verify(node_t* p) { return p->get_type() == TYPE_SIMOB; }
	void verify_simob() const;
	simob_t();
	simob_t(simob_t&);
	void mark_in_use();
	void print(std::ostream& ostr) const { ostr << "<simob_t " << (long)this << ">"; }
	void init(int numv, int nump);
	void UpdateObject();
	void UpdateModel();
	void UpdatePos();
	void UpdateWorld();

	// These functions used for movable links
	void set_min_max(float mn, float mx) { _minparm = mn; _maxparm = mx; }
	void set_parameter(float f);
	float get_parameter() const { return _parameter; }
	void set_joint_func(PMAT44FUNC p);
	bool is_joint() const { return (_pjointfunc) ? true : false; }
	//
	// These functions used for serial agents
	simob_t* get_graspee() const { return _end->GetChild(); }
	simob_t* release_grasp();
	bool grasp(simob_t* ps);
	bool is_serial_agent() const { return _end ? true : false; }
	void set_classic(bool b) { _classic = b; }
	bool make_agent(simob_t* plast, int numlinks);
	int get_num_solutions() { return _num_solns; }
	solution* get_solution(mat44* pmat, int idx);
	mat44& get_end_position() { return _end->GetPosition(); }
	//
	bool is_attached() const { return (pmatparent) ? true : false; }
	bool is_in_env() const { return _is_in_env; }
	void check_in_env() const { if (!_is_in_env) throw_eval_exception(OBJECT_NOT_IN_ENV); }
	void set_in_env(bool b) { _is_in_env = b; }
	simob_t* GetChild(int idx = 0);
	int GetNumChildren() { return _children.size(); }
	void SetPosition(const mat44& mat);
	void set_parent_mat(mat44* pm);
	void PosChanged();
	void set_ambient(GLfloat* p);
	void set_shininess(GLfloat* p);
	void set_diffuse(GLfloat* p);
	void set_specular(GLfloat* p);
	void Attach(simob_t* ps);
	bool Detach(simob_t* ps);
	void Draw();
	int GetNumVec() const { return _numvec; }
	int GetNumPoly() const { return _numpoly; }
	void Add(simob_t& g);
	void add_world(simob_t& g);
	void intersect(simob_t& g);
	void union_with(simob_t& g);
	void calc_bounding_box();
	mat44& GetPosition() { UpdatePos(); return _matpos; }
	void add_vector(vector_t& v);
	void new_poly(int n) { _polycnts[_numpoly++] = n; }
	void make_box(float x, float y, float z);
	void make_circle(float r, int facets);
	void make_opencyl(float r, float h, int facets);
	void make_closedcyl(float r, float h, int facets);
	void make_sphere(float r, int facets);
	void make_opencone(float radius, float height, int facets,
		point3d_t* pnormu = NULL, point3d_t* pnorml = NULL);
	void make_closedcone(float r, float h, int facets);
	void make_truncated_opencone(float rl, float ru, float height, int facets,
		point3d_t* pnormu = NULL, point3d_t* pnorml = NULL);
	void make_truncated_closedcone(float rl, float ru, float h, int facets);
	void make_opendome(float r, int facets);
	void make_closeddome(float r, int facets);
	void make_extrusion(simob_t*, float h);
	void make_revolution(simob_t* ps, long facets);
	void make_polys();
	static int check_collide(simob_t* X1, simob_t* X2);
	static int two_collide(simob_t* X1, simob_t* X2);
	static int low_collision(simob_t* X1, simob_t* X2);
	int poly_edge(edge_t* re, int ne, bounding_box_t& bbox);
	//
	DECLARE_NODE(simob_t, 16)
};

#endif


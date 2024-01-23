#include "stdafx.h"
#include <memory>
#include <float.h>

#include "gl\gl.h"
#include "gl\glu.h"

#include "ik.h"
#include "matrix.h"
#include "grob.h"
#include "package.h"
#include "simula.h"
#include "lisp_engine.h"

using namespace std;

simob_t::simob_t() : node_t(TYPE_SIMOB)
{
}

simob_t::simob_t(simob_t &rs) : node_t(TYPE_SIMOB)
{
	add(rs);
	memcpy(_ambient, rs._ambient, 4*sizeof(GLfloat));
	memcpy(_diffuse, rs._diffuse, 4*sizeof(GLfloat));
	memcpy(_specular, rs._specular, 4*sizeof(GLfloat));
}

void simob_t::init(int numv,int nump)
{
	_gworld.resize(numv);
	_gmodel.resize(numv);
	_polycnts.resize(nump);
	_polygons.resize(nump);
	_numpoly = _numvec = 0;
	_invalidate_world = true;
}

void simob_t::update_pos()
{
	if (_invalidate_position)
	{
		_matpos = _matrel;
		if (_classic && _pjointfunc)
			_matpos.premultiply((*_pjointfunc)(_parameter));
		if (pmatparent)
			_matpos.premultiply(*pmatparent);
		if (!_classic && _pjointfunc)
			_matpos.postmultiply((*_pjointfunc)(_parameter));
		_invalidate_position = false;
		_invalidate_world = true;
	}
}

void simob_t::update_world()
{
	update_pos();
	_matpos.transform(_gworld.data(),_gmodel.data(),_numvec);
	_invalidate_world = false;
}

void simob_t::update_model()
{
	make_polys();
	calc_bounding_box();
	_invalidate_model = false;
	_invalidate_world = true;
}

void simob_t::update_object()
{
	if (_invalidate_model)
		update_model();
	if (_invalidate_position)
		update_pos();
	if (_invalidate_world)
		update_world();
}

void simob_t::attach(simob_t *ps)
{
	_children.push_back(ps);
	ps->set_parent_mat(&_matpos);
}

bool simob_t::detach(simob_t *ps)
{
	ps->set_parent_mat(nullptr);
	for (auto iter = _children.begin(); iter != _children.end(); ++iter)
	{
		if (*iter == ps)
		{
			_children.erase(iter);
			return true;
		}
	}
	return false;
}

void simob_t::mark_in_use()
{
	node_t::mark_in_use();
	if (_inverse_kinematics != nullptr)
		_inverse_kinematics->mark_in_use();
	for (auto& child : _children)
		child->mark_in_use();
}

void simob_t::set_parent_mat(mat44 *pm)
{
	pmatparent = pm;
	pos_changed();
}

void simob_t::set_position(const mat44 &mat)
{
	_matrel = mat;
	pos_changed();
}

void simob_t::set_joint_func(PMAT44FUNC p)
{
	_pjointfunc = p;
	pos_changed();
}

void simob_t::set_parameter(float f)
{
	if (f<_minparm || f>_maxparm)
		throw_other_exception(lisp_engine._package.get_symbol("JOINTLIMIT"),nil); // out of reach exception
	_parameter=f;
	pos_changed();
}

simob_t *simob_t::get_child(int idx)
{
	return (idx >= (int)_children.size()) ? nullptr : _children[idx];
}

simob_t* simob_t::get_child(size_t idx)
{
	return (idx >= _children.size()) ? nullptr : _children[idx];
}

void simob_t::pos_changed()
{
	_invalidate_position = true;
	for (auto& child: _children)
		child->pos_changed();
}

void simob_t::set_ambient(GLfloat *p)
{
	memcpy(_ambient, p, sizeof(GLfloat)*3);
	for (auto& child : _children)
		child->set_ambient(p);
}

void simob_t::set_shininess(GLfloat* p)
{
	memcpy(_shininess, p, sizeof(GLfloat) * 1);
	for (auto& child : _children)
		child->set_shininess(p);
}

void simob_t::set_diffuse(GLfloat *p)
{
	memcpy(_diffuse, p, sizeof(GLfloat)*3);
	for (auto& child : _children)
		child->set_diffuse(p);
}

void simob_t::set_specular(GLfloat *p)
{
	memcpy(_specular, p, sizeof(GLfloat)*3);
	for (auto& child : _children)
		child->set_specular(p);
}

void simob_t::draw()
{
	int i;
	update_object();

	int *pc = _polycnts.data();
	vertex3d_t* pPt = _gworld.data();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, _specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, _shininess);
	glColor3fv(_ambient);
	for (i=0;i<_numpoly;i++,pc++)
	{
		glBegin(GL_POLYGON);

		bool bNoNormals = pPt[0].normal == point3d_t{ 0.f, 0.f, 0.f };
		if (bNoNormals && *pc > 2)
		{
			point3d_t norm;
			point3d_t v1 = pPt[1]-pPt[0];
			point3d_t v2 = pPt[2]-pPt[1];
			norm = (v1*v2)/(v1.length()*v2.length());		/* (P1-P0)x(P2-P1)/(||P1-P0||||P2-P1||) */
			norm.normalize();
			glNormal3f(norm.x, norm.y, norm.z);
		}
		for (int j=0;j<*pc;j++)
		{
			if (!bNoNormals)
				glNormal3f(pPt[j].normal.x, pPt[j].normal.y, pPt[j].normal.z);
			glVertex3f(pPt[j].pt.x, pPt[j].pt.y, pPt[j].pt.z);
		}
		pPt += *pc;
		glEnd();
	}
	for (auto& child : _children)
		child->draw();

	if (get_sim()._show_frames)
	{
		point3d_t axis[4]= {
			point3d_t{0.F,0.F,0.F},
			point3d_t{1.F,0.F,0.F},
			point3d_t{0.F,1.F,0.F},
			point3d_t{0.F,0.F,1.F}
			};
		CPoint pts[4];
		GLfloat red[4] = {1.f, 0.f, 0.f, 1.f};
		GLfloat green[4] = {0.f, 1.f, 0.f, 1.f};
		GLfloat blue[4] = {0.f, 0.f, 1.f, 1.f};
		float mx = (_boxmodel.hd.x > _boxmodel.hd.y) ? _boxmodel.hd.x : _boxmodel.hd.y;
		mx = (_boxmodel.hd.z > mx) ? _boxmodel.hd.z : mx;
		mx = (mx < 2.F) ? 1.F : mx/2.F;
		axis[1].x *= mx;
		axis[2].y *= mx;
		axis[3].z *= mx;
		_matpos.transform(axis,4);
		glBegin(GL_LINES);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
		glColor3fv(red);
		glVertex3f(axis[0].x, axis[0].y, axis[0].z);
		glVertex3f(axis[1].x, axis[1].y, axis[1].z);
//		glVertex3f(0, 0, 0);
//		glVertex3f(100, 0, 0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, green);
		glColor3fv(green);
		glVertex3f(axis[0].x, axis[0].y, axis[0].z);
		glVertex3f(axis[2].x, axis[2].y, axis[2].z);
//		glVertex3f(0, 0, 0);
//		glVertex3f(0, 100, 0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
		glColor3fv(blue);
		glVertex3f(axis[0].x, axis[0].y, axis[0].z);
		glVertex3f(axis[3].x, axis[3].y, axis[3].z);
//		glVertex3f(0, 0, 0);
//		glVertex3f(0, 0, 100);
		glEnd();


	}
}

void simob_t::verify_simob() const
{
	ASSERT(_numvec == _gmodel.size());
	ASSERT(_numvec == _gworld.size());
	ASSERT(_numpoly == _polygons.size());
	ASSERT(_numpoly == _polycnts.size());
}

template <typename T>
void append(std::vector<T>& t1, const std::vector<T>& t2)
{
	t1.insert(t1.end(), t2.begin(), t2.end());
}

void simob_t::add(simob_t &g)
{
	verify_simob();
	g.verify_simob();

	append(_gmodel, g._gmodel);
	_gworld.resize(_numvec + g._numvec);
	append(_polycnts, g._polycnts);
	_polygons.resize(_numpoly + g._numpoly);

	_numvec += g._numvec;
	_numpoly += g._numpoly;
	_invalidate_model = true;
}

void simob_t::add_world(simob_t &g)
{
	verify_simob();
	g.verify_simob();

	_gmodel.resize(_numvec + g._numvec);
	_gworld.resize(_numvec + g._numvec);
	update_pos();
	g.update_world();
	mat44 mrel = _matpos.fastinverse() * g._matpos;
	mrel.transform(&_gmodel[_numvec], &g._gmodel[0], g._numvec);
	append(_polycnts, g._polycnts);
	_polygons.resize(_numpoly + g._numpoly);

	_numvec += g._numvec;
	_numpoly += g._numpoly;
	_invalidate_model = _invalidate_world = true;
}

/**************************************************************
Both objects may already have a transformation matrix associated
with them. The world coordinates of g should be transformed by the inverse
of the position matrix of [this] and then added to the model coordinates.
***************************************************************/

void simob_t::add_vector(vector_t &v)
{
	size_t n = v.size();
	if (n != 3 && n != 6)
		throw_eval_exception(VECTOR_NOT_POINT);
	number_node_t *nx = (number_node_t *)v[0];
	nx->check_number();
	number_node_t *ny = (number_node_t *)v[1];
	ny->check_number();
	number_node_t *nz = (number_node_t *)v[2];
	nz->check_number();
	vertex3d_t& vert = _gmodel[_numvec];
	vert.pt = point3d_t{ *nx, *ny, *nz };
	if (n == 6)
	{
		number_node_t* nx = (number_node_t*)v[3];
		nx->check_number();
		number_node_t* ny = (number_node_t*)v[4];
		ny->check_number();
		number_node_t* nz = (number_node_t*)v[5];
		nz->check_number();
		vert.normal = point3d_t{*nx, *ny, *nz};
	}
	else
		vert.normal = point3d_t{};
	_numvec++;
	_invalidate_model = _invalidate_world = true;
}

static vertex3d_t unitbox[30] =
{
	vertex3d_t(0.f,0.f,0.f,0.f,0.f,-1.f),vertex3d_t(0.f,1.f,0.f,0.f,0.f,-1.f),vertex3d_t(1.f,1.f,0.f,0.f,0.f,-1.f),vertex3d_t(1.f,0.f,0.f,0.f,0.f,-1.f),vertex3d_t(0.f,0.f,0.f,0.f,0.f,-1.f), // bottom
	vertex3d_t(0.f,0.f,1.f,0.f,0.f,1.f),vertex3d_t(1.f,0.f,1.f,0.f,0.f,1.f),vertex3d_t(1.f,1.f,1.f,0.f,0.f,1.f),vertex3d_t(0.f,1.f,1.f,0.f,0.f,1.f),vertex3d_t(0.f,0.f,1.f,0.f,0.f,1.f), // top
	vertex3d_t(1.f,0.f,0.f,1.f,0.f,0.f),vertex3d_t(1.f,1.f,0.f,1.f,0.f,0.f),vertex3d_t(1.f,1.f,1.f,1.f,0.f,0.f),vertex3d_t(1.f,0.f,1.f,1.f,0.f,0.f),vertex3d_t(1.f,0.f,0.f,1.f,0.f,0.f), // front
	vertex3d_t(0.f,0.f,0.f,-1.f,0.f,0.f),vertex3d_t(0.f,0.f,1.f,-1.f,0.f,0.f),vertex3d_t(0.f,1.f,1.f,-1.f,0.f,0.f),vertex3d_t(0.f,1.f,0.f,-1.f,0.f,0.f),vertex3d_t(0.f,0.f,0.f,-1.f,0.f,0.f), // back
	vertex3d_t(0.f,0.f,0.f,0.f,-1.f,0.f),vertex3d_t(1.f,0.f,0.f,0.f,-1.f,0.f),vertex3d_t(1.f,0.f,1.f,0.f,-1.f,0.f),vertex3d_t(0.f,0.f,1.f,0.f,-1.f,0.f),vertex3d_t(0.f,0.f,0.f,0.f,-1.f,0.f), // left
	vertex3d_t(0.f,1.f,0.f,0.f,1.f,0.f),vertex3d_t(0.f,1.f,1.f,0.f,1.f,0.f),vertex3d_t(1.f,1.f,1.f,0.f,1.f,0.f),vertex3d_t(1.f,1.f,0.f,0.f,1.f,0.f),vertex3d_t(0.f,1.f,0.f,0.f,1.f,0.f)  // right
};
static int boxcnts[6] = {5,5,5,5,5,5};

void simob_t::make_box(float x,float y,float z)
{
	init(30,6);
	_numvec = 30;
	_numpoly = 6;
	memcpy(_polycnts.data(), boxcnts, _numpoly*sizeof(int));
	mat44 mat = mat44::trans(-x/2.f,-y/2.f,0.f)*mat44::scale(x,y,z);
	mat.transform(_gmodel.data(), unitbox, _numvec);
	_invalidate_model = true;
}

void simob_t::make_circle(float radius,int facets)
{
	init(facets+1,1);
	_polycnts[_numpoly++] = facets+1;
	int i;
	for (i=0;i<=facets;i++)
	{
		float ang = (float)(2.*RPI)*(float)(facets-i)/(float)facets;
		_gmodel[_numvec++] = vertex3d_t(radius*(float)cos(ang),radius*(float)sin(ang),0.F, 0.F, 0.F, -1.F);
	}
	_invalidate_model = true;
	verify_simob();
}

void simob_t::make_truncated_opencone(float rl,float ru,float height, int facets, point3d_t* pnormu, point3d_t* pnorml)
{
	int i;
	vertex3d_t face[4];
	vertex3d_t tface[4];

	float angle = (float)(2.*RPI)/(float)facets;
	init(5*facets,facets);
	if (pnormu != NULL)
		face[0] = vertex3d_t(ru,0.F,height, pnormu->x, pnormu->y, pnormu->z);
	else
	{
		point3d_t normal{ height, 0.f, rl - ru };
		normal.normalize();
		face[0] = vertex3d_t(point3d_t{ ru, 0.F, height }, normal);
	}
	if (pnorml != nullptr)
		face[1] = vertex3d_t(rl,0.F,0.F, pnorml->x, pnorml->y, pnorml->z);
	else
	{
		point3d_t normal{ height, 0.f, rl - ru };
		normal.normalize();
		face[1] = vertex3d_t{ point3d_t{ rl, 0.F, 0.F }, normal };
	}
	mat44 mat = mat44::rotatez(angle);
	mat.transform(face[2], face[1]);
	mat.transform(face[3], face[0]);
	for (i=0;i<facets;i++)
	{
		_polycnts[_numpoly++] = 5;
		angle = (float)(2.*RPI)*(float)(facets-i)/(float)facets;
		mat44::rotatez(angle).transform(tface,face,4);
		_gmodel[_numvec++] = tface[0];
		_gmodel[_numvec++] = tface[1];
		_gmodel[_numvec++] = tface[2];
		_gmodel[_numvec++] = tface[3];
		_gmodel[_numvec++] = tface[0];
	}
	float radius = (ru > rl) ? ru : rl;
	_invalidate_model = true;
	verify_simob();
}

void simob_t::make_truncated_closedcone(float rl,float ru,float height, int facets)
{
	simob_t ct,cb,tc;
	tc.make_truncated_opencone(rl,ru,height,facets);
	ct.make_circle(ru,facets);
	cb.make_circle(rl,facets);
	add(ct);
	(mat44::trans(0.F,0.F,height)*mat44::rotatex((float)RPI)).transform(_gmodel.data(),_numvec);
	add(tc);
	add(cb);
	_invalidate_model = true;
	verify_simob();
}

void simob_t::make_opencyl(float radius,float height, int facets)
{
	make_truncated_opencone(radius,radius,height,facets);
	_invalidate_model = true;
}

void simob_t::make_closedcyl(float radius,float height, int facets)
{
	make_truncated_closedcone(radius,radius,height,facets);
	_invalidate_model = true;
	verify_simob();
}

void simob_t::make_opencone(float radius,float height, int facets, point3d_t* pnormu, point3d_t* pnorml)
{
	int i;
	vertex3d_t face[3];
	vertex3d_t tface[3];

	float angle = (float)(2.*RPI/(double)facets);
	mat44 mat = mat44::rotatez(angle/2);
	init(4*facets,facets);
	if (pnormu != nullptr)
		face[0] = vertex3d_t(0.F,0.F,height, pnormu->x, pnormu->y, pnormu->z);
	else
		face[0] = vertex3d_t(0.F,0.F,height,height,0.F,radius);

	mat.transform(face[0]);
	if (pnorml != nullptr)
		face[1] = vertex3d_t(radius, 0.F, 0.F, pnorml->x, pnorml->y, pnorml->z);
	else
		face[1] = vertex3d_t(radius, 0.F, 0.F, height, 0.F,radius);
	mat = mat44::rotatez(angle);
	mat.transform(face[2], face[1]);
	for (i=0;i<facets;i++)
	{
		_polycnts[_numpoly++] = 4;
		angle = (float)(2.*RPI)*(float)(facets-i)/(float)facets;
		mat44::rotatez(angle).transform(tface,face,3);
		_gmodel[_numvec++] = tface[0];
		_gmodel[_numvec++] = tface[1];
		_gmodel[_numvec++] = tface[2];
		_gmodel[_numvec++] = tface[0];
	}
	_invalidate_model = true;
	verify_simob();
}

void simob_t::make_closedcone(float radius,float height, int facets)
{
	simob_t cb;
	make_opencone(radius,height,facets);
	cb.make_circle(radius,facets);
	add(cb);
	_invalidate_model = true;
	verify_simob();
}

void simob_t::make_opendome(float r,int facets)
{
	simob_t t;
	int i,nd = facets/2;
	float rl,ru,h=0.F;
	double rat1,rat2;
	point3d_t normu, norml;
	for (i=nd;i>0;i--)
	{
		rat1 = (double)i/(double)nd*RPI/2.;
		rat2 = (double)(i-1)/(double)nd*RPI/2.;
		normu.x = (float)cos(rat1);
		normu.z = (float)sin(rat1);
		norml.x = (float)cos(rat2);
		norml.z = (float)sin(rat2);
		normu.y = norml.y = 0.f;
		h = r*(float)(sin(rat1) - sin(rat2) );
		mat44::trans(0.F,0.F,h).transform(_gmodel.data(),_numvec);
		ru = r*(float)cos(rat1);
		rl = r*(float)cos(rat2);
		if (i==nd)
		{
			normu = point3d_t{ 0.f, 0.f, 1.0f };
			t.make_opencone(rl,h,facets, &normu, &norml);
		}
		else
			t.make_truncated_opencone(rl,ru,h,facets, &normu, &norml);
		add(t);
	}
	_invalidate_model = true;
	verify_simob();
}

void simob_t::make_closeddome(float r,int facets)
{
	make_opendome(r,facets);
	simob_t t;
	t.make_circle(r,facets);
	add(t);
	_invalidate_model = true;
	verify_simob();
}

void simob_t::make_sphere(float r,int facets)
{
	simob_t t;
	make_opendome(r,facets);
	mat44::rotatex((float)RPI).transform(_gmodel.data(),_numvec);
	t.make_opendome(r,facets);
	add(t);
	_invalidate_model = true;
	verify_simob();
}

void simob_t::make_extrusion(simob_t *ps,float h)
{
	int i,np = ps->get_num_vec()-1;
	init(np*5+2*(np+1),np+2);
	ps->update_object();
	for (i=0;i<=np;i++)
	{
		_gmodel[_numvec] = ps->_gworld[i];
		_gmodel[_numvec].normal = point3d_t{};
		_numvec++;
	}
	mat44::trans(0.F,0.F,h).transform(_gmodel.data(),_numvec);
	_polycnts[_numpoly++] = np+1;
	for (i=0;i<=np;i++)
	{
		_gmodel[_numvec] = ps->_gworld[np-i];
		_gmodel[_numvec].normal = point3d_t{};
		_numvec++;
	}
	_polycnts[_numpoly++] = np+1;
	for (i=0;i<np;i++)
	{
		_gmodel[_numvec++] = ps->_gworld[i];
		_gmodel[_numvec++] = ps->_gworld[i + 1];
		_gmodel[_numvec] = ps->_gworld[i + 1];
		_gmodel[_numvec++].pt.z += h;
		_gmodel[_numvec] = ps->_gworld[i];
		_gmodel[_numvec++].pt.z += h;
		_gmodel[_numvec++] = ps->_gworld[i];
		_polycnts[_numpoly++] = 5;
	}
	_invalidate_model = true;
	verify_simob();
}

void simob_t::make_revolution(simob_t *ps, long facets)
{
	int i,j,np = ps->get_num_vec()-1;
	init(np*facets*5,np*facets);
	ps->update_object();
	std::vector<vertex3d_t> pt1(np + 1);
	std::vector<vertex3d_t> pt2(np + 1);

	_numvec = 0;
	_numpoly = 0;
	for (i=0;i<facets;i++)
	{
		float ang1 = (float)(2.*RPI)*(float)i/(float)facets;
		float ang2 = (float)(2.*RPI)*(float)(i+1)/(float)facets;
		for (j=0;j<=np;j++)
		{
			pt1[j] = ps->_gworld[j];
			pt2[j] = ps->_gworld[j];
		}
		mat44::rotatez(ang1).transform(pt1.data(), np+1);
		mat44::rotatez(ang2).transform(pt2.data(), np+1);
		for (j=0;j<np;j++)
		{
			int nCnt = 0;
			_gmodel[_numvec+nCnt] = pt1[j+1];
			nCnt++;
			_gmodel[_numvec+nCnt] = pt1[j];
			nCnt++;
			if (!(pt1[j] == pt2[j]))
			{
				_gmodel[_numvec+nCnt] = pt2[j];
				nCnt++;
			}
			_gmodel[_numvec+nCnt] = pt2[j+1];
			nCnt++;
			if (!(pt2[j+1] == pt1[j+1]))
			{
				_gmodel[_numvec+nCnt] = pt1[j+1];
				nCnt++;
			}
			_polycnts[_numpoly++] = nCnt;
			_numvec += nCnt;
		}
	}
	_invalidate_model = true;
}

void simob_t::calc_bounding_box()
{
	if (_numvec != 0)
	{
		float mnx, mxx, mny, mxy, mnz, mxz;
		mnx = mny = mnz = FLT_MAX;
		mxx = mxy = mxz = -FLT_MAX;
		for (int i = 0; i < _numvec; i++)
		{
			if (_gmodel[i].pt.x < mnx) mnx = _gmodel[i].pt.x;
			if (_gmodel[i].pt.x > mxx) mxx = _gmodel[i].pt.x;
			if (_gmodel[i].pt.y < mny) mny = _gmodel[i].pt.y;
			if (_gmodel[i].pt.y > mxy) mxy = _gmodel[i].pt.y;
			if (_gmodel[i].pt.z < mnz) mnz = _gmodel[i].pt.z;
			if (_gmodel[i].pt.z > mxz) mxz = _gmodel[i].pt.z;
		}
		_boxmodel.set_center((mnx + mxx) / 2.F, (mny + mxy) / 2.F, (mnz + mxz) / 2.F);
		_boxmodel.set_diagonal((mxx - mnx) / 2.F, (mxy - mny) / 2.F, (mxz - mnz) / 2.F);
	}
	else
	{
		_boxmodel.set_center(0.f, 0.f, 0.f);
		_boxmodel.set_diagonal(0.f, 0.f, 0.f);
	}
}

/**************************************************************
***************************************************************
*********************  Collision ******************************
***************************************************************
**************************************************************/

void simob_t::make_polys()
{
	int cnt = 0;
	for (int i=0;i<_numpoly;i++)
	{
		_polygons[i].make_poly(&_gmodel[cnt],_polycnts[i]-1);
		cnt += _polycnts[i];
	}
}

void polygon_t::make_poly(const vertex3d_t *p,int nv)
{
	ASSERT(nv != 0);
	if (nv != penalties.size())
		penalties.resize(nv);
	vec_ptr = p;

	point3d_t v1,v2;
	float d;
	v1 = vec_ptr[1]-vec_ptr[0];
	v2 = vec_ptr[2]-vec_ptr[1];
	norm = (v1*v2)/(v1.length()*v2.length());		/* (P1-P0)x(P2-P1)/(||P1-P0||||P2-P1||) */
	d = norm.length();
	if(fabs(d - 1.0) > 1.0e-5)
		norm /= d;
	nd = -vertex3d_t::dot(norm,vec_ptr[0]); /* -N.P0 */
	int i = 0;
	for (auto& p : penalties)
	{
		v1 = vec_ptr[i+1]-vec_ptr[i];
		p.normal = (norm * v1)/v1.length();
		p.distance = -vertex3d_t::dot(p.normal,vec_ptr[i]);
		++i;
	}
}

int polygon_t::point_in_polygon(const point3d_t &p)
{
	ASSERT(!penalties.empty());
	for (auto& pen : penalties)
	{
		if (pen.penalty(p) < 0.)
			return(0);
	}
	return(1);
}

int polygon_t::edge_in_polygon(const edge_t &e)
{
	float r1,r2,t;
	point3d_t pt;
	r1=residue(e.beg);
	r2=residue(e.end);
	if(r1*r2 >= 0.F)
		return(0);
	t = r1/(r1-r2);
	pt = e.beg + (e.end-e.beg)*t;
	return(point_in_polygon(pt));
}

int simob_t::check_collide(simob_t *X1,simob_t *X2)
{
	simob_t *X2B = X2;
	while (X1)
	{
		X2 = X2B;
		while (X2)
		{
			if (two_collide(X1,X2))
				return TRUE;

			if (X2->get_num_children() > 1)
			{
				size_t n = X2->get_num_children();
				for (size_t i = 0; i < n; i++)
					if (check_collide(X1,X2->get_child(i)))
						return TRUE;
				return FALSE;
			}
			X2 = X2->get_child(0);
		}
		if (X1->get_num_children() > 1)
		{
			size_t n = X1->get_num_children();
			for (size_t i = 0; i < n; i++)
			{
				if (check_collide(X1->get_child(i), X2B))
					return TRUE;
			}
			return FALSE;
		}
		X1 = X1->get_child(0);
	}
	return FALSE;
}

int simob_t::two_collide(simob_t *X1,simob_t *X2)
{
	mat44 mat;
	bounding_box_t bt;

	mat = X2->get_position().fastinverse()*X1->get_position();
	mat.transform(bt,X1->_boxmodel); // transform b1 to T2 system
	if(!bt.box_collide(X2->_boxmodel))
		return(0); /* boxes did not collide */

	/* boxes collided so check in other frame */

	mat = X1->get_position().fastinverse()*X2->get_position();
	mat.transform(bt,X2->_boxmodel);
	if(!bt.box_collide(X1->_boxmodel))
		return(0); /* boxes did not collide */

	/* boxes have definitely collided */
	return low_collision(X1,X2);
}

static vector<point3d_t> tr;
static vector<edge_t> re;

int simob_t::low_collision(simob_t *X1,simob_t *X2)
{
	int n1,n2,mn;
	mat44 mat;
	bounding_box_t bbox;
	int i,j,idx,ridx;

	n1 = X1->_numvec;
	n2 = X2->_numvec;
	mn = (n1 > n2) ? n1 : n2;
	if(mn > (int) tr.size())
	{
		tr.resize(mn);
		re.resize(mn);
	}

//
	mat = X2->get_position().fastinverse()*X1->get_position();
	mat.transform(tr.data(),X1->_gmodel.data(),n1);
	/* tr holds vector list #1 in coordinate system #2 */

	/* cycle through points and create reduced edge array -- re */
	for(i=idx=ridx=0; i< X1->_numpoly; i++)
	{
		for(j=0;j < X1->_polycnts[i]-1;j++,idx++)
			if (X2->_boxmodel.edge_int_box(tr[idx],tr[idx+1]))
				re[ridx++].set(tr[idx],tr[idx+1]);
		idx++;
	}
	mat.transform(bbox,X1->_boxmodel);
	if(X2->poly_edge(re.data(),ridx,bbox))
		return(1);

/////////////////

	mat = X1->get_position().fastinverse()*X2->get_position();
	mat.transform(tr.data(),X2->_gmodel.data(),n2);

	/* cycle through points and create reduced edge array -- re */
	idx = ridx = 0;
	for(i=idx=ridx=0; i< X2->_numpoly; i++)
	{
		for(j=0;j< X2->_polycnts[i]-1;j++,idx++)
			if (X1->_boxmodel.edge_int_box(tr[idx],tr[idx+1]))
				re[ridx++].set(tr[idx],tr[idx+1]);
		idx++;
	}

	mat.transform(bbox,X2->_boxmodel);
	if(X1->poly_edge(re.data(),ridx,bbox))
		return(1);

	return(0);
}

int simob_t::poly_edge(edge_t *re,int ne,bounding_box_t &bbox)
{
	int i,k;
	polygon_t *p;
	for(i=0; i < _numpoly;i++)
	{
		p = &_polygons[i];
		if(p->poly_int_box(bbox))
		{
			for(k=0; k<ne; k++)
				if(p->edge_in_polygon(re[k]))
					return(1);
		}
	}
	return(0);
}

bool simob_t::grasp(simob_t *ps)
{
	if (!_end)
		return false;
	if (ps->_is_graspee || _end->get_num_children())
		return false;
	mat44 tmat(_end->get_position().fastinverse()*ps->get_position());
	ps->_is_graspee = true;
	_end->_children.push_back(ps);
	ps->set_parent_mat(&(_end->_matpos));
	ps->set_position(tmat);
	return true;
}

simob_t *simob_t::release_grasp()
{
	if (!_end)
		return nullptr;
	if (!_end->get_num_children())
		return nullptr;
	simob_t *ps = _end->get_child();
	mat44 mat = ps->get_position();
	ps->_is_graspee = false;
	ps->set_parent_mat(nullptr);
	_end->_children.erase(_end->_children.begin());
	ps->set_position(mat);
	return ps;
}

// Compute end
// Determine inverse kinematics

bool simob_t::make_agent(simob_t *plast,int numlinks)
{
	_end = plast;
	std::vector<jointdef> joints(numlinks);
	simob_t *ps = get_child(0);
	int i=0;
	while (ps)
	{
		memcpy(joints[i].mat,ps->_matrel.m,sizeof(float)*16);
		joints[i].mn = ps->_minparm;
		joints[i].mx = ps->_maxparm;
		joints[i].type = (ps->_pjointfunc==nullptr) ? 0 :
				(ps->_pjointfunc==mat44::transz) ? 1 : 2;
		ps = ps->get_child(0);
		i++;
	}
	for (auto& ik : lisp_engine._invkin)
	{
		_inverse_kinematics = ik->compute_parameters(joints.data(), numlinks);
		if (_inverse_kinematics != nullptr)
		{
			_num_solns = _inverse_kinematics->get_num_solutions();
			return true;
		}
	}
	_inverse_kinematics = nullptr;
	return false;
}

solution *simob_t::get_solution(mat44 *pmat,int idx)
{
	if (!_inverse_kinematics)
		throw_eval_exception(NO_IK_AVAIL);
	if (idx == -1)
		idx = 0;
	if (idx < 0 || idx >= _num_solns)
		return nullptr;
	mat44 tmat = _matpos.fastinverse()*(*pmat);
	if (!_inverse_kinematics->inverse_kinematics(tmat.m))
		return nullptr;
	return &_inverse_kinematics->_solutions[idx];
}

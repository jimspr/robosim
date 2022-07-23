#include "stdafx.h"
#include <memory>
#include "matrix.h"
#include "grob.h"

using namespace std;

#define RPI 3.14159265359

float mat44::ident[4][4] = {
	{1.F,0.F,0.F,0.F},
	{0.F,1.F,0.F,0.F},
	{0.F,0.F,1.F,0.F},
	{0.F,0.F,0.F,1.F}
};

void mat44::print(ostream& ostr) const
{
	ostr << "<mat44";
	for (int i = 0; i < 16; i++)
		ostr << ' ' << m[i / 4][i % 4];
	ostr << ">";
}

void mat44::transform(point3d_t* dv, point3d_t* sv, int nv) const
{
	for (int i = 0; i < nv; i++, dv++, sv++)
		transform(*dv, *sv);
}

void mat44::transform(point3d_t* sv, int nv) const
{
	for (int i = 0; i < nv; i++, sv++)
		transform(*sv);
}

void mat44::transform(point3d_t& p) const
{
	point3d_t pt;
	transform(pt, p);
	p = pt;
}

void mat44::transform(point3d_t& pt, const point3d_t& p) const
{
	pt.x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3];
	pt.y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3];
	pt.z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3];
}

void mat44::transform(vertex3d_t* dv, vertex3d_t* sv, int nv) const
{
	for (int i = 0; i < nv; i++, dv++, sv++)
		transform(*dv, *sv);
}

void mat44::transform(vertex3d_t* sv, int nv) const
{
	for (int i = 0; i < nv; i++, sv++)
		transform(*sv);
}

void mat44::transform(vertex3d_t& p) const
{
	vertex3d_t pt;
	transform(pt, p);
	p = pt;
}

void mat44::transform(vertex3d_t& v1, const vertex3d_t& v2) const
{
	const auto& p2 = v2.pt;
	const auto& n2 = v2.normal;
	v1.pt.x = m[0][0] * p2.x + m[0][1] * p2.y + m[0][2] * p2.z + m[0][3];
	v1.pt.y = m[1][0] * p2.x + m[1][1] * p2.y + m[1][2] * p2.z + m[1][3];
	v1.pt.z = m[2][0] * p2.x + m[2][1] * p2.y + m[2][2] * p2.z + m[2][3];
	// Adjust the normal to be unit length if needed.
	if (n2 != point3d_t{})
	{
		ASSERT(fabs(n2.length() - 1.0) < EPS);
		v1.normal.x = m[0][0] * n2.x + m[0][1] * n2.y + m[0][2] * n2.z;
		v1.normal.y = m[1][0] * n2.x + m[1][1] * n2.y + m[1][2] * n2.z;
		v1.normal.z = m[2][0] * n2.x + m[2][1] * n2.y + m[2][2] * n2.z;
		v1.normal /= v1.normal.length();
	}
}

void mat44::transform(point3d_t* dv, vertex3d_t* sv, int nv) const
{
	for (int i = 0; i < nv; i++, dv++, sv++)
		transform(*dv, *sv);
}

void mat44::transform(point3d_t& dest, const vertex3d_t& src) const
{
	const auto& p = src.pt;
	dest.x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3];
	dest.y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3];
	dest.z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3];
}

void mat44::transform(bounding_box_t& db, bounding_box_t& sb) const
{
	transform(db.c, sb.c);
	dilate(db.hd, sb.hd);
}

void mat44::dilate(point3d_t& pt, point3d_t& p) const
{
	pt.x = (float)(fabs(m[0][0] * p.x) + fabs(m[0][1] * p.y) + fabs(m[0][2] * p.z));
	pt.y = (float)(fabs(m[1][0] * p.x) + fabs(m[1][1] * p.y) + fabs(m[1][2] * p.z));
	pt.z = (float)(fabs(m[2][0] * p.x) + fabs(m[2][1] * p.y) + fabs(m[2][2] * p.z));
}

void mat44::transform(CPoint* dv, point3d_t* sv, int numv) const
{
	int i;
	float z, h;
	for (i = 0; i < numv; i++, sv++, dv++)
	{
		z = sv->x * m[2][0] + sv->y * m[2][1] + sv->z * m[2][2] + m[2][3];
		if (z < 0.)
		{
			h = sv->x * m[3][0] + sv->y * m[3][1] + sv->z * m[3][2] + m[3][3];
			dv->x = (int)((sv->x * m[0][0] + sv->y * m[0][1] + sv->z * m[0][2] + m[0][3]) / h);
			dv->y = (int)((sv->x * m[1][0] + sv->y * m[1][1] + sv->z * m[1][2] + m[1][3]) / h);
		}
		else
			dv->x = dv->y = 0;
	}
}

void mat44::premultiply(const mat44& mat)
{
	mat44 res;
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			res.m[i][j] = mat.m[i][0] * m[0][j] +
			mat.m[i][1] * m[1][j] +
			mat.m[i][2] * m[2][j] +
			mat.m[i][3] * m[3][j];
	memcpy(m, res.m, sizeof(float) * 16);
}

void mat44::postmultiply(const mat44& mat)
{
	mat44 res;
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			res.m[i][j] = m[i][0] * mat.m[0][j] +
			m[i][1] * mat.m[1][j] +
			m[i][2] * mat.m[2][j] +
			m[i][3] * mat.m[3][j];
	memcpy(m, res.m, sizeof(float) * 16);
}


mat44 mat44::operator*(const mat44& mat) const
{
	mat44 res;
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			res.m[i][j] = m[i][0] * mat.m[0][j] +
			m[i][1] * mat.m[1][j] +
			m[i][2] * mat.m[2][j] +
			m[i][3] * mat.m[3][j];
	return res;
}

mat44 mat44::ROTATEX(float x)
{
	mat44 mat;
	mat.m[1][1] = mat.m[2][2] = (float)cos(x);
	mat.m[2][1] = (float)sin(x);
	mat.m[1][2] = (float)sin(-x);
	//	mat.m[1][2] = -mat.m[2][1];
	return mat;
}

mat44 mat44::ROTATEY(float y)
{
	mat44 mat;
	mat.m[0][0] = mat.m[2][2] = (float)cos(y);
	mat.m[0][2] = (float)sin(y);
	mat.m[2][0] = -mat.m[0][2];
	return mat;
}

mat44 mat44::ROTATEZ(float z)
{
	mat44 mat;
	mat.m[0][0] = mat.m[1][1] = (float)cos(z);
	mat.m[1][0] = (float)sin(z);
	mat.m[0][1] = -mat.m[1][0];
	return mat;
}

mat44 mat44::ROTATEZDEG(float z)
{
	mat44 mat;
	float rad = DEG2RAD(z);
	mat.m[0][0] = mat.m[1][1] = (float)cos(rad);
	mat.m[1][0] = (float)sin(rad);
	mat.m[0][1] = -mat.m[1][0];
	return mat;
}

mat44 mat44::TRANS(float x, float y, float z)
{
	mat44 mat;
	mat.m[0][3] = x;
	mat.m[1][3] = y;
	mat.m[2][3] = z;
	return mat;
}

mat44 mat44::TRANSX(float x)
{
	mat44 mat;
	mat.m[0][3] = x;
	return mat;
}

mat44 mat44::TRANSY(float y)
{
	mat44 mat;
	mat.m[1][3] = y;
	return mat;
}

mat44 mat44::TRANSZ(float z)
{
	mat44 mat;
	mat.m[2][3] = z;
	return mat;
}

mat44 mat44::TRANS(const point3d_t& pt)
{
	mat44 mat;
	mat.m[0][3] = pt.x;
	mat.m[1][3] = pt.y;
	mat.m[2][3] = pt.z;
	return mat;
}

mat44 mat44::FLIPX(void)
{
	mat44 mat;
	mat.m[0][0] = -1.F;
	return mat;
}

mat44 mat44::FLIPY(void)
{
	mat44 mat;
	mat.m[1][1] = -1.F;
	return mat;
}

mat44 mat44::FLIPZ(void)
{
	mat44 mat;
	mat.m[2][2] = -1.F;
	return mat;
}

mat44 mat44::SCALE(float x, float y, float z)
{
	mat44 mat;
	mat.m[0][0] = x;
	mat.m[1][1] = y;
	mat.m[2][2] = z;
	mat.m[3][3] = 1.F;
	return mat;
}

mat44 mat44::PERSPECTIVE(float f)
{
	mat44 mat;
	mat.m[3][2] = -1.F / f;
	return mat;
}

mat44 mat44::EULER(const euler_zyz_t& e)
{
	return ROTATEZ(e.phi) * ROTATEY(e.theta) * ROTATEZ(e.psi);
}

mat44 mat44::RPY(const euler_rpy_t& e)
{
	return ROTATEZ(e.roll) * ROTATEY(e.pitch) * ROTATEX(e.yaw);
}

mat44 mat44::CYL(float z, float alpha, float r)
{
	return TRANS(r * (float)cos(alpha), r * (float)sin(alpha), z);
}

mat44 mat44::SPH(float alpha, float beta, float r)
{
	return TRANS(r * (float)cos(alpha) * (float)sin(beta), r * (float)sin(alpha) * (float)sin(beta), r * (float)cos(beta));
}

mat44 mat44::DH(float theta, float d, float a, float alpha)
{
	return ROTATEZ(theta) * TRANS(a, 0.F, d) * ROTATEX(alpha);
}

euler_zyz_t mat44::GetEulerZYZ()
{
	CheckZero();
	double phi, theta, psi;
	theta = atan2(sqrt(m[0][2] * m[0][2] + m[1][2] * m[1][2]), m[2][2]); // theta 0 -- PI
	if (fabs(sin(theta)) < EPS)
	{
		psi = 0.;
		if (theta < RPI / 2.)
			phi = atan2(m[1][0], m[0][0]);
		else
			phi = atan2(-m[1][0], -m[0][0]);
	}
	else
	{
		phi = atan2(m[1][2], m[0][2]);
		psi = atan2(m[2][1], -m[2][0]);
	}
	return euler_zyz_t{ (float)phi,(float)theta,(float)psi };
}

void mat44::get_rpy(point3d_t& pt) const
{
	double phi, theta, psi;
	theta = atan2(-m[2][0], sqrt(m[2][1] * m[2][1] + m[2][2] * m[2][2])); // -PI/2 -- PI/2
	if (fabs(cos(theta)) < EPS)
	{
		psi = 0.;
		phi = atan2(m[1][2], m[0][2]);
	}
	else
	{
		phi = atan2(m[1][0], m[0][0]);
		psi = atan2(m[2][1], m[2][2]);
	}
	pt = point3d_t{ (float)phi, (float)theta, (float)psi };
}

void mat44::get_sph(point3d_t& pt) const
{
	float alpha;
	float beta;
	float r;
	if (fabs(m[1][3]) + fabs(m[0][3]) < EPS)
		alpha = 0.F;
	else
		alpha = (float)atan2(m[1][3], m[0][3]);
	r = (float)sqrt(m[0][3] * m[0][3] + m[1][3] * m[1][3] + m[2][3] * m[2][3]);
	beta = (float)sqrt(m[0][3] * m[0][3] + m[1][3] * m[1][3]);
	if (fabs(beta) + fabs(m[2][3]) > EPS)
		beta = (float)atan2(sqrt(m[0][3] * m[0][3] + m[1][3] * m[1][3]), m[2][3]);
	pt = point3d_t{ alpha, beta, r };
}

void mat44::get_cyl(point3d_t& pt) const
{
	float alpha = 0.F;
	float r = 0.F;
	if (fabs(m[1][3]) + fabs(m[0][3]) > EPS)
		alpha = (float)atan2(m[1][3], m[0][3]);
	r = (float)sqrt(m[0][3] * m[0][3] + m[1][3] * m[1][3]);
	pt = point3d_t{ m[2][3], alpha, r };
}

void mat44::get_rect(point3d_t& pt) const
{
	pt = point3d_t{ m[0][3], m[1][3], m[2][3] };
}

bool mat44::get_dh(dh_t& dh) const
{
	if (m[0][0] > m[1][0]) // cos(theta) > sin(theta)
	{
		dh.da = m[0][3] / m[0][0];
		dh.dz = m[2][3];
	}
	else
	{
		dh.da = m[1][3] / m[1][0];
		dh.dz = m[2][3];
	}

	if (fabs(m[2][0]) > EPS) // Rotation about Y
		return false;
	if (fabs(m[0][0] * m[1][3] - m[1][0] * m[0][3]) > EPS) // translation along Y
		return false;
	dh.theta = (float)atan2(m[1][0], m[0][0]);
	dh.alpha = (float)atan2(m[2][1], m[2][2]);
	return true;
}

mat44 mat44::fastinverse(void) const
{
	mat44 t;
	int i, j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			t.m[i][j] = m[j][i];
			t.m[j][3] = -(m[0][j] * m[0][3] + m[1][j] * m[1][3] + m[2][j] * m[2][3]);
		}
	return t;
}

/*******************************************************************
	The transformation matrix of the camera coordinate system
	according to the conventions of Paul, etc. is
	R = T(fx,fy,fz)RZ(beta)RY(alpha)RZ(gamma)RZ(twist).
	This makes the z axis of the camera point away from the
	point the camera is looking at. Displayable points, then, will
	have a negative z value. (I still need to mirror the y axis
	across the x axis. This will be to establish the axes for display.)
	RZ(gamma) is used to keep the camera aligned up.
	Gamma is 90 degrees except when the view is aligned along the z
	axis, when it becomes 0.

	This matrix R describes the coordinate system of the camera
	relative to the coordinate system of the world. This matrix,
	when multiplied by a point specified in the camera coordinate
	system will give the point specified in the world system.
	THIS IS THE OPPOSITE OF WHAT I WANT.
	I want to transform points from the world system to the coordinate
	system of the camera. To get this transformation, I merely take
	the inverse of this transformation.
	 -1     -1       -1       -1       -1     -1
	R   = RZ(twist)RZ(gamma)RY(alpha)RZ(beta)T(fx,fy,fz)

	 -1
	R   = RZ(-twist)RZ(-gamma)RY(-alpha)RZ(-beta)T(-fx,-fy,-fz)

******************************************************************/

mat44 mat44::viewpoint(const point3d_t& f, const point3d_t& t, float twist, float zoom, float focal_length, int winw, int winh)
{
	float beta, alpha, gamma;
	float rtwist, zm;
	point3d_t a;

	a = f - t;

	if ((a.x < .1) && (a.x > -.1) && (a.y < .1) && (a.y > -.1)) // aligned along z
	{
		beta = gamma = 0.F;
		alpha = (a.z > 0.F) ? 0.F : (float)RPI;
	}
	else
	{
		gamma = (float)(RPI / 2.); // 90 degrees
		beta = (float)atan2(a.y, a.x);
		alpha = (float)atan2(sqrt(a.x * a.x + a.y * a.y), a.z);
	}
	rtwist = DEG2RAD(twist);
	zm = zoom / 10.F;
	return
		TRANS((float)(winw / 2), (float)(winh / 2), 0.F) *
		SCALE(zm, -zm, zm) *
		PERSPECTIVE(focal_length) *
		ROTATEZ(-rtwist - gamma) *
		ROTATEY(-alpha) *
		ROTATEZ(-beta) *
		TRANS(-f.x, -f.y, -f.z);
}

#ifdef DASFASF
mat44 mat44::viewpoint(const point3d_t& f, const point3d_t& t, float twist, float zoom, float focal_length, int winw, int winh, float eyeoff)
{
	float beta, alpha, gamma;
	float rtwist, zm;
	float r, theta;
	point3d_t a;

	a = f - t;

	if ((a.x < .1) && (a.x > -.1) && (a.y < .1) && (a.y > -.1)) // aligned along z
	{
		beta = gamma = 0.F;
		alpha = (a.z > 0.F) ? 0.F : (float)RPI;
	}
	else
	{
		gamma = (float)(RPI / 2.); // 90 degrees
		beta = (float)atan2(a.y, a.x);
		alpha = (float)atan2(sqrt(a.x * a.x + a.y * a.y), a.z);
	}
	rtwist = twist * (float)(RPI / 180.);
	zm = zoom / 10.F;
	theta = (float)atan2(eyeoff, sqrt(a.x * a.x + a.y * a.y));
	r = (float)sqrt(eyeoff * eyeoff + a.x * a.x + a.y * a.y);
	a.x = r * (float)cos(beta + theta);
	a.y = r * (float)sin(beta + theta);
	a += t;

	return
		TRANS((float)(winw / 2), (float)(winh / 2), 0.F) *
		SCALE(zm, -zm, zm) *
		PERSPECTIVE(focal_length) *
		//		TRANS(eyeoff,0.F,0.F)*
		ROTATEZ(-rtwist - gamma) *
		ROTATEY(-alpha) *
		ROTATEZ(-(beta + theta)) *
		TRANS(-a.x, -a.y, -a.z);
}
#endif

mat44 mat44::viewpoint(const point3d_t& f, const point3d_t& t, float twist, float zoom, float focal_length, int winw, int winh, float eyeoff)
{
	float beta, alpha, gamma;
	float rtwist, zm;
	point3d_t a;

	a = f - t;

	if ((a.x < .1) && (a.x > -.1) && (a.y < .1) && (a.y > -.1)) // aligned along z
	{
		beta = gamma = 0.F;
		alpha = (a.z > 0.F) ? 0.F : (float)RPI;
	}
	else
	{
		gamma = (float)(RPI / 2.); // 90 degrees
		beta = (float)atan2(a.y, a.x);
		alpha = (float)atan2(sqrt(a.x * a.x + a.y * a.y), a.z);
	}
	rtwist = DEG2RAD(twist);
	zm = zoom / 10.F;
	return
		TRANS((float)(winw / 2), (float)(winh / 2), 0.F) *
		SCALE(zm, -zm, zm) *
		PERSPECTIVE(focal_length) *
		TRANS(eyeoff, 0.F, 0.F) *
		ROTATEZ(-rtwist - gamma) *
		ROTATEY(-alpha) *
		ROTATEZ(-beta) *
		TRANS(-f.x, -f.y, -f.z);
}

bounding_box_t bounding_box_t::union_with(const bounding_box_t& bx) const
{
	float mnx, mxx;
	float mny, mxy;
	float mnz, mxz;
	mnx = min(c.x - hd.x, bx.c.x - bx.hd.x);
	mxx = max(c.x + hd.x, bx.c.x + bx.hd.x);
	mny = min(c.y - hd.y, bx.c.y - bx.hd.y);
	mxy = max(c.y + hd.y, bx.c.y + bx.hd.y);
	mnz = min(c.z - hd.z, bx.c.z - bx.hd.z);
	mxz = max(c.z + hd.z, bx.c.z + bx.hd.z);
	bounding_box_t u(
		point3d_t{ (mnx + mxx) / 2.F, (mny + mxy) / 2.F, (mnz + mxz) / 2.F },
		point3d_t{(mxx - mnx) / 2.F, (mxy - mny) / 2.F, (mxz - mnz) / 2.F}
	);
	return u;
}

int bounding_box_t::box_collide(const bounding_box_t& b) const
{
	if (fabs(c.x - b.c.x) >= hd.x + b.hd.x) /* return false if centers separated enough */
		return(0);
	if (fabs(c.y - b.c.y) >= hd.y + b.hd.y) /* return false if centers separated enough */
		return(0);
	if (fabs(c.z - b.c.z) >= hd.z + b.hd.z) /* return false if centers separated enough */
		return(0);
	return(1); /*return true: centers not far enough apart */
}

int bounding_box_t::edge_int_box(const point3d_t& p1, const point3d_t& p2) const
{
	FLOAT t1, t2, u1, l1, u2, l2;
	point3d_t pt1, pt2;
	pt1 = p1 - c;
	pt2 = p2 - p1;
	t1 = pt1.x;
	t2 = pt2.x;

	if (t2 == 0.)
	{
		if (fabs(t1) > hd.x)
			return(0);
		u1 = (FLOAT)1.;
		l1 = (FLOAT)0.;
	}
	else if (t2 > 0.0)
	{
		u1 = (hd.x - t1) / t2;
		l1 = (-hd.x - t1) / t2;
	}
	else
	{
		l1 = (hd.x - t1) / t2;
		u1 = (-hd.x - t1) / t2;
	}
	t1 = pt1.y;
	t2 = pt2.y;
	if (t2 == 0.)
	{
		if (fabs(t1) > hd.y)
			return(0);
		u2 = (FLOAT)1.;
		l2 = (FLOAT)0.;
	}
	else if (t2 > 0.0)
	{
		u2 = (hd.y - t1) / t2;
		l2 = (-hd.y - t1) / t2;
	}
	else
	{
		l2 = (hd.y - t1) / t2;
		u2 = (-hd.y - t1) / t2;
	}
	u1 = min(u1, u2);
	l1 = max(l1, l2);
	t1 = pt1.z;
	t2 = pt2.z;
	if (t2 == 0.)
	{
		if (fabs(t1) > hd.z)
			return(0);
		u2 = (FLOAT)1.;
		l2 = (FLOAT)0.;
	}
	else if (t2 > 0.0)
	{
		u2 = (hd.z - t1) / t2;
		l2 = (-hd.z - t1) / t2;
	}
	else
	{
		l2 = (hd.z - t1) / t2;
		u2 = (-hd.z - t1) / t2;
	}
	u1 = min(u1, u2);
	l1 = max(l1, l2);
	if (u1 >= l1)
		return(1);
	return(0);
}


BOOL mat44::GetDH(float pt[4])
{
	if (fabs(m[0][2]) > EPS) // Rotation about Y
		return FALSE;
	if (fabs(m[1][2] * m[2][3] - m[1][3] * m[2][2]) > EPS) // translation along Y
		return FALSE;
	pt[0] = (float)atan2((-m[1][2]), m[2][2]);
	pt[2] = (float)atan2((-m[0][1]), m[0][0]);
	pt[1] = m[0][3];
	if (m[2][2] > m[1][2]) // cos(theta) > sin(theta)
		pt[3] = m[2][3] / m[2][2];
	else
		pt[3] = m[1][3] / m[1][2];
	return TRUE;
}

void mat44::CheckZero()
// Check every element of matrix for value near zero and make it zero if close enough.
{
	if (fabs(m[0][0]) < EPS)
		m[0][0] = .0f;
	if (fabs(m[0][1]) < EPS)
		m[0][1] = .0f;
	if (fabs(m[0][2]) < EPS)
		m[0][2] = .0f;
	if (fabs(m[0][3]) < EPS)
		m[0][3] = .0f;
	if (fabs(m[1][0]) < EPS)
		m[1][0] = .0f;
	if (fabs(m[1][1]) < EPS)
		m[1][1] = .0f;
	if (fabs(m[1][2]) < EPS)
		m[1][2] = .0f;
	if (fabs(m[1][3]) < EPS)
		m[1][3] = .0f;
	if (fabs(m[2][0]) < EPS)
		m[2][0] = .0f;
	if (fabs(m[2][1]) < EPS)
		m[2][1] = .0f;
	if (fabs(m[2][2]) < EPS)
		m[2][2] = .0f;
	if (fabs(m[2][3]) < EPS)
		m[2][3] = .0f;
	if (fabs(m[3][0]) < EPS)
		m[3][0] = .0f;
	if (fabs(m[3][1]) < EPS)
		m[3][1] = .0f;
	if (fabs(m[3][2]) < EPS)
		m[3][2] = .0f;
	if (fabs(m[3][3]) < EPS)
		m[3][3] = .0f;

}

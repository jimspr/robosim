#pragma once
#ifndef __MATRIX_H
#define __MATRIX_H

#include "node.h"

#define EPS 1.0E-5

/* Denavit-Hartenberg parameters. */
struct dh_t
{
	float theta;
	float dz;
	float da;
	float alpha;
};

struct euler_zyz_t
{
	float phi = 0.f;;
	float theta = 0.f;;
	float psi = 0.f;;
	euler_zyz_t operator+(const euler_zyz_t& pt) const { return euler_zyz_t{ phi + pt.phi, theta + pt.theta, psi + pt.psi }; }
	euler_zyz_t operator-(const euler_zyz_t& pt) const { return euler_zyz_t{ phi - pt.phi, theta - pt.theta, psi - pt.psi }; }
	euler_zyz_t operator/(float d) const { return euler_zyz_t{ phi / d, theta / d, psi / d }; }
	euler_zyz_t operator*(float d) const { return euler_zyz_t{ phi * d, theta * d, psi * d }; }
	void operator/=(float d)
	{
		phi /= d;
		theta /= d;
		psi /= d;
	}
	euler_zyz_t& operator+=(const euler_zyz_t& e)
	{
		phi += e.phi;
		theta += e.theta;
		psi += e.psi;
		return *this;
	}
};

struct euler_rpy_t
{
	float roll = 0.f;;
	float pitch = 0.f;;
	float yaw = 0.f;;
	euler_rpy_t operator+(const euler_rpy_t& e) const { return euler_rpy_t{ roll + e.roll, pitch + e.pitch, yaw + e.yaw }; }
	euler_rpy_t operator-(const euler_rpy_t& e) const { return euler_rpy_t{ roll - e.roll, pitch - e.pitch, yaw - e.yaw }; }
	euler_rpy_t operator/(float d) const { return euler_rpy_t{ roll / d, pitch / d, yaw / d }; }
	euler_rpy_t operator*(float d) const { return euler_rpy_t{ roll * d, pitch * d, yaw * d }; }
	void operator/=(float d)
	{
		roll /= d;
		pitch /= d;
		yaw /= d;
	}
	euler_rpy_t& operator+=(const euler_rpy_t& e)
	{
		roll += e.roll;
		pitch += e.pitch;
		yaw += e.yaw;
		return *this;
	}
};

/* Denavit-Hartenburg parameters */
struct dh_params
{
	float dz;
	float da;
	float theta;
	float alpha;
};

class point3d_t
{
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	point3d_t(float a,float b,float c) : x(a), y(b), z(c) {}
	point3d_t() = default;
	void offset(const point3d_t &pt) {x+=pt.x;y+=pt.y;z+=pt.z;}
	void normalize()
	{
		float len = length();
		if (fabs(len - 1.0) > EPS)
			*this /= len;
	}
	point3d_t operator+(const point3d_t& pt) const { return point3d_t{ x + pt.x,y + pt.y,z + pt.z }; }
	point3d_t operator-(const point3d_t & pt) const { return point3d_t{ x - pt.x,y - pt.y,z - pt.z }; }
	float length() const {return (float)sqrt(x*x + y*y + z*z);}
	static float dipr(const point3d_t &p1,const point3d_t &p2) {return (float)(fabs(p1.x*p2.x) + fabs(p1.y*p2.y) + fabs(p1.z*p2.z));}
	static float dot(const point3d_t &p1,const point3d_t &p2) {return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;}
	point3d_t operator*(const point3d_t &p2) const // Cross product
	{
		return point3d_t{ (y * p2.z - z * p2.y) , (z * p2.x - x * p2.z) , (x * p2.y - y * p2.x) };
	}
	point3d_t operator/(float d) const { return point3d_t{ x / d,y / d,z / d }; }
	point3d_t operator*(float d) const { return point3d_t{ x * d,y * d,z * d }; }
	point3d_t& operator/=(float d)
	{
		x /= d;
		y /= d;
		z /= d;
		return *this;
	}
	bool operator==(const point3d_t& pt) const {return memcmp(this, &pt, sizeof(point3d_t))==0;}
	bool operator!=(const point3d_t& pt) const { return !memcmp(this, &pt, sizeof(point3d_t)) == 0; }
	point3d_t &operator+=(const point3d_t &pt)
	{
		x += pt.x;
		y += pt.y;
		z += pt.z;
		return *this;
	}
};

class vertex3d_t
{
public:
	point3d_t pt;
	point3d_t normal;
	vertex3d_t() = default;
	vertex3d_t(const point3d_t& p, const point3d_t& n) : pt(p), normal(n) {}
	vertex3d_t(float a, float b, float c, float d, float e, float f)
	{
		pt = point3d_t{ a, b, c };
		normal = point3d_t{ d, e, f };
	}
	point3d_t operator-(const vertex3d_t &p) const {return pt - p.pt;}
	bool operator==(const vertex3d_t& p) {return memcmp(this, &p, sizeof(vertex3d_t))==0;}
	static float dot(const point3d_t& p1, const vertex3d_t& p2)
	{
		return point3d_t::dot(p1, p2.pt);
	}
};

class bounding_box_t
{
public:
	point3d_t c;
	point3d_t hd;
	bounding_box_t(const point3d_t &cen,const point3d_t &half) { c = cen;hd = half;}
	bounding_box_t() {}
	void offset(const point3d_t &pt) {c.offset(pt);}
	void set_center(const point3d_t &pt) {c = pt;}
	void set_center(float x,float y,float z) {c.x = x;c.y=y;c.z=z;}
	void set_diagonal(const point3d_t &pt) {hd = pt;}
	void set_diagonal(float x,float y,float z) {hd.x = x;hd.y=y;hd.z=z;}
	bounding_box_t union_with(const bounding_box_t &bx) const;
	int box_collide(const bounding_box_t &b) const;
	int edge_int_box(const point3d_t &p1,const point3d_t &p2) const;
	point3d_t Dimensions() const { return point3d_t{ 2.F * hd.x,2.F * hd.y,2.F * hd.z }; }
};

class mat44 final : public node_t
{
public:
	float m[4][4] = {
		{1.f,0.f,0.f,0.f},
		{0.f,1.f,0.f,0.f},
		{0.f,0.f,1.f,0.f},
		{0.f,0.f,0.f,1.f}
	};

	mat44() : node_t(TYPE_MAT44)
	{
	}

	mat44(const float t[4][4]) : node_t(TYPE_MAT44)
	{
		memcpy(m, t, 16 * sizeof(float));
	}

	static bool verify(node_t* p) { return p->get_type() == TYPE_MAT44; }
	void check_zero();
	bool get_dh(float pt[4]);

	void print(std::ostream&) const;
	void zero() { memset(m, 0, 16 * sizeof(float)); }
	// Statics
	static mat44 rotatex(float x);
	static mat44 rotatey(float y);
	static mat44 rotatez(float z);
	static mat44 rotatezdeg(float z);
	//	static mat44 rotate(float x,float y,float z);
	static mat44 transx(float x);
	static mat44 transy(float y);
	static mat44 transz(float z);
	static mat44 trans(float x, float y, float z);
	static mat44 trans(const point3d_t& pt);
	static mat44 scale(float x, float y, float z);
	static mat44 flipx();
	static mat44 flipy();
	static mat44 flipz();
	static mat44 perspective(float f);
	static mat44 euler(const euler_zyz_t& e);
	static mat44 rpy(const euler_rpy_t& e);
	static mat44 cyl(float z, float alpha, float r);
	static mat44 sph(float alpha, float beta, float r);
	static mat44 dh(float theta, float d, float a, float alpha);
	static mat44 viewpoint(const point3d_t& f, const point3d_t& t, float twist, float zoom, float focal_length, int winw, int winh);
	static mat44 viewpoint(const point3d_t& f, const point3d_t& t, float twist, float zoom, float focal_length, int winw, int winh, float eyeoff);

	euler_zyz_t get_euler_zyz();
	void get_rpy(point3d_t& pt) const;
	void get_cyl(point3d_t& pt) const;
	void get_sph(point3d_t& pt) const;
	void get_rect(point3d_t& pt) const;
	bool get_dh(dh_t& dh) const;

	mat44 operator*(const mat44& mat) const;
	void premultiply(const mat44& mat);
	void postmultiply(const mat44& mat);

	void rotatex_ref(float x) { premultiply(rotatex(x)); }
	void rotatey_ref(float y) { premultiply(rotatey(y)); }
	void rotatez_ref(float z) { premultiply(rotatez(z)); }
	void trans_ref(float x, float y, float z) { premultiply(trans(x, y, z)); }
	void scale_ref(float x, float y, float z) { premultiply(scale(x, y, z)); }
	void flipx_ref() { premultiply(flipx()); }
	void flipy_ref() { premultiply(flipy()); }
	void flipz_ref() { premultiply(flipz()); }

	void rotatex_loc(float x) { postmultiply(rotatex(x)); }
	void rotatey_loc(float y) { postmultiply(rotatey(y)); }
	void rotatez_loc(float z) { postmultiply(rotatez(z)); }
	void trans_loc(float x, float y, float z) { postmultiply(trans(x, y, z)); }
	void scale_loc(float x, float y, float z) { postmultiply(scale(x, y, z)); }
	void flipx_loc() { postmultiply(flipx()); }
	void flipy_loc() { postmultiply(flipy()); }
	void flipz_loc() { postmultiply(flipz()); }

	void transform(point3d_t* dv, point3d_t* sv, int nv) const;
	void transform(point3d_t* sv, int nv) const;
	void transform(point3d_t& dest, const point3d_t& src) const;
	void transform(point3d_t& src) const;
	//	void transform(CPoint* dv, point3d_t* sv, int numv) const;
	void transform(vertex3d_t* dv, vertex3d_t* sv, int nv) const;
	void transform(vertex3d_t* sv, int nv) const;
	void transform(vertex3d_t& dest, const vertex3d_t& src) const;
	void transform(vertex3d_t& src) const;

	void transform(point3d_t* dv, vertex3d_t* sv, int nv) const;
	void transform(point3d_t& dest, const vertex3d_t& src) const;

	void transform(bounding_box_t& db, bounding_box_t& sb) const;
	void dilate(point3d_t& pt, point3d_t& p) const;
	mat44 fastinverse() const;
	//
	DECLARE_NODE(mat44, 128)
};

#endif

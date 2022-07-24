#pragma once
#ifndef __IK_H
#define __IK_H

#include "node.h"

struct jointdef
{
	float mat[4][4];
	float mn, mx;
	int type; //0-fixed, 1-prismatic, 2-revolute
};

struct solution
{
	int degrees;
	float values[16]{};
	void init(int n)
	{
		ASSERT(n < 16);
		degrees = n;
	}
	float& operator [] (size_t i) { return values[i]; }
};

typedef float fmat44[4][4];
typedef int (*PFINVKIN)(char* pbuf, fmat44 mat, solution* q);
typedef int (*PFG)();
typedef int (*PFCP)(char* buf, jointdef* joints, int numjoints);

class function;

class ik_interface
{
public:
	std::vector<solution> _solutions;
	virtual ~ik_interface() {}
	virtual bool is_ok() = 0;
	virtual int get_num_solutions() = 0;
	virtual int get_degrees_of_freedom() = 0;
	virtual int inverse_kinematics(fmat44 mat) = 0;
	virtual std::unique_ptr<ik_interface> compute_parameters(jointdef* joints, int numjoints) = 0;
	virtual void mark_in_use() = 0;
	void alloc_solutions();
};

class ik_dll_t : public ik_interface
{
public:
	ik_dll_t(const std::string& libname);
	virtual ~ik_dll_t();
	virtual bool is_ok();
	virtual int get_num_solutions();
	virtual int get_degrees_of_freedom();
	virtual int inverse_kinematics(fmat44 mat);
	virtual std::unique_ptr<ik_interface> compute_parameters(jointdef* joints, int numjoints);
	virtual void mark_in_use();

protected:
	std::vector<char> _parameters;
	HINSTANCE _handle_dll;
	std::string _lib_name;
	PFINVKIN _fn_inverse_kinematics;
	PFG _fn_get_buffer_size;
	PFG _fn_get_num_solutions;
	PFG _fn_get_degrees_of_freedom;
	PFCP _fn_compute_parameters;
};

class ik_lisp_t : public ik_interface
{
public:
	ik_lisp_t(function* pIK, function* pGNS, function* pGD, function* pCP);
	virtual bool is_ok();
	virtual int get_num_solutions();
	virtual int get_degrees_of_freedom();
	virtual int inverse_kinematics(fmat44 mat);
	virtual std::unique_ptr<ik_interface> compute_parameters(jointdef* joints, int numjoints);
	virtual void mark_in_use();

protected:
	node_t* _parameters;
	function* _fn_inverse_kinematics;
	function* _fn_get_num_solutions;
	function* _fn_get_degrees_of_freedom;
	function* _fn_compute_parameters;
};

extern std::vector<std::unique_ptr<ik_interface>> invkin;

#endif

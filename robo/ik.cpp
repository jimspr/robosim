#include "stdafx.h"
#include <memory>

#include "ik.h"
#include "node.h"
#include "matrix.h"

using namespace std;

void ik_interface::alloc_solutions()
{
	int nDeg = get_degrees_of_freedom();
	int nSol = get_num_solutions();
	_solutions.resize(nSol);
	for (auto& sol : _solutions)
		sol.init(nDeg);
}

ik_dll_t::ik_dll_t(const std::string& libname) : _lib_name(libname)
{
	if (_handle_dll = LoadLibrary(_lib_name.c_str()))
	{
		_fn_inverse_kinematics = (PFINVKIN) GetProcAddress(_handle_dll,"InverseKinematics");
		_fn_get_buffer_size = (PFG) GetProcAddress(_handle_dll,"GetBufferSize");
		_fn_get_num_solutions = (PFG) GetProcAddress(_handle_dll,"GetNumSolutions");
		_fn_get_degrees_of_freedom = (PFG) GetProcAddress(_handle_dll,"GetDegreesOfFreedom");
		_fn_compute_parameters = (PFCP) GetProcAddress(_handle_dll,"ComputeParameters");
	}
}

ik_dll_t::~ik_dll_t()
{
	FreeLibrary(_handle_dll);
}

bool ik_dll_t::is_ok()
{
	return
		_handle_dll && 
		_fn_inverse_kinematics && 
		_fn_get_buffer_size && 
		_fn_get_num_solutions && 
		_fn_compute_parameters;
}

int ik_dll_t::get_num_solutions()
{
	return _fn_get_num_solutions();
}

int ik_dll_t::get_degrees_of_freedom()
{
	return _fn_get_degrees_of_freedom();
}

int ik_dll_t::inverse_kinematics(fmat44 mat)
{
	ASSERT(!_parameters.empty());
	ASSERT(!_solutions.empty());
	return _fn_inverse_kinematics(&_parameters[0], mat, &_solutions[0]);
}

std::unique_ptr<ik_interface> ik_dll_t::compute_parameters(jointdef* joints, int numjoints)
{
	std::vector<char> buf(_fn_get_buffer_size(), 0);
	int nRet = _fn_compute_parameters(&buf[0], joints, numjoints);
	if (nRet == 0)
	{
		return nullptr;
	}
	auto pIK = make_unique<ik_dll_t>(_lib_name);
	ASSERT(pIK->is_ok());
	pIK->_parameters = std::move(buf);
	pIK->alloc_solutions();
	return pIK;
}

void ik_dll_t::mark_in_use()
{
}

ik_lisp_t::ik_lisp_t(function* pIK, function* pGNS, function* pGD, function* pCP)
{
	_fn_inverse_kinematics = pIK;
	_fn_get_num_solutions = pGNS;
	_fn_get_degrees_of_freedom = pGD;
	_fn_compute_parameters = pCP;
	_parameters = NULL;
}

bool ik_lisp_t::is_ok()
{
	return
		_fn_inverse_kinematics && 
		_fn_get_num_solutions && 
		_fn_get_degrees_of_freedom &&
		_fn_compute_parameters;
}

int ik_lisp_t::get_num_solutions()
{
	node_t *pRet = _fn_get_num_solutions->evalnoargs((cons_t*)nil);
	if (pRet->is_a(TYPE_LONG))
		return (int) (long) *(number_node_t*)pRet;
	else
		return 0;
}

int ik_lisp_t::get_degrees_of_freedom()
{
	node_t *pRet = _fn_get_degrees_of_freedom->evalnoargs((cons_t*)nil);
	if (pRet->is_a(TYPE_LONG))
		return (int) (long) *(number_node_t*)pRet;
	else
		return 0;
}

int ik_lisp_t::inverse_kinematics(fmat44 mat)
{
	ASSERT(_parameters != NULL);
	ASSERT(!_solutions.empty());
	cons_t* args = new cons_t(_parameters, new cons_t(new mat44(mat), nil));
	cons_t *pRet = (cons_t*)_fn_inverse_kinematics->evalnoargs(args);
	if ((node_t*)pRet == (node_t*)nil)
		return 0;
	ASSERT(pRet->is_a(TYPE_CONS));
	ASSERT(pRet->get_num_items() == get_num_solutions());
	int nSol = 0;
	int nVar = 0;
	while (pRet->is_a(TYPE_CONS))
	{
		cons_t* pSol = pRet->CarCONS();
		ASSERT(pSol->is_a(TYPE_CONS));
		nVar = 0;
		while (pSol->is_a(TYPE_CONS))
		{
			ASSERT(pSol->Car()->is_a_number());
			_solutions[nSol][nVar] = (float)*(number_node_t*)(pSol->Car());
			nVar++;
			pSol = pSol->CdrCONS();
		}
		nSol++;
		pRet = pRet->CdrCONS();
	}
	return nSol;
}

std::unique_ptr<ik_interface> ik_lisp_t::compute_parameters(jointdef* joints, int numjoints)
{
	cons_t* pArgs = (cons_t*)nil;
	while (numjoints--)
	{
		cons_t *pJoint = 
			new cons_t(new mat44(joints[numjoints].mat),
			new cons_t(new number_node_t(joints[numjoints].mn),
			new cons_t(new number_node_t(joints[numjoints].mx),
			new cons_t(new number_node_t((long)joints[numjoints].type), nil))));
		pArgs = new cons_t(pJoint ,pArgs);
	}
	node_t *pRet = _fn_compute_parameters->evalnoargs(new cons_t(pArgs,nil));
	if (pRet == nil)
		return NULL;
	auto pIK = make_unique<ik_lisp_t>(_fn_inverse_kinematics, 
		_fn_get_num_solutions, _fn_get_degrees_of_freedom, _fn_compute_parameters);
	pIK->_parameters = pRet;
	pIK->alloc_solutions();
	return pIK;
}

void ik_lisp_t::mark_in_use()
{
	_fn_compute_parameters->mark_in_use();
	_fn_inverse_kinematics->mark_in_use();
	_fn_get_degrees_of_freedom->mark_in_use();
	_fn_get_num_solutions->mark_in_use();
	if (_parameters != NULL)
		_parameters->mark_in_use();
}

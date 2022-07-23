#pragma once
#include "simula.h"

class light_page_t : public CPropertyPage
{
public:
	light_page_t(const char* caption, light_t& light, view_wnd_t& wnd);
	light_t _light;
	light_t& _ref_light;
	view_wnd_t& _view_wnd;

	enum { IDD = IDD_LIGHTPAGE };

// Overrides
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckEnabled();
	afx_msg void OnEditChange(UINT);
	DECLARE_MESSAGE_MAP()
};

class ambient_light_page_t : public CPropertyPage
{
public:
	ambient_light_page_t(const char* caption, ambient_light_t& light, view_wnd_t& wnd);
	ambient_light_t _light;
	ambient_light_t& _ref_light;
	view_wnd_t& _view_wnd;

	enum { IDD = IDD_AMBIENTPAGE };

	// Overrides
public:
	virtual BOOL OnApply();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckEnabled();
	afx_msg void OnEditChange(UINT);
	DECLARE_MESSAGE_MAP()
};

// LightPage.cpp : implementation file
//

#include "stdafx.h"
#include "robo.h"
#include "resource.h"
#include "LightPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void DDVector(CDataExchange* pDX, UINT id1, UINT id2, UINT id3, float* pf, float fmin, float fmax)
{
	DDX_Text(pDX, id1, pf[0]);
	DDV_MinMaxFloat(pDX, pf[0], fmin, fmax);
	DDX_Text(pDX, id2, pf[1]);
	DDV_MinMaxFloat(pDX, pf[1], fmin, fmax);
	DDX_Text(pDX, id3, pf[2]);
	DDV_MinMaxFloat(pDX, pf[2], fmin, fmax);
}

/////////////////////////////////////////////////////////////////////////////
// light_page_t property page

light_page_t::light_page_t(const char* caption, light_t& light, view_wnd_t& wnd)
 : CPropertyPage(light_page_t::IDD), _ref_light(light), _view_wnd(wnd)
{
	_light = _ref_light;
	m_strCaption = caption;
	m_psp.pszTitle = m_strCaption;
	m_psp.dwFlags |= PSP_USETITLE;
}

void light_page_t::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDVector(pDX, IDC_EDITRA, IDC_EDITGA, IDC_EDITBA, _light._ambient_properties, -1.f, 1.f);
	DDVector(pDX, IDC_EDITRD, IDC_EDITGD, IDC_EDITBD, _light._diffuse_properties, -1.f, 1.f);
	DDVector(pDX, IDC_EDITRS, IDC_EDITGS, IDC_EDITBS, _light._specular_properties, -1.f, 1.f);

	DDX_Text(pDX, IDC_EDITX, _light._position[0]);
	DDX_Text(pDX, IDC_EDITY, _light._position[1]);
	DDX_Text(pDX, IDC_EDITZ, _light._position[2]);
	DDX_Text(pDX, IDC_KCA, _light.constant_att);
	DDX_Text(pDX, IDC_KLA, _light.linear_att);
	DDX_Text(pDX, IDC_KQA, _light.quadratic_att);


	DDX_Check(pDX, IDC_CHECKENABLED, _light._is_enabled);
	DDX_Check(pDX, IDC_SPOTLIGHTON, _light._is_spot_enabled);
	DDX_Text(pDX, IDC_SPX, _light._spotlight_direction[0]);
	DDX_Text(pDX, IDC_SPY, _light._spotlight_direction[1]);
	DDX_Text(pDX, IDC_SPZ, _light._spotlight_direction[2]);
	DDX_Text(pDX, IDC_SEPO, _light._spotlight_exponent);
	DDX_Text(pDX, IDC_SCUTOF, _light._spotlight_cutoff);

}


BEGIN_MESSAGE_MAP(light_page_t, CPropertyPage)
	ON_CONTROL_RANGE(EN_CHANGE, 0, 0xffff, OnEditChange)
	ON_BN_CLICKED(IDC_CHECKENABLED, OnCheckEnabled)
	ON_BN_CLICKED(IDC_SPOTLIGHTON, OnCheckEnabled)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// light_page_t message handlers

BOOL light_page_t::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL light_page_t::OnApply() 
{
	BOOL bRes = CPropertyPage::OnApply();
	if (bRes)
	{
		_ref_light = _light;
		_view_wnd.Invalidate();
		_view_wnd.UpdateWindow();
		SetModified(FALSE);
	}
	return bRes;
}

void light_page_t::OnCheckEnabled()
{
	SetModified(TRUE);
}

void light_page_t::OnEditChange(UINT) 
{
	SetModified(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// ambient_light_page_t property page

ambient_light_page_t::ambient_light_page_t(const char* caption, ambient_light_t& light, view_wnd_t& wnd)
	: CPropertyPage(ambient_light_page_t::IDD), _ref_light(light), _view_wnd(wnd)
{
	_light = _ref_light;
	m_strCaption = caption;
	m_psp.pszTitle = m_strCaption;
	m_psp.dwFlags |= PSP_USETITLE;
}

void ambient_light_page_t::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECKENABLED, _light._is_enabled);
	DDVector(pDX, IDC_GLOAMBLR, IDC_GLOAMBLG, IDC_GLOAMBLB, _light.light, -1.f, 1.f);
}

BEGIN_MESSAGE_MAP(ambient_light_page_t, CPropertyPage)
	ON_CONTROL_RANGE(EN_CHANGE, 0, 0xffff, OnEditChange)
	ON_BN_CLICKED(IDC_CHECKENABLED, OnCheckEnabled)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ambient_light_page_t message handlers

BOOL ambient_light_page_t::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	return TRUE;
}

BOOL ambient_light_page_t::OnApply()
{
	BOOL bRes = CPropertyPage::OnApply();
	if (bRes)
	{
		_ref_light = _light;
		_view_wnd.Invalidate();
		_view_wnd.UpdateWindow();
		SetModified(FALSE);
	}
	return bRes;
}

void ambient_light_page_t::OnCheckEnabled()
{
	SetModified(TRUE);
}

void ambient_light_page_t::OnEditChange(UINT)
{
	SetModified(TRUE);
}

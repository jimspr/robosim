#include "stdafx.h"
#include "robo.h"
#include "mainwin.h"
#include "lispwin.h"
#include "lispenv.h"
#include "argstack.h"
#include "package.h"
#include "ik.h"
#include "grob.h"
#include "lisp_engine.h"

using namespace std;

lisp_engine_t lisp_engine;

simob_t::node_list_type simob_t::list("simob_t");

extern void init_forms(void);
extern void init_funcs(void);
extern void init_lfuncs(void);
extern void init_constants(void);
extern void init_variables(void);

robo_app_t theApp;
// Keep spLispWnd alive until the end.
shared_ptr<lisp_window_t> spLispWnd;

BEGIN_MESSAGE_MAP(robo_app_t, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// robo_app_t construction

robo_app_t::~robo_app_t()
{
}

int robo_app_t::Run(void)
{
	lisp_engine._env.reploop();

	if (m_pMainWnd == nullptr)
		return 0;

	m_pMainWnd->PostMessage(WM_CLOSE);
	return CWinApp::Run();
}

bool robo_app_t::run_one(void)
{
	LONG lIdleCount = 0;
	MSG* pMsg = AfxGetCurrentMessage();
	while (!::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE) &&
		OnIdle(lIdleCount++))
	{
	}
	if (!PumpMessage())
		return false;
	return true;
}

BOOL robo_app_t::InitInstance(void)
{
	auto pMainWnd = new main_window_t();
	m_pMainWnd = pMainWnd;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	spLispWnd = pMainWnd->_lisp_window;

	init_forms();
	init_funcs();
	init_lfuncs();
	init_constants();
	init_variables();

	node_t* l = cons_t::make_list(lisp_engine._package.get_symbol("LOAD"), new string_node_t("startup.lsp"));
	try
	{
		l->eval();
	}
	catch(eval_exception_t& e)
	{
		e._line_number = lisp_engine._env._readtable._line_cnt;
		AfxMessageBox(get_rlerror_msg(e, "STARTUP.LSP").c_str(), MB_OK|MB_ICONSTOP);
	}
	return TRUE;
}

#include "stdafx.h"
#include "robo.h"
#include "mainwin.h"
#include "lispwin.h"
#include "lispenv.h"
#include "argstack.h"
#include "package.h"
#include "ik.h"
#include "grob.h"

using namespace std;

// TODO - Make these globals be members of appropriate classes.
lisp_env_t lisp_env{ cin, cout };
node_list_array_t NodeListArray;
binding_stack_t g_bind_stack;

/* g_frame_stack is just a centralized place to put arguments to functions so that when the GC
   runs, it can mark these nodes as in use. */
frame_stack_t g_frame_stack;

package_t* current_package;

std::vector<std::unique_ptr<ik_interface>> invkin;

extern void init_forms(void);
extern void init_funcs(void);
extern void init_lfuncs(void);
extern void init_constants(void);
extern void init_variables(void);

node_list_t<symbol_t, 1024> symbol_t::plist("symbol_t LIST");
node_list_t<cons_t, 1024> cons_t::plist("cons_t LIST");
node_list_t<number_node_t, 1024> number_node_t::plist("number_node_t LIST");
node_list_t<vector_t, 128> vector_t::plist("vector_t LIST");
node_list_t<string_node_t, 512> string_node_t::plist("string_node_t LIST");
node_list_t<stream_node_t, 8> stream_node_t::plist("stream_node_t LIST");
node_list_t<bound_symbol_t, 1024> bound_symbol_t::plist("bound_symbol_t LIST");
node_list_t<special_form_t, 32> special_form_t::plist("special_form_t LIST");
node_list_t<macro_t, 32> macro_t::plist("macro_t LIST");
node_list_t<usrfunction_t, 32> usrfunction_t::plist("usrfunction_t LIST");
node_list_t<sysfunction_t, 256> sysfunction_t::plist("sysfunction_t LIST");
node_list_t<mat44, 128> mat44::plist("mat44 LIST");
node_list_t<simob_t, 16> simob_t::plist("simob_t LIST");

CRoboApp theApp;
shared_ptr<lisp_window_t> spLispWnd;

BEGIN_MESSAGE_MAP(CRoboApp, CWinApp)
	//{{AFX_MSG_MAP(CRoboApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	// Standard file based document commands
	// Standard print setup command
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoboApp construction

CRoboApp::CRoboApp()
{
	hSpinLib = LoadLibrary("SPIN.DLL");
}

CRoboApp::~CRoboApp()
{
	delete current_package;
	FreeLibrary(hSpinLib);
}

int CRoboApp::Run(void)
{
	try
	{
		lisp_env.reploop();
	}
	catch(done_exception_t* e)
	{
		e->Delete();
		ExitInstance();
		return 1;
	}

	if (m_pMainWnd == nullptr)
		return 0;

	m_pMainWnd->PostMessage(WM_CLOSE);
	return CWinApp::Run();
}

int CRoboApp::RunOne(void)
{
	LONG lIdleCount = 0;
	MSG* pMsg = AfxGetCurrentMessage();
	while (!::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE) &&
		OnIdle(lIdleCount++))
	{
	}
	if (!PumpMessage())
	{
		spLispWnd->stop();
	}
	return 1;
}

BOOL CRoboApp::InitInstance(void)
{
	auto pMainWnd = new main_window_t();
	m_pMainWnd = pMainWnd;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	current_package = new package_t("COMMON-LISP","CL");

	spLispWnd = pMainWnd->_lisp_window;

	init_forms();
	init_funcs();
	init_lfuncs();
	init_constants();
	init_variables();

	node_t *l = new cons_t(
			current_package->get_symbol("LOAD"),
			new cons_t(
				new string_node_t("startup.lsp"),
				nil
				)
			);
	try
	{
		l->eval();
	}
	catch(eval_exception_t* e)
	{
		e->_line_number = lisp_env._readtable._line_cnt;
		AfxMessageBox(get_rlerror_msg("STARTUP.LSP", *e).c_str(), MB_OK|MB_ICONSTOP);
		e->Delete();
	}
	return TRUE;
}

#pragma once
#ifndef __MAIN_H
#define __MAIN_H

class robo_app_t : public CWinApp
{
public:
	~robo_app_t();
	int Run(void) override;
	BOOL InitInstance(void) override;
	bool run_one(void);

	DECLARE_MESSAGE_MAP()
};

#endif

#pragma once

struct lisp_host_t
{
	virtual void periodic_notify() = 0;
	virtual void mark_in_use() = 0;
	virtual void add_busy() = 0;
	virtual void release_busy() = 0;
	virtual void exit() = 0;

	int _notify_cnt = 0;
	int _periodic_cnt = 100;
	// Called by cons_t::eval
	void notify_eval()
	{
		// Periodically, let host do some processing.
		if ((_notify_cnt++ % _periodic_cnt) == 0)
		{
			periodic_notify();
		}
	}
};

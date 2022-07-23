(defun joint-control-3r (robot)
	(setq jc-robot robot)
	(ask-dialog
		#'(lambda (j1 j2 j3)
			(set-agent-joints jc-robot (list j1 j2 j3))
		)
		"Joint Control"
		'("1" 0.0) '("2" 112.0) '("3" 117.0)
	)
	(setq jc-robot nil)
)
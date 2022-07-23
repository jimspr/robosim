(defun make-android nil (make-parallel-agent
	(make-fixed-link
		(make-box 100 100 5) (translate 0 0 -5)
	)
	(translate 0 0 0)
	(make-puma560) (translate 10 0 0)
	(make-puma560) (translate -10 0 0)
))
(defvar cnt 0)

(defun tak (x y z)
	(setq cnt (1+ cnt))
	(if (>= y x)
		z
		(tak (tak (1- x) y z)
			(tak (1- y) z x)
			(tak (1- z) x y)
		)
	)
)

(time (print (tak 13 12 6)))
(time (print (tak 14 12 6)))
(time (print (tak 15 12 6)))

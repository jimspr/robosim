(defun t1 (x)
	(t2)
)

(defun t2 nil
	(print x)
)

(setq x 2)
(t1 3)
(print "Should have printed 2")
		
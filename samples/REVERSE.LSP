(defun lr (list)
	(do ( (x list (cdr x))
		  (y '() (cons (car x) y))
		)
		((endp x) y)
	)
)

(defun lr* (list)
	(do* ( (x list (cdr x))
		  (y '() (cons (car x) y))
		)
		((endp x) y)
	)
)

(setq a '(a b c d e))
(print "LR")
(print (lr a))
(print "LR*")
(print (lr* a))

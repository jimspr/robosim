(defun verify (x y msg)
	(if (/= x y)
		(progn
			(princ "failed - verify ")
			(prin1 msg)
			(terpri)
			nil
		)
		t
	)
)

(defun verifyn (x y msg)
	(if (= x y)
		(progn
			(princ "failed - verifyn ")
			(prin1 msg)
			(terpri)
			nil
		)
		t
	)
)

(princ "Starting Tests")
(terpri)
; Make sure verify/verifyn work.
(verify 0 1 "intentional failure - make sure verify works")
(verifyn 1 1 "intentional failure - make sure verifyn works")

; Actual Tests
(verifyn nil t "nil t")
(verify t t "t t")
(verify nil nil "nil nil")
(verify (not nil) t "(not nil) t")
(verify (not t) nil "(not t) nil")
(verify (+ 1 2 3 4 5 6) 21 "(+ 1 2 3 4 5 6) 21")

(verify (* (asin 1) 2) pi "(* (asin 1) 2) pi")
(verify (asin 1) (/ pi 2) "(asin 1) (/ pi 2)")
(verify (asin 0) 0 "((asin 0) 0")

(verify (car nil) nil "(car nil) nil")
(verify (cdr nil) nil "(cdr nil) nil")

(defun fact (x)
	(if (= x 0)
		1
		(* x (fact (- x 1)))
	)

)

(defun sum (x)
	(if (= x 0)
		0
		(+ x (sum (- x 1)))
	)
)

(print "(fact 10)=")
(print (fact 10))
(print "(sum 10)=")
(print (sum 10))

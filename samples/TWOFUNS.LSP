(let ((x 3))
	(setq funs (list
		(function (lambda () (print x)))
		(function (lambda (y) (setq x y)))
		)
	)
)

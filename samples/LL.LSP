(defun list-length (x)
	(do (( n 0 (+ n 2))
		(fast x (cddr fast))
		(slow x (cdr slow)))
		(nil)
		(when (endp fast) (return n))
		(when (endp (cdr fast)) (return (+ n 1)))
		(when (and (eq fast slow) (> n 0)) (return nil))
	)
)

(print "Length of '(a b c d) is 4")
(print (list-length '(a b c d)))


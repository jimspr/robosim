;;allows easy creation of a robot template

(defun template nil
	(prog (names str question)
		(setq names (ask-dialog "Robot" '("Robot Name" :symbol) '("File Name" :string)))
		(if
			(null names)
			(return nil)
		)
		(setq str (open (cadr names)))
		(setq question '(ask-dialog "Robot Link Parms" '("theta" :float) '("dz" :float) '("da" :float) '("alpha" :float) '("Rev(0)-Pris(1)-Fixed(2)" :int)))
		(print
			(list 'defun (car names) nil
				(do	(
						(linkparms (eval question) (eval question))
						(links '(make-serial-agent))
					)
					((null linkparms) links) ; when user hits esc -> done
					(rplacd (last links)
						(cons
							(list 
								(cond
									( (= (nth 4 linkparms) 0) 'make-revolute-link)
									( (= (nth 4 linkparms) 1) 'make-prismatic-link)
									(t 'make-fixed-link)
								)
								'(make-box 10 10 10)
								nil
							)
							(list (list 'd-h (car linkparms) (cadr linkparms) (caddr linkparms) (cadddr linkparms)))
						)
					)
				)
			) 
			str
		)
		(close str)
		t
	)
)

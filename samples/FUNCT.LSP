(defun repl (lst ind val)

	(setq cnt (- (lng lst) 1))		;length of lst -1
	(setq ind (- ind 1))
	(setq a nil)				;initialize list 'a'
	(do
		((n cnt (- n 1)))		;construct list from last elem forward
		((< n 0)a)
		(if (/= n ind)	(setq a (cons (nth n lst) a))
				(setq a (cons val a))
		)

	)
)

(defun lng(x)					;length of a list
	(do (( n 0 (+ n 2))
		(fast x (cddr fast))
		(slow x (cdr slow)))
		(nil)
		(when (endp fast) (return n))
		(when (endp (cdr fast)) (return (+ n 1)))
		(when (and (eq fast slow) (> n 0)) (return nil))
	)
)

(defun c_red (ob) (set-object ob :color '(255 0 0)) )
(defun c_green (ob) (set-object ob :color '(0 255 0)) )
(defun c_blue (ob) (set-object ob :color '(0 0 255)) )
(defun c_yellow (ob) (set-object ob :color '(255 255 0)) )
(defun c_cyan (ob) (set-object ob :color '(0 255 255)) )
(defun c_muave (ob) (set-object ob :color '(255 0 255)) )
(defun c_white (ob) (set-object ob :color '(255 255 255)) )
(defun c_black (ob) (set-object ob :color '(0 0 0)) )
(defun c_gray (ob) (set-object ob :color '(128 128 128)) )





(defun create-box (x y z color)
	(let (name)
		(setq name (make-box x y z))
		(set-object name :color color)
		(use-objects name)
		name
	)
)
(setq mybox (create-box 10 20 30 '(255 0 0)))
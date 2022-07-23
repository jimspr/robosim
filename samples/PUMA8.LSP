; puma 8
(defun puma8 nil
	(set-object (setq puma-1 (make-puma560)) :color '(255 0 0))
	(set-object (setq puma-2 (make-puma560)) :color '(0 255 0))
	(set-object (setq puma-3 (make-puma560)) :color '(0 0 255))
	(set-object (setq puma-4 (make-puma560)) :color '(255 255 0))
	(set-object (setq puma-5 (make-puma560)) :color '(255 0 0))
	(set-object (setq puma-6 (make-puma560)) :color '(0 255 0))
	(set-object (setq puma-7 (make-puma560)) :color '(0 0 255))
	(set-object (setq puma-8 (make-puma560)) :color '(255 255 0))

	(set-position puma-1 (translate 75 25 0))
	(set-position puma-2 (translate 25 25 0))
	(set-position puma-3 (translate -25 25 0))
	(set-position puma-4 (translate -75 25 0))
	(set-position puma-5 (translate 75 -25 0))
	(set-position puma-6 (translate 25 -25 0))
	(set-position puma-7 (translate -25 -25 0))
	(set-position puma-8 (translate -75 -25 0))
	(set-look-from 0 150 100)
	(set-look-at 0 0 0)
	(set-camera 50 0 100)
	(remove-objects)
	(use-objects puma-1 puma-2 puma-3 puma-4 puma-5 puma-6 puma-7 puma-8)

	(do ( (ypos 8 (+ ypos .5)))
		((>= ypos 16) t)
		(unwind-protect
			(progn
				(set-update nil)
				(inc puma-1 0 ypos)
				(inc puma-2 1 ypos)
				(inc puma-3 2 ypos)
				(inc puma-4 3 ypos)
				(inc puma-5 4 ypos)
				(inc puma-6 5 ypos)
				(inc puma-7 6 ypos)
				(inc puma-8 7 ypos)
			)
			(set-update t)
		)
	)

	(setq puma-1 nil puma-2 nil puma-3 nil puma-4 nil puma-5 nil puma-6 nil puma-7 nil puma-8 nil)
)



(defun inc (ob idsol ypos)
	(set-agent-joints ob 
		(get-solution
			ob
			(matmult (get-position ob) (translate ypos ypos 0) (rotatex 180))
			idsol)
	)
)


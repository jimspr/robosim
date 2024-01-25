; puma 8
(defun puma8 nil
	(set-object (setq puma-1 (make-puma560)) :color '(255 0 0))
	(set-object (setq puma-2 (make-puma560)) :color '(0 255 0))
	(set-object (setq puma-3 (make-puma560)) :color '(0 0 255))
	(set-object (setq puma-4 (make-puma560)) :color '(255 255 0))
	(set-object (setq puma-5 (make-puma560)) :color '(255 0 255))
	(set-object (setq puma-6 (make-puma560)) :color '(0 255 255))
	(set-object (setq puma-7 (make-puma560)) :color '(128 128 128))
	(set-object (setq puma-8 (make-puma560)) :color '(255 255 255))

	(set-position puma-1 (translate 100 50 0))
	(set-position puma-2 (translate 33 50 0))
	(set-position puma-3 (translate -33 50 0))
	(set-position puma-4 (translate -100 50 0))
	(set-position puma-5 (translate 100 -25 0))
	(set-position puma-6 (translate 33 -25 0))
	(set-position puma-7 (translate -33 -25 0))
	(set-position puma-8 (translate -100 -25 0))
	(set-look-from 0 150 100)
	(set-look-at 0 0 0)
	(set-camera 50 0 100)
	(remove-objects)
	(use-objects puma-1 puma-2 puma-3 puma-4 puma-5 puma-6 puma-7 puma-8)

	(unwind-protect
		; Iterate through back and forth 10 times
		(do
			(
				(x 1 (+ x 1))
		    )
			; Move arm out
			( ( > x 10) t)
			(do ( (ypos 8 (+ ypos .5)))
				((>= ypos 16) t)
				(update-all ypos)
			)
			; Move arm in
			(do ( (ypos 16 (- ypos .5)))
				((<= ypos 8) t)
				(update-all ypos)
			)
		)
		; Clear locals
		(setq puma-1 nil puma-2 nil puma-3 nil puma-4 nil puma-5 nil puma-6 nil puma-7 nil puma-8 nil)
	)
)

(defun update-all (ypos)
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

(defun inc (ob idsol ypos)
	(set-agent-joints ob 
		(get-solution
			ob
			(matmult (get-position ob) (translate ypos ypos 0) (rotatex 180))
			idsol)
	)
)


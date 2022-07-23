;	*****
;	DEMO3D.LSP
;	*****

;	This demo shows a cylinder moving in various motions
;	to demonstrate 3-D effect.  This demo was prepared by
;	James F. Springfield, and modified by George E. Cook,
;	November 27,1993.  This demo is one of the SIMULATION
;	demos provided in the STARTUP.LSP file (Technology 2003
;	Conference).

;	*****

(defun demo3d nil
	(clear-simulation)
	(new-view3d 10)
	(maximize-active-window)
	(setq cyl (make-cylinder 10 100 30))
	(set-camera 50 0 150)
	(set-look-from 0 0 200)
	(set-look-at 0 0 50)
	(use-objects cyl)
	(do
		( (twist 0 (+ twist 90)) )
		( (= twist 450) t)
		(set-camera 50 twist 150)
		(do
			( (z 200 (- z 5)) )
			(( = z 100) t)
			(set-look-from 0 0 z)
		)
		(do
			( (x 5 (+ x 5)) )
			(( = x 40) t)
			(set-look-from x 0 105)
		)
		(do
			( (z 100 (- z 5)) )
			(( = z -10) t)
			(set-look-from 35 0 z)
		)
		(do
			( (x 30 (- x 5)) )
			(( = x -5) t)
			(set-look-from x 0 -5)
		)
		(do
			( (z -10 (- z 5)) )
			(( = z -100) t)
			(set-look-from 0 0 z)
		)
	)
)


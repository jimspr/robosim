;	*****
;	1-AXIS_T.LSP
;	*****

;	Modeling of 1-axis trap door. 
;	Used to simulate releasing of ball to demonstrate
;	ball bouncing on floor.  Other files needed for
;	complete demo include:  1-AXIS_R.LSP, BALLDEMO.LSP,
;	and STARTUP.LSP (Technology 2003 Version).

;	Prepared for NASA Technology 2003 Conference.

;	George E. Cook, November 24, 1993.

;	*****
;	Base Link
;	*****

(defun make-1-axis_trap nil (make-serial-agent
	(make-fixed-link
		(make-box 400 400 100) (matmult (rotatex 90)
				(translate 150 0 -150))
		(make-cylinder 50 400) (translate 0 0 0)
		(make-box 400 400 100) (matmult (rotatex 90)
				(translate -550 0 -150))
	)
	(matmult (rotatex 90) (translate 200 350 0))

;	*****
;	Link One
;	*****

	(make-revolute-link nil nil
		(make-box 300 400 100) (matmult (rotatex 90)
				(translate -200 0 -50))
	)
	(translate 0 0 0)

))

		
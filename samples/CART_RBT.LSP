;	*****
;	CARTESIAN_ROBOT.LSP
;	*****

;	Modeling of 3-axis cartesian robot.
;	Used with the files CARTDEMO.LSP and STARTUP.LSP
;	(Technology 2003 Version) to demonstrate robot
;	assemblying and positioning blocks to spell "NASA".

;	Prepared for NASA Technology 2003 Conference.
 
;	George E. Cook, November 4, 1993.

;	*****
;	Base Link
;	*****

(defun make-cartesian_robot nil (make-serial-agent
	(make-fixed-link
		(make-box 200 200 1000) (matmult (rotatey -90)
			(translate -150 650 50))
		(make-box 200 200 1000) (matmult (rotatey -90)
			(translate -150 -650 50))
		(make-box 200 200 1000) (matmult (rotatey -90)
			(translate 2650 650 50))
		(make-box 200 200 1000) (matmult (rotatey -90)
			(translate 2650 -650 50))
		(make-box 100 200 3000) (translate 0 650 -250)
		(make-box 100 200 3000) (translate 0 -650 -250)
	)
	(matmult (rotatey -90)
			(translate 1050 0 0))
;
;	*****
;	Link One
;	*****
;
	(make-prismatic-link nil nil
		(make-box 100 1100 200) (translate 0 0 -250)
	)
	(translate 0 0 0)
;
;	*****
;	Link Two
;	*****
;
	(make-prismatic-link nil nil
		(make-box 200 100 200) (translate -150 100 -100)
	)
	(matmult (rotatex -90) (rotatez -90))
;
;	*****
;	Link Three
;	*****
;
	(make-prismatic-link nil nil
		(make-box 100 100 1400) (translate 0 0 -1050)
	)
	(rotatex -90)
))

		
;	*****
;	3R_ROBOT.LSP
;	*****
;
;	Modeling of 3-axis robot similar to Problem 3.3 in 
;	"Introduction to Robotics" by John J. Craig.
;	A demonstration of this robot tracking a straight-line
;	is given in the file 3R-DEMO.LSP.  Other files used in
;	this demo are: TRACELIN.LSP and STARTUP.LSP (Technology
;	2003 Version).

;	George E. Cook, October 16, 1993.
;
;	*****
;	Base Link
;	*****
;
(defun make-3r_robot nil (make-serial-agent
	(make-fixed-link
		(make-cylinder 75 100) (translate 0 0 -200)
		(make-cylinder 100 25) (translate 0 0 -225)
	)
	(translate 0 0 225)
;
;	*****
;	Link One
;	*****
;
	(make-revolute-link nil nil
		(make-cylinder 75 200) (translate 0 0 -100)
		(make-cylinder 50 575) (matmult (rotatey 90)
			(translate 0 0 75))
		(make-cylinder 50 100) (matmult (rotatex 90)
			(translate 700 0 -50))
	)
	(translate 0 0 0)
;
;	*****
;	Link Two
;	*****
;
	(make-revolute-link nil nil
		(make-cylinder 35 600) (matmult (rotatey 90)
			(translate 0 0 50))
		(make-cylinder 50 100) (translate 700 0 -50)
	)
	(matmult (rotatex 90) (translate 700 0 0))
;
;	*****
;	Link Three
;	*****
;
	(make-revolute-link nil nil
		(make-cylinder 20 450) (matmult (rotatey 90)
			(translate 0 0 50))
		(make-cylinder 10 200) (matmult (rotatey 90)
			(translate 0 0 500))
	)
	(translate 700 0 0)
))
		
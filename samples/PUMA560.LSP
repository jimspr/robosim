;; 
;; Auxilary kinematics information for the PUMA 560 robot
;;

;
; LINK PARAMETERS
;   This command can be used to overwrite the ROBOSIM-generated
;   link parameters in the model after loading. It is necessary
;   because ROBOSIM does not follow the Denavit-Hartenberg
;   convention. We don't use it here, since the built-in 
;   PUMA inverse kinematics routine has been modified to
;   accept ROBOSIM's conventions.
; 		JOINT	THETA 	DZ	DA	ALPHA
; linkparam	1	0.0	0.0	0.0	-90.0
; linkparam	2	0.0	149.5	432.0	0.0
; linkparam	3	0.0	0.0	20.3	90.0
; linkparam	4	0.0	432.0	0.0	-90.0
; linkparam	5	0.0	0.0	0.0	90.0
; linkparam	6	0.0	56.5	0.0	0.0

;
; JOINT LIMITS
;		JOINT	MIN	MAX
;jointlimit	1	-160.0	160.0
;jointlimit	2	-225.0	45.0
;jointlimit	3	-45.0	225.0
;jointlimit	4	-110.0	170.0
;jointlimit	5	-100.0	100.0
;jointlimit	6	-266.0	266.0

;
; ANALYTICAL INVERSE KINEMATICS SOLUTION
;
;invertfunc	puma560_inv

(defun make-puma560 nil (make-serial-agent :classic
	(make-fixed-link
;		(make-cylinder 1.5 6) (translate 0 -6 -22)
;		(make-box 5 4 4) (translate 0 -6 -26)
		(make-cylinder 8 1.3) (translate 0 0 -26)
		(make-cylinder 7 1) (translate 0 0 -24.7)
		(make-cylinder 3.5 21) (translate 0 0 -23.7)
	)
	(translate 0 0 26)
	(make-revolute-link nil nil
		(make-cylinder 2.7 9.16) (translate 0 0 -3.88)
		(make-cylinder 3.5 1) (translate 0 0 6.39)
		(make-cylinder 3 1) (translate 0 0 5.39)
	)
	(matmult (rotatex -90))

	(make-revolute-link nil nil
		(make-extrude-surface 
			(make-primitive (list
				#(0 -9 0)
				#(1.25 -8.93 0)
				#(3.3 -8.53 0)
				#(5 -7.9 0)
				#(5 2 0)
				#(3 17 0)
				#(2.9 17.78 0)
				#(2.6 18.5 0)
				#(2.12 19.12 0)
				#(1.5 19.6 0)
				#(.78 19.9 0)
				#(0 20 0)
				#(-.78 19.9 0)
				#(-1.5 19.6 0)
				#(-2.12 19.12 0)
				#(-2.6 18.5 0)
				#(-2.9 17.78 0)
				#(-3 17 0)
				#(-5 2 0)
				#(-5 -7.9 0)
				#(-3.3 -8.53 0)
				#(-1.25 -8.93 0)
				#(0 -9 0)
			))
			4
		)
;		(matmult  (translate -17.57 0 1.75) (rotatez -90))
		(matmult  (translate -13.57 0 1.75) (rotatez -90))
	)
	(matmult (translate 17 0 5.87))

	(make-revolute-link nil nil
		(make-extrude-surface 
			(make-primitive (list
				#(0 -5 0)
				#(.87 -4.92 0)
				#(1.29 -4.83 0)
				#(1.71 -4.7 0)
				#(2.25 -4.47 0)
				#(.57 13.8 0)
;				#(-1.18 13.8 0)
				#(-2.93 13.8 0)
				#(-2.25 -4.47 0)
				#(-1.71 -4.7 0)
				#(-1.29 -4.83 0)
				#(-.87 -4.92 0)
				#(0 -5 0)
			))
			3.5
		)
		(matmult (rotatex 90) (translate 1.18 0 -1.75))
	)
	(matmult (translate -.8 0 0) (rotatex 90))

	(make-revolute-link nil nil
		(make-cylinder 1.6 1.2) (matmult (translate 0 0 -1.75))
		(make-cylinder 1.6 1.2) (matmult (translate 0 0 .55))
		(make-box 3.5 3.5 1.75) (matmult (rotatex 90) (translate 0 0 -3.35))
	)
	(matmult (translate 0 0 17.05) (rotatex -90))

	(make-revolute-link nil nil
		(make-cylinder .5 .4) (translate 0 0 1.6)
		(make-cylinder 1.6 1) (matmult (rotatex 90) (translate 0 0 -.5))
	)
	(rotatex 90)

	(make-revolute-link nil nil
		(make-cylinder 1 .2) (translate 0 0 -.215)
	)
	(translate 0 0 2.215)
	(make-fixed-link
		(make-cylinder .2 1.5) (translate 0 0 -1.5)
	)
	(translate 0 0 1.5)
;	(make-fixed-link
;		(make-box .5 .5 2.76) (translate -.7 0 1.75)
;		(make-box .5 .5 2.76) (translate .7 .75 1.75)
;		(make-box .5 .5 2.76) (translate .7 -.75 1.75)
;		(make-cylinder 1.5 1.75) (translate 0 0 .005)
;	)
;	nil
))


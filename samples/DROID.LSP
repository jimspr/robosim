;       Called from DROID.LSP
;
;	Creators:
;       	Lynn Conley
;       	Janis Rose
;
;       ********
;       Android parts
;       ********
;

(setq fa 8)				;number of faces on the arm-cylinder

(defun make-trunk nil (make-serial-agent
;	Positioner
	(make-fixed-link)
	(translate 0 0 0)
	
;	x-position
	(make-prismatic-link nil nil)
	(matmult (rotatey 90) (translate 0 0 0))

;	y-position
	(make-prismatic-link nil nil)
	(matmult (rotatex -90) (translate 0 0 0))

;	z-position
	(make-prismatic-link nil nil)
	(matmult (rotatey -90) (rotatez -90) )

;	orientation joint
	(make-revolute-link nil nil)
	(translate 0 0 0)

;       ********
;       Torso
;       ********

        (make-revolute-link nil nil
 
		(make-extrude-surface
			(make-primitive
				'(	#(0 -8 0)
					#(45 -10 0)
					#(45 10 0)
					#(0 8 0)
					#(-4 6.93 0)
					#(-6.93 4 0)
					#(-8 0 0)
					#(-6.93 -4 0)
					#(-4 -6.93 0)
					#(0 -8 0)
				)
			)
			40
		)
		(translate 0 0 -20)
                (make-cylinder 7 4 12) 
			(matmult (rotatey 90) (translate 0 0 45))
        )
        (matmult (rotatey 90) (rotatez 180) (translate 100 0 0))
;
;       ****
;       Head
;       ****
        (make-revolute-link nil nil
                (make-cylinder 7 4 12) (translate 0 0 0)
		(make-dome 10 12) (matmult (rotatex 180) (translate 0 0 -11.15))
		(make-cylinder 10 4 12) (translate 0 0 11.15)
		(make-dome 10 12) (translate 0 0 15.15)
        )
	(matmult (rotatey 90) (rotatez 180) (translate 0 0 49))
;        (translate 0 0 49)
    )
)

(defun make-larm nil (make-serial-agent 
;	Positioner
	(make-fixed-link)
	(translate 0 0 0)
	
;	x-position
	(make-prismatic-link nil nil)
	(matmult (rotatey 90) (translate 0 0 0))

;	y-position
	(make-prismatic-link nil nil)
	(matmult (rotatex -90) (translate 0 0 0))

;	z-position
	(make-prismatic-link nil nil)
	(matmult (rotatey -90) (rotatez -90) )

;	revolute to align with torso
	(make-revolute-link nil nil)
	(translate 0 0 0)


;	waist bend
	(make-revolute-link nil nil)
	(matmult (rotatey 90) (rotatez 180) (translate 100 0 0))

;       *********        
;       Upper arm
;       *********
;	
;	shoulder contains two rev. joints

	(make-revolute-link nil nil)
	(matmult (rotatex 180) (rotatez -90) (translate 0 39 -25))

        (make-revolute-link nil nil
		(make-sphere 5 8) (translate 0 0 0)
                (make-truncated-cone 5 4 18 fa) (matmult (rotatey 90)
			(translate 0 0 5))
	)
        (matmult (rotatez -90) (rotatex -90))

;
;	*********
;	Lower arm
;	*********

	(make-revolute-link nil nil
		(make-cylinder 4 3 12) (translate 0 0 -1.5)
		(make-truncated-cone 4 3 18 fa) (matmult (rotatey 90)
			(translate 0 0 4))
	)
	(matmult (rotatex 90) (translate 27 0 0))

;
;	****************************
;	roll, pitch and yaw of wrist
;	****************************
	
	(make-revolute-link nil nil) (matmult (rotatey 90) (translate 0 0 26))
	(make-revolute-link nil nil) (matmult (rotatey 90) (rotatez 90))
	(make-revolute-link nil nil
		(make-sphere 1 12) (translate 0 0 0)
		(make-sphere 2 12) (translate 4 0 0)
	)	(matmult (rotatey 90) (rotatez 90))
	
    )
)

(defun make-rarm nil (make-serial-agent
;	Positioner
	(make-fixed-link)
	(translate 0 0 0)
	
;	x-position
	(make-prismatic-link nil nil)
	(matmult (rotatey 90) (translate 0 0 0))

;	y-position
	(make-prismatic-link nil nil)
	(matmult (rotatex -90) (translate 0 0 0))

;	z-position
	(make-prismatic-link nil nil)
	(matmult (rotatey -90) (rotatez -90) )

;	revolute to align with torso
	(make-revolute-link nil nil)
	(translate 0 0 0)


;	waist bend
	(make-revolute-link nil nil)
	(matmult (rotatey 90) (rotatez 180) (translate 100 0 0))

;	*********        
;       Upper arm
;       *********
;	
;	shoulder conatins two rev. joints

 	(make-revolute-link nil nil)
	(matmult (rotatex 180) (rotatez -90) (translate 0 39 25))	

	(make-revolute-link nil nil
		(make-sphere 5 8) (translate 0 0 0)
                (make-truncated-cone 5 4 18 fa) 
			(matmult (rotatey 90) (translate 0 0 5))
	)
        (matmult (rotatez -90) (rotatex 90))

;	*********
;	Lower arm
;	*********
;
	(make-revolute-link nil nil
		(make-cylinder 4 3 12) (translate 0 0 -1.5)
		(make-truncated-cone 4 3 18 fa) (matmult (rotatey 90)
			(translate 0 0 4))
	)
	(matmult (rotatex -90) (translate 27 0 0))
;
;	****************************
;	roll, pitch and yaw of wrist
;	****************************
	
	(make-revolute-link nil nil) (matmult (rotatey 90) (translate 0 0 26))
	(make-revolute-link nil nil) (matmult (rotatey 90) (rotatez 90))
	(make-revolute-link nil nil
		(make-sphere 1 12) (translate 0 0 0)
		(make-sphere 2 12) (translate 4 0 0)
	)	(matmult (rotatey 90) (rotatez 90))
	
    )
)
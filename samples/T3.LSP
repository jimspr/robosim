;(defun make-twot3s nil
;	(make-parallel-agent
;		(make-prismatic-link
;			(make-box 138 38 20) (rotatey -90)
;		)
;		(rotatey 90)
;		(make-t3) (matmult (rotatey -90) (translate -50 0 20))
;		(make-t3) (matmult (rotatey -90) (translate 50 0 20))
;	)
;)

(defun make-t3 nil
	(make-serial-agent
		(make-fixed-link
			(make-truncated-cone 16 10 36) (translate 0 0 3.5)
			(make-cylinder 19 3.5) nil
		)
		nil

		(make-revolute-link nil nil
			(make-cylinder 11 4) (translate 0 0 -20)
			(make-box 10 2 20) (translate 0 -4 -16)
			(make-box 10 2 20) (translate 0 4 -16)
			(make-cylinder 8 14) (matmult (rotatex 90) (translate 0 0 -19))
		)
		(translate 0 0 59.42)
            
		(make-revolute-link nil nil
			(make-box 40 6 6) (translate 14 3 -3)
			(make-cylinder 3 6) (translate 40 0 -3)
			(make-box 6 6 6) (matmult (translate 40 0 0) (rotatez -30) (translate -3 0 -3))
		)
		(rotatex 90)

		(make-revolute-link nil nil
			(make-cylinder 3 6) (translate 0 0 -3)
			(make-box 10 6 6) (translate 35 0 -3)
			(make-box 30 6 6) (matmult (translate 30 0 0) (rotatez -11.54) (translate -15 0 -3))
		)
		(translate 40 0 0)

		(make-revolute-link nil nil
			(make-cylinder 2.5 3.8) (translate 0 0 -1.9)
			(make-cylinder 2.5 4) (matmult (translate 8 0 0) (rotatex -90) (translate 0 0 -2))
			(make-box 3 3 3) (translate 4 0 -1.5)
		)
		(translate 40 0 0)

		(make-revolute-link nil nil
			(make-cylinder 3.5 5) (matmult (rotatez -90) (rotatey 90) (translate 0 0 7))
			(make-box 7 7 1) (matmult (rotatez -90) (rotatey 90) (translate 0 0 6))
			(make-box 1 7 8.5) (matmult (rotatez -90) (rotatey 90) (translate -2.5 0 -2.5))
			(make-box 1 7 8.5) (matmult (rotatez -90) (rotatey 90) (translate 2.5 0 -2.5))
			(make-cylinder 2.5 3) (translate 0 0 3)
		)
		(matmult (translate 8 0 0) (rotatex -90))

		(make-revolute-link nil nil
			(make-cylinder 4 3) (translate 0 0 14)
			(make-cylinder 3 2) (translate 0 0 12)
		)
		(rotatex 90)

		(make-fixed-link
			(make-cylinder .5 6) nil
		)
		(translate 0 0 17.25)

;		(make-fixed-link)
;		(matmult (translate 0 0 7) (rotatez 90))
	);make-serial-agent
)	;defun



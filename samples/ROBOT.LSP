(defun make-ssrobot nil
	(make-serial-agent
		(make-fixed-link
			(make-box .5 160 .5) (matmult (rotatey 90) (translate 7.5 80 -2.5))
			(make-box .5 160 .5) (matmult (rotatey -90) (translate -2.5 80 -2.5))
		)
		(translate 0 -100 0)
		(make-prismatic-link nil nil
			(make-box 6 8 2) (matmult (rotatex -90) (translate 0 0 -1))
			(make-box 1 1 4) (matmult (rotatex -90) (translate 0 0 1))
		)
		(matmult (translate -1 80 -5) (rotatey -90) (rotatex 90))
		(make-revolute-link nil nil
			(make-cylinder .5 1) (matmult (rotatey 90) (translate 0 0 -.5))
			(make-box 1 1 4) (matmult (translate 0 -2.5 0) (rotatex 90) (translate 0 0 -2))
		)
		(matmult (rotatex -90) (translate 0 0 5.5))
		(make-revolute-link nil nil
			(make-cylinder .5 1) (matmult (rotatey 90) (translate 0 0 -.5))
			(make-box 1 1 10) (matmult (translate 0 5.5 0) (rotatex 90) (translate 0 0 -5))
		)
		(matmult (translate 0 -5 0) (rotatex 90))
		(make-revolute-link nil nil
			(make-cylinder .5 1) (translate 0 0 -.5)
			(make-box 1 1 8) (matmult (rotatex -90) (translate 0 0 .5))
		)
		(matmult (translate 0 11 0) (rotatez 90) (rotatex -90))
		(make-revolute-link nil nil
			(make-cylinder .5 1) (translate 0 0 -.5)
			(make-box 1 1 3) (matmult (rotatex -90) (translate 0 0 .5))
		)
		(translate 0 9 0)
		(make-revolute-link nil nil
			(make-box 1 2 .2) nil
			(make-box 1 .2 .8) (translate 0 -.5 .2)
			(make-box 1 .2 .8) (translate 0 .5 .2)
			(make-cylinder 1 4) (matmult (translate 0 0 1.5) (rotatex 90) (translate 0 0 -2))
		)
		(matmult (rotatex -90) (translate 0 0 3.5))
		(make-fixed-link
		)
		(translate 0 0 .01)
	)
)


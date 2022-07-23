(defun fly-by nil
	(clear-simulation)
	(new-view)
	(maximize-active-window)
	(set-update t)
;	(set-num-facets 10)
	(use-objects (make-station))
	(do
		(
			(y -200 (+ y 1))
		)
		((= y 200) t)
		(set-look-from 100 y 100)
	)
)
		
(defun make-robot nil
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

(defun make-livinmod nil (make-serial-agent
	(set-object (make-fixed-link
		(make-cylinder 2 1) (matmult (translate 0 -8.5 53.5))
		(make-cylinder 2 1) (matmult (translate 0 -8.5 30))
		(make-cylinder 2 1) (matmult (translate 0 8.5 61.5))
		(make-cylinder 2 1) (matmult (translate 0 8.5 30))
		(make-cylinder 2 1) (matmult (translate 0 -15 -22) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 15 -22) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 -22) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 -8.5 -16))
		(make-cylinder 2 1) (matmult (translate 0 -8.5 15))
		(make-cylinder 2 1) (matmult (translate 0 8.5 -16))
		(make-cylinder 2 1) (matmult (translate 0 8.5 15))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 -8.5) (rotatez 180) (translate 14 0 0) (rotatey 90) (translate 0 0 -7))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 -8.5) (rotatez 180) (translate 14 0 0) (rotatey 90) (rotatez 270) (translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 -8.5) (rotatez 180) (translate 14 0 0) (rotatey 90) (rotatez 180)	(translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 -8.5) (rotatez 180) (translate 14 0 0) (rotatey 90) (rotatez 90) (translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 -8.5) (rotatez 180) (translate 14 0 0) (rotatey 90) (translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 14 0 -8.5) (rotatey 90) (translate 0 0 -7))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 14 0 -8.5) (rotatey 90) (rotatez 270) (translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 14 0 -8.5) (rotatey 90) (rotatez 180) (translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 14 0 -8.5) (rotatey 90) (rotatez 90) (translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 14 0 -8.5) (rotatey 90) (translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 14 0 -8.5) (rotatey 90) (translate 0 0 6))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 22) (rotatez 270) (translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 22) (rotatez 180) (translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 22) (rotatez 90) (translate 0 6.5 0) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 6.5 22) (rotatex 90) (translate 0 0 -.5))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 28))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 -16))
		(make-cylinder 2 1) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 15))
	) :color '(255 255 0))
	nil
	(set-object (make-fixed-link
		(make-cylinder 6 6) (matmult (translate 0 -8.5 44.5) (rotatez 180) (rotatey 90) (translate 0 0 7.5))
		(make-cylinder 2 5) (matmult (translate 0 -8.5 44.5) (rotatez 180) (rotatey 90) (translate 0 0 7))
		(make-cylinder 6 6) (matmult (translate 0 -8.5 44.5) (rotatey 90) (translate 0 0 7.5))
		(make-cylinder 2 5) (matmult (translate 0 -8.5 44.5) (rotatey 90) (translate 0 0 7))
		(make-cylinder 7 2) (matmult (translate 0 -8.5 31))
		(make-cylinder 7 30) (matmult (translate 0 8.5 31))
		(make-cylinder 6 14) (matmult (translate 0 -7.5 -22) (rotatex 90) (translate 0 0 -7))
		(make-cylinder 6 14) (matmult (translate 0 7.5 -22) (rotatex 90) (translate 0 0 -7))
		(make-cylinder 7 30) (matmult (translate 0 -8.5 -15))
		(make-cylinder 7 30) (matmult (translate 0 8.5 -15))
		(make-cylinder 5 10) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 -8.5) (rotatez 180) (translate 14 0 0) (rotatey 90) (translate 0 0 7))
		(make-cylinder 5 10) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 -8.5) (rotatez 180) (translate 14 0 0) (rotatey 90) (translate 0 0 6))
		(make-cylinder 5 10) (matmult (translate 0 0 23) (rotatex 90) (translate 14 0 -8.5) (rotatey 90) (translate 0 0 7))
		(make-cylinder 5 10) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 29))
		(make-cylinder 7 30) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 -15))
	) :color '(0 255 0))
	nil
	(set-object (make-fixed-link
		(make-sphere 6) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 -8.5) (rotatez 180) (translate 14 0 0) (rotatey 90))
		(make-sphere 6) (matmult (translate 0 0 23) (rotatex 90) (translate 14 0 -8.5) (rotatey 90))
		(make-sphere 6) (matmult (translate 0 0 23) (rotatex 90) (translate 0 0 22))
	) :color '(0 255 255))
	nil
))

(defun make-belt nil
	(make-fixed-link
		(make-box 6 160 .1) (translate 5 80 -10)
		(make-box 4 6 4) (translate 5 53 -9.95)
		(make-box 5 4 6) (translate 5 63 -9.95)
		(make-box 5 8 5) (translate 5 97 -9.95)
		(make-box 4 3 3) (translate 5 105 -9.95)
	)
)

(defun make-cells nil
	(make-fixed-link
		(make-cylinder 2 5 10) (rotatey 90)
		(make-cylinder 1 5 10) (rotatey 90)
		(make-cylinder .2 15 10) (matmult (rotatey 90) (translate 0 0 5))
		(make-box .6 12.2 .4) (matmult (rotatey 90) (translate 0 0 20))
		(make-cylinder .1 24 10) (matmult (rotatey 90) (translate 0 0 20.4))
		(make-box .2 6 24) (matmult (rotatey 90) (translate 0 3 20.4))
		(make-box .2 6 24) (matmult (rotatey 90) (translate 0 -3 20.4))
	)
)

(defun make-subcells nil
	(make-fixed-link
		(make-cylinder .05 20) (translate 0 0 -20.1)
		(make-box 5 20 .1) (matmult (rotatex -90) (translate 2.55 10.1 -.05))
		(make-box 5 20 .1) (matmult (rotatex -90) (translate 0 .1 0) (rotatey 180) (translate 2.55 10 -.05))
		(make-box 10.2 .1 8) (matmult (rotatex -90) (translate 0 .05 -4))
	)
)

(defun make-box-array (n dim)
	(do 
		(
			(i 1 (1+ i))
			(arr (make-box dim dim dim))
			(box (make-box dim dim dim))
		)
		((= i n) (set-position arr (translate (/ dim 2) (/ dim 2) (- dim) )))
		(set-position box (translate 0 (* i dim) 0))
		(append-objects arr box)
	)
)

(defun make-station nil
	(make-parallel-agent
;make the base of the space station
		(make-fixed-link
			(make-box 4 3 6) (translate 12 48.5 -10)
			(make-box 4 3 8) (translate 12 43.5 -10)
			(make-belt) nil
			(make-box-array 16 10) (translate 5 5 -10)
		)
		(translate 0 -100 0)

;living module attaches to base of station
		(make-livinmod)
		(matmult (translate 0 70 -17) (rotatez 180) (rotatey -90))

; make robot
		(set-object (make-robot) :color '(0 0 255))
		(translate 0 100 0)

;small solar panel attaches to base
		(set-object (make-revolute-link nil nil
			(make-cells) (rotatey 90)
		) :color '(255 0 0))
		(matmult (translate 2.5 15 -7.9) (rotatez 90) (rotatex -90))

;another small solar panel attaches to base
		(set-object (make-revolute-link nil nil
			(make-cells) (rotatey 90)
		) :color '(255 0 0))
		(matmult (translate 2.5 135 -7.9) (rotatez 90) (rotatex -90))

		(make-main2)
		nil
		(make-main3)
		nil
	)
)

(defun make-main2 nil (make-serial-agent
	(make-revolute-link nil nil
;		MAINCELLS.OUT
;			TRANSLATE X=0.,Y=15.,Z=0.
;			ROTATE X=90.
;			TRANSLATE X=0.,Y=0.,Z=4.
;
		(make-box-array 8 10) (matmult (translate 0 0 4) (rotatex 90) (translate 0 0 -5))
		(make-box 8 8 8) (matmult (translate 0 0 4) (rotatex 90) (translate 0 5 -5))
		(make-cylinder 2 4 10) nil
	)
	(matmult (translate 5 0 -5) (rotatex 90))
	(set-object (make-fixed-link
		(make-subcells) (matmult (translate 0 0 4) (rotatex 90) (translate 0 15 -5) (rotatez -90))
		(make-subcells) (matmult (translate 0 0 4) (rotatex 90) (translate 0 55 -5) (rotatez -90))
	) :color '(255 0 0))
	nil
	(make-revolute-link nil nil
;		MAINCELLS.OUT
;			ROTATE Y=90.
	)
	(matmult (translate 0 0 59) (rotatez 90) (rotatex -90))
))

(defun make-main3 nil (make-serial-agent
	(make-revolute-link nil nil
;		MAINCELLS.OUT
;			TRANSLATE X=0.,Y=15.,Z=0.
;			ROTATE X=90.
;			TRANSLATE X=0.,Y=0.,Z=4.
;
		(make-box-array 8 10) (matmult (translate 0 0 4) (rotatex 90) (translate 0 0 -5))
		(make-box 8 8 8) (matmult (translate 0 0 4) (rotatex 90) (translate 0 5 -5))
		(make-cylinder 2 4 10) nil
	)
	(matmult (translate 5 160 -5) (rotatex -90))
	(set-object (make-fixed-link
		(make-subcells) (matmult (translate 0 0 4) (rotatex 90) (translate 0 15 -5) (rotatez -90))
		(make-subcells) (matmult (translate 0 0 4) (rotatex 90) (translate 0 55 -5) (rotatez -90))
	) :color '(255 0 0))
	nil
	(make-revolute-link nil nil
;		MAINCELLS.OUT
;			ROTATE Y=90.
	)
	(matmult (translate 0 0 59) (rotatez 90) (rotatex -90))
))

(fly-by)

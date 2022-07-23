;	*****
;	WELD.LSP
;	*****

;	This demo is of two PUMA560 robots performing a weld
;	on a part contoured to the same shape as the Space 
;	Shuttle Main Engine noozle.  This demo was prepared by
;	James F. Springfield, and modified for use at the 
;	NASA Technology 2003 Conference by George E. Cook,
;	November 27, 1993.  This demo is one of the SIMULATION
;	demos provided in the STARTUP.LSP file (Technology 2003
;	Version).

;	*****

(defun make-nozzle nil
	(make-extrude-surface 
		(make-primitive (list

		(vector 0 -10 0)
		(vector (+ 5	5) -10 0)
		(vector (+ 4.525	5) -9 0)
		(vector (+ 4.1	5) -8 0)
		(vector (+ 3.725	5) -7 0)
		(vector (+ 3.4	5) -6 0)
		(vector (+ 3.125	5) -5 0)
		(vector (+ 2.9	5) -4 0)
		(vector (+ 2.725	5) -3 0)
		(vector (+ 2.6	5) -2 0)
		(vector (+ 2.525	5) -1 0)
		(vector (+ 2.5	5) 0 0)
		(vector (+ 2.525	5) 1 0)
		(vector (+ 2.6	5) 2 0)
		(vector (+ 2.725	5) 3 0)
		(vector (+ 2.9	5) 4 0)
		(vector (+ 3.125	5) 5 0)
		(vector (+ 3.4	5) 6 0)
		(vector (+ 3.725	5) 7 0)
		(vector (+ 4.1	5) 8 0)
		(vector (+ 4.525	5) 9 0)
		(vector (+ 5	5) 10 0)

		(vector (- -5	5) 10 0)
		(vector (- -4.525	5) 9 0)
		(vector (- -4.1	5) 8 0)
		(vector (- -3.725	5) 7 0)
		(vector (- -3.4	5) 6 0)
		(vector (- -3.125	5) 5 0)
		(vector (- -2.9	5) 4 0)
		(vector (- -2.725	5) 3 0)
		(vector (- -2.6	5) 2 0)
		(vector (- -2.525	5) 1 0)
		(vector (- -2.5	5) 0 0)
		(vector (- -2.525	5) -1 0)
		(vector (- -2.6	5) -2 0)
		(vector (- -2.725	5) -3 0)
		(vector (- -2.9	5) -4 0)
		(vector (- -3.125	5) -5 0)
		(vector (- -3.4	5) -6 0)
		(vector (- -3.725	5) -7 0)
		(vector (- -4.1	5) -8 0)
		(vector (- -4.525	5) -9 0)
		(vector (- -5	5) -10 0)

;		(vector (+ 5	5) -10 0)
		(vector 0 -10 0)
		))
		1
	)
)

(defun weld nil
	(set-look-from 100 100 45)
	(set-look-at 0 0 20)
	(set-camera 50 0 200)
;	(remove-objects)
	(let
		(
		(nozzle (make-nozzle))
		(puma-1 (make-puma560))
		(puma-2 (make-puma560))
		)

		(set-object nozzle :color '(0 0 255))
		(set-position puma-1 (matmult (translate -20 0 0) (rotatez 180)))
		(set-position puma-2 (translate 20 0 0))
		(set-object puma-1 :color '(255 255 0))
		(set-object puma-2 :color '(255 0 255))

		(use-objects puma-1 puma-2 nozzle)

		(set-agent-joints puma-2 (get-solution puma-2 (matmult (translate 0 0 1.1) (rotatex 180)) 2))
		(grasp puma-2 nozzle)
		(set-position puma-1 (matmult (translate -30 0 0) (rotatez 180)))
		(set-position puma-2 (translate 30 0 0))
		(set-agent-joints puma-2 (get-solution puma-2 (matmult (translate 0 0 15) (rotatey -90)) 2))

		(set-agent-joints puma-1 (get-solution puma-1
			(matmult (translate 0 -10 26) (rotatex 180))
			2
			)
		)

		(tp puma-1 (+ 5	5) -10 0)
		(tp puma-1 (+ 4.525	5) -9 0)
		(tp puma-1 (+ 4.1	5) -8 0)
		(tp puma-1 (+ 3.725	5) -7 0)
		(tp puma-1 (+ 3.4	5) -6 0)
		(tp puma-1 (+ 3.125	5) -5 0)
		(tp puma-1 (+ 2.9	5) -4 0)
		(tp puma-1 (+ 2.725	5) -3 0)
		(tp puma-1 (+ 2.6	5) -2 0)
		(tp puma-1 (+ 2.525	5) -1 0)
		(tp puma-1 (+ 2.5	5) 0 0)
		(tp puma-1 (+ 2.525	5) 1 0)
		(tp puma-1 (+ 2.6	5) 2 0)
		(tp puma-1 (+ 2.725	5) 3 0)
		(tp puma-1 (+ 2.9	5) 4 0)
		(tp puma-1 (+ 3.125	5) 5 0)
		(tp puma-1 (+ 3.4	5) 6 0)
		(tp puma-1 (+ 3.725	5) 7 0)
		(tp puma-1 (+ 4.1	5) 8 0)
		(tp puma-1 (+ 4.525	5) 9 0)
		(tp puma-1 (+ 5	5) 10 0)

		(setq puma-1 nil puma-2 nil nozzle nil)
	)
)

(defun tp (ob x y z)
	(move-straight-to ob
			(matmult (translate z y (+ x 16)) (rotatex 180))
			2
			5
	)
)
	
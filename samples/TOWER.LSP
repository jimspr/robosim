;	*****
;	TOWER.LSP
;	*****

;	This is a demo of a PUMA560 robot transferring a "tower"
;	of blocks from one location to the other.  This demo
;	was prepared by James F. Springfield, and modified for 
;	use at the NASA Technology 2003 Conference by George E.
;	Cook, November 27, 1993.  This demo is one of the 
;	SIMULATION demos provided in the STARTUP.LSP file
;	(Technology 2003 Version).

;	*****

(defun tower nil
	(set-look-from 0 70 40)
	(set-look-at 0 0 15)
	(set-camera 50 0 100)
	(setq puma-1 (set-object (make-puma560)))
	(setq b1 (set-object (make-box 10 10 1) :color '(255 0 0)))
	(setq b2 (set-object (make-box 8 8 1) :color '(0 255 0)))
	(setq b3 (set-object (make-box 6 6 1) :color '(0 0 255)))
	(set-position b1 (translate 20 15 0))
	(set-position b2 (translate 20 15 1))
	(set-position b3 (translate 20 15 2))
	(use-objects b1 b2 b3 puma-1)
	(move-inter-to puma-1 (matmult (translate 20 15 10) (rotatex 180)) 2 20)

	(down 1 3)

	(grasp puma-1 b3)
	(up 1 3)
	(across 1 3)
	(down 3 1)
	(release puma-1)
	(up 3 1)

	(across 3 1)

	(down 1 2)
	(grasp puma-1 b2)
	(up 1 2)
	(across 1 2)
	(down 2 1)
	(release puma-1)
	(up 2 1)

	(across 2 3)

	(down 3 1)
	(grasp puma-1 b3)
	(up 3 1)
	(across 3 2)
	(down 2 2)
	(release puma-1)
	(up 2 2)
	(across 2 1)
	(down 1 1)
	(grasp puma-1 b1)
	(up 1 1)
	(across 1 3)
	(down 3 1)
	(release puma-1)
	(up 3 1)
	(across 3 2)
	(down 2 2)
	(grasp puma-1 b3)
	(up 2 2)
	(across 2 1)
	(down 1 1)
	(release puma-1)
	(up 1 1)
	(across 1 2)
	(down 2 1)
	(grasp puma-1 b2)
	(up 2 1)
	(across 2 3)
	(down 3 2)
	(release puma-1)
	(up 3 2)
	(across 3 1)
	(down 1 1)
	(grasp puma-1 b3)
	(up 1 1)
	(across 1 3)
	(down 3 3)
	(release puma-1)
	(up 3 3)
	(setq puma-1 nil b1 nil b2 nil b3 nil)
)

(defun up (stack from)
	(move-straight-to
		puma-1
		(matmult (translate (* (- 2 stack) 20) 15 10) (rotatex 180))
		2
		5
	)
)

(defun down (stack to)
	(move-straight-to
		puma-1
		(matmult (translate (* (- 2 stack) 20) 15 to) (rotatex 180))
		2
		5
	)
)

(defun across (stackf stackt)
	(move-straight-to
		puma-1
		(matmult (translate (* (- 2 stackt) 20) 15 10) (rotatex 180))
		2
		(* 5 (abs (- stackf stackt)))
	)
)


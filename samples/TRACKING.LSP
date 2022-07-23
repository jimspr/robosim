;	*****
;	TRACKING.LSP
;	*****

;	PUMA560 tracking demo.  Prepared by James F. Springfield, and
;	modified by George E. Cook for NASA Technology 2003 Conference.

;	November 27, 1993

;	*****

(defun tracking nil
	(remove-objects)
;	(clear-simulation)
;	(new-view)
;	(maximize-active-window)
	(let
		(
		(puma-1 (set-object (make-puma560) :color '(255 255 0)))
		(b1 (set-object (make-box 26 26 1) :color '(0 255 255)))
		(c 13)
		)
;		(set-look-from 120 120 75)
;		(set-look-at 0 0 15)
;		(set-camera 50 0 200)
;		(set-eye-separation 15)
		(set-position puma-1 (translate 0 0 1))
		(use-objects b1 puma-1)
		(move-inter-to puma-1 (matmult (translate c c 1.1) (rotatex 180)) 3 20)
		(move-straight-to puma-1 (matmult (translate c (- c) 1.1) (rotatex 180)) 3 20)
		(move-straight-to puma-1 (matmult (translate (- c) (- c) 1.1) (rotatex 180)) 3 20)
		(move-straight-to puma-1 (matmult (translate (- c) c 1.1) (rotatex 180)) 3 20)
		(move-straight-to puma-1 (matmult (translate c c 1.1) (rotatex 180)) 3 20)
	)
)

;	*****
;	PUMTRACK.LSP
;	*****

;	*****
;	Puma560 tracking demo prepared for NASA Technology 2003.

;	George E. Cook, November 26, 1993
;	*****

;	*****
;	Set viewing parameters.
;	*****

(set-look-at 0 0 15)
(set-look-from 120 120 75)
(set-camera 50 0 200)
(set-eye-separation 15)

;	*****
;	Create Puma and rectangular base.
;	*****

(setq puma-1 (set-object (make-puma560) :color '(255 255 0)))
(setq b1 (set-object (make-box 26 26 1) :color '(0 255 255)))
(use-objects b1 puma-1)

;	*****
;	Trace edges of base with straight-line motion of Puma.
;	*****

(defvar c 13)
(move-inter-to puma-1 (matmult (translate c c 1.1)
				(rotatex 180)) 3 20)
(move-straight-to puma-1 (matmult (translate c (- 0 c) 1.1) 
				(rotatex 180)) 3 20)
(move-straight-to puma-1 (matmult (translate (- 0 c) (- 0 c) 1.1) 
				(rotatex 180)) 3 20)
(move-straight-to puma-1 (matmult (translate (- 0 c) c 1.1)
				(rotatex 180)) 3 20)
(move-straight-to puma-1 (matmult (translate c c 1.1)
				(rotatex 180)) 3 20)

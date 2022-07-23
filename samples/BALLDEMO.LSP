;	*****
;	BALLDEMO.LSP
;	*****

;	Simulation of ball released from overhead trap door
;	and bouncing on floor below.  Other files needed for
;	demo include: 1-AXIS_R.LSP, 1-AXIS_T.LSP, and STARTUP.LSP
;	(Technology 2003 Version).

;	Prepared for NASA Technology 2003 Conference.

;	George E. Cook, November 27, 1993.

;	*****

;	*****
;	Set viewing parmeters.
;	*****

(set-look-at 0 0 0)
(set-look-from 0 1500 0)

;	*****
;	Create floor and "pad" that ball bounces on.
;	*****

(setq floor (make-box 1000 1350 10))
(set-object floor :color '(0 255 0))
(set-position floor (translate 0 0 -385))
(setq base (make-cylinder 200 100))
(set-position base (translate 0 0 -575))
(set-object base :color '(255 255 0))

;	*****
;	Load ball model (r) and trap door model (br).
;	*****

(load "1-axis_r.lsp")
(setq r (make-1-axis_robot))
(set-object r :color '(255 0 255))
(drive-agent r '(500) 1)
(load "1-axis_t.lsp")
(setq br (make-1-axis_trap))
(set-object br :color '(0 255 255))

;	*****
;	View objects in simulation.
;	*****

(use-objects r base br floor)

;	*****
;	Open trap door (br) and simulate bouncing ball (r).
;	*****

(drive-agent br '(90) 1)

(do	(
	( time 0 (+ time 1))
	)
	((= time 50) t)   

  	(defun d-sin (tim)
		(- (abs (* 375 (* (exp (* -0.1 tim)) (sin (* .628 tim))))) 375)
	)

	(drive-agent r (list (d-sin time)) 4)

)






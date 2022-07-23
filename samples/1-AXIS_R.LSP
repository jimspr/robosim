;	*****
;	1-AXIS_R.LSP
;	*****

;	Modeling of 1-axis cartesian robot. 
;	Used to simulate motion of ball bouncing on floor.
;	Base link is invisible, link one is ball.
;	Other files needed for complete demo include:
;	1-AXIS_T.LSP, BALLDEMO.LSP, and STARTUP.LSP 
;	(Technology 2003 Version).

;	Prepared for NASA Technology 2003 Conference.

;	George E. Cook, November 24, 1993.

;	*****
;	Base Link
;	*****

(defun make-1-axis_robot nil (make-serial-agent
	(make-fixed-link)
	(translate 0 0 0)

;	*****
;	Link One
;	*****

	(make-prismatic-link nil nil
		(make-sphere 100 10) (translate 0 0 0)
	)
	(translate 0 0 0)

))

		
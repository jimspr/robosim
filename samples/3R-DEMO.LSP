;	*****
;	3R-DEMO.LSP
;	*****
;	Demo of 3-axis robot tracking a st-line between x=-1500,
;	y=900, z=800 and x=1500, y=700, z=500.  Straight-line path
;	is divided into 10 segments with a 10X10X10 box located at
;	each of the 11 nodes (including start and stop nodes).
;	Other files needed for demo include: 3R-ROBOT.LSP, 
;	TRACELIN.LSP, and STARTUP.LSP (Technology 2003 Version).
;	The file J-CNT-3R.LSP is also loaded.  This file creates
;	a dialog box for changing the 3 joint values.  To call 
;	the joint-control-3r dialog box, enter the command,
;	(joint-control-3r 3r).

;	George E. Cook, October 16, 1993.
;	*****

(remove-objects)
(set-look-at 0 0 500)
(set-look-from 0 3500 500)
(set-camera 50 0 100)

;	*****
;	Load 3-axis robot, 3-R_ROBOT.
;	*****

(load "3r_robot.lsp")
(setq 3r (make-3r_robot))
(set-object 3r :color '(255 255 0))
(use-objects 3r)

;	*****
;	Build floor.
;	*****

(setq floor (make-box 2500 2500 10))
(set-position floor (translate 0 0 -10))
(use-objects floor)

;	*****
;	Creation of composite of 11 boxes (nodes) evenly spaced 
;	along straight-line segment.
;	*****

(setq nodes (make-composite
	(make-box 10 10 10) (translate -1500 900 795)
	(make-box 10 10 10) (translate -1200 880 765)
	(make-box 10 10 10) (translate -900 860 735)
	(make-box 10 10 10) (translate -600 840 705)
	(make-box 10 10 10) (translate -300 820 675)
	(make-box 10 10 10) (translate 0 800 645)
	(make-box 10 10 10) (translate 300 780 615)
	(make-box 10 10 10) (translate 600 760 585)
	(make-box 10 10 10) (translate 900 740 555)
	(make-box 10 10 10) (translate 1200 720 525)
	(make-box 10 10 10) (translate 1500 700 495)
	)
)
(set-object nodes :color '(0 255 255))
(use-objects nodes)
(setq st-line (make-primitive (list
		#(-1500 900 800)
		#(-1500 900 800.01)
		#(1500 700 500.01)
		#(1500 700 500)
		#(-1500 900 800)
		))
)
(set-object st-line :color '(0 255 0))
(use-objects st-line)

;	*****
;	Load file to move robot to each node along straight-
;	line with joint angles determined from inverse kinematics.
;	*****

(load "tracelin.lsp")
(load "j-cnt-3r.lsp")
;(joint-control-3r 3r)

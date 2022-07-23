;
;
;       ******************
;       Droid demo program
;       ******************
;
;	joints:
;		left arm  (x y z direction waist_bend arm_forward arm_up 
;		  elbow_up)
;	 	trunk 	  (x y z direction waist_bend neck_bend)
;		right arm (x y z direction waist_bend arm_forward arm_up
;		  elbow_up)
;	since no get-joint-angles exist we'll assign
;	 unique variables to store current joint values

(maximize-active-window)
(set-camera 50 0 100)
;
(set-look-at 40 0 135)
(set-look-from 40 -400 165)
(load "funct.lsp")			;functions
(load "droid.lsp")			;android parts

;	design workspace
;(setq floor (make-cylinder 450 250 36))
;(set-position floor (translate 0 0 0))
;(c_blue floor)

(setq tab1 (make-box 100 200 10))
(set-position tab1 (translate 0 0 80))
(setq leg1 (make-cylinder 12 80 12))
;(set-position leg1 (translate -30 -80 0))	;to max speed
;(setq leg2 (make-cylinder 8 80 10))		; use 1 support
;(set-position leg2 (translate -30 80 0))
;(setq leg3 (make-cylinder 8 80 10))
;(set-position leg3 (translate 30 -80 0))
;(setq leg4 (make-cylinder 8 80 10))
;(set-position leg4 (translate 30 80 0))
(setq tab1 (add-objects tab1 leg1))
(setq tab2 (add-objects tab1))
(set-position tab1 (translate 300 50 0))
;(set-position tab2 (translate -300 50 0))
(c_cyan tab1)
;(c_muave tab2)

(remove-objects)
(use-objects tab1)

;	design objects
(setq ob1 (make-extrude-surface			;torso
		(make-primitive
			'(	#(0 -8 0)
				#(45 -10 0)
				#(45 10 0)
				#(0 8 0)
				#(-4 6.93 0)
				#(-6.93 4 0)
				#(-8 0 0)
				#(-6.93 -4 0)
				#(-4 -6.93 0)
				#(0 -8 0)
			)
		)
		40
	))
(setq ob2 (make-cylinder 7 4 12))
(set-position ob2 (matmult (rotatey 90) (translate -20 0 45)))
(setq torso_3 (add-objects ob1 ob2))
;(setq torso_4 (add-objects torso_3))
(c_gray torso_3)
;(c_gray torso_4)
(set-position torso_3 (translate 260 0 90))
;(set-position torso_4 (matmult (rotatez 180) (translate 260 -100 90)))
(use-objects torso_3)

(setq ob1 (make-cylinder 7 4 12))		;neck and head
(setq ob2 (make-dome 10 12))
(set-position ob2 (matmult (rotatex 180) (translate 0 0 -11.15)))
(setq ob3 (make-cylinder 10 4 12))
(set-position ob3 (translate 0 0 11.15))
(setq ob4 (make-dome 10 12))
(set-position ob4 (translate 0 0 15.15))
(setq head_3 (add-objects ob1 ob2 ob3 ob4))
;(setq head_4 (add-objects ob1 ob2 ob3 ob4))
(c_gray head_3)
;(c_gray head_4)
(set-position head_3 (translate 325 0 90))
;(set-position head_4 (translate -325 100 90))
(use-objects head_3)

(setq ruj_3 (make-sphere 5 8))			;upper arm joints
(set-position ruj_3 (translate 260 -20 95))
(c_gray ruj_3)
(setq luj_3 (make-sphere 5 8))
(set-position luj_3 (translate 280 -20 95))
(c_gray luj_3)
;(setq ruj_4 (make-sphere 5 8))
;(set-position ruj_4 (translate -260 120 95))
;(c_gray ruj_4)
;(setq luj_4 (make-sphere 5 8))
;(set-position luj_4 (translate -280 120 95))
;(c_gray luj_4)
(use-objects ruj_3 luj_3)

(setq rua_3 (make-truncated-cone 5 4 18 8))	;upper arms
(set-position rua_3 (translate 300 -20 90))
(c_gray rua_3)
(setq lua_3 (make-truncated-cone 5 4 18 8))
(set-position lua_3 (translate 320 -20 90))
(c_gray lua_3)
;(setq rua_4 (make-truncated-cone 5 4 18 8))
;(set-position rua_4 (translate -300 120 90))
;(c_gray rua_4)
;(setq lua_4 (make-truncated-cone 5 4 18 8))
;(set-position lua_4 (translate -320 120 90))
;(c_gray lua_4)
(use-objects rua_3 lua_3)

(setq rlj_3 (make-cylinder 4 3 12))		;lower arm joints
(set-position rlj_3 (translate 270 -30 90))
(c_gray rlj_3)
(setq llj_3 (make-cylinder 4 3 12))
(set-position llj_3 (translate 290 -30 90))
(c_gray llj_3)
;(setq rlj_4 (make-cylinder 4 3 12))
;(set-position rlj_4 (translate -270 130 90))
;(c_gray rlj_4)
;(setq llj_4 (make-cylinder 4 3 12))
;(set-position llj_4 (translate -290 130 90))
;(c_gray llj_4)
(use-objects rlj_3 llj_3)

(setq rla_3 (make-truncated-cone 4 3 20 8))	;lower arms
(set-position rla_3 (translate 310 -30 90))
(c_gray rla_3)
(setq lla_3 (make-truncated-cone 4 3 20 8))
(set-position lla_3 (translate 330 -30 90))
(c_gray lla_3)
;(setq rla_4 (make-truncated-cone 4 3 20 8))
;(set-position rla_4 (translate -310 130 90))
;(c_gray rla_4)
;(setq lla_4 (make-truncated-cone 4 3 20 8))
;(set-position lla_4 (translate -330 130 90))
;(c_gray lla_4)
(use-objects rla_3 lla_3)


;	first robot
(setq dt_1 (make-trunk))		
(c_red dt_1)
(setq dl_1 (make-larm))
(c_green dl_1)
(setq dr_1 (make-rarm))
(c_green dr_1)

(setq eye (make-sphere 2 6))		;draw eyes
(setq eye1 (make-sphere 2 6))
(set-position eye (matmult (rotatex 90) (translate -4 167.5 8)))
(set-position eye1 (matmult (rotatex 90) (translate +4 167.5 8)))
(setq eye (add-objects eye eye1))

(setq eye_1 (add-objects eye))		;give robot 'vision'
(c_cyan eye_1)

(setq la_1 '(0 0 0 0 0 0 0 0 0 0 0))	; inital joint angles
(setq ra_1 '(0 0 0 0 0 0 0 0 0 0 0))
(setq tr_1 '(0 0 0 0 0 0))

;offset first robot
(set-agent-joints dl_1 (setq la_1 (repl la_1 1 40)))
(set-agent-joints dt_1 (setq tr_1 (repl tr_1 1 40)))
(set-agent-joints dr_1 (setq ra_1 (repl ra_1 1 40)))
(set-position eye_1 (translate 40 0 0))

(use-objects dl_1 dt_1 dr_1 eye_1)
(grasp dt_1 eye_1)

;	second robot
;(setq dt_2 (make-trunk))		
;(c_blue dt_2)
;(setq dl_2 (make-larm))
;(c_green dl_2)
;(setq dr_2 (make-rarm))
;(c_green dr_2)

;(setq eye_2 (add-objects eye))		;give robot 'vision'
;(c_yellow eye_2)

;(setq la_2 '(0 0 0 0 0 0 0 0 0 0 0))	; inital joint angles
;(setq ra_2 '(0 0 0 0 0 0 0 0 0 0 0))
;(setq tr_2 '(0 0 0 0 0 0))

;offset second robot
;(set-agent-joints dl_2 (setq la_2 (repl la_2 1 -40)))
;(set-agent-joints dt_2 (setq tr_2 (repl tr_2 1 -40)))
;(set-agent-joints dr_2 (setq ra_2 (repl ra_2 1 -40)))
;(set-position eye_2 (translate -40 0 0))

;(use-objects dl_2 dt_2 dr_2 eye_2)
;(grasp dt_2 eye_2)

; program demo

(set-double-buffer t)			;only show update of object
(do					;rotate to work position
	((x 10 (+ x 10)))
	((> x 106)t)
	(set-update nil)
	(set-agent-joints dl_1 (setq la_1 (repl la_1 4 x)))
	(set-agent-joints dt_1 (setq tr_1 (repl tr_1 4 x)))
	(set-agent-joints dr_1 (setq ra_1 (repl ra_1 4 x)))
;	(set-agent-joints dl_2 (setq la_2 (repl la_2 4 (* x -1))))
;	(set-agent-joints dt_2 (setq tr_2 (repl tr_2 4 (* x -1))))
;	(set-agent-joints dr_2 (setq ra_2 (repl ra_2 4 (* x -1))))
	(set-update t)
)

(set-update nil)
(set-agent-joints dl_1 (setq la_1 (repl la_1 8 8)))
(set-agent-joints dr_1 (setq ra_1 (repl ra_1 8 8)))
;(set-agent-joints dl_2 (setq la_2 (repl la_2 8 8)))
;(set-agent-joints dr_2 (setq ra_2 (repl ra_2 8 8)))
(set-update t)

(setq xo_1 (nth 0 tr_1))		;get x,y origin
;(setq xo_2 (nth 0 tr_2))
	
(do					;walk to work space
	((n 1 (+ n 1)))
	((> n 20)t)
	(set-update nil)
	(set-agent-joints dl_1 (setq la_1 
		(repl (repl (repl la_1 6 (* 10 (cos (* n pi))))
			 2 (* n 2.5)) 1 (+ xo_1 (* n 9)))))
	(set-agent-joints dt_1 (setq tr_1 
		(repl (repl tr_1 2 (* n 2.5)) 1 (+ xo_1 (* n 9)))))
	(set-agent-joints dr_1 (setq ra_1 
		(repl (repl (repl ra_1 6 (* -10 (cos (* n pi))))
			 2 (* n 2.5)) 1 (+ xo_1 (* n 9)))))
;	(set-agent-joints dl_2 (setq la_2 
;		(repl (repl (repl la_2 6 (* 10 (cos (* n pi))))
;			 2 (* n 2.5)) 1 (+ xo_2 (* n -9)))))
;	(set-agent-joints dt_2 (setq tr_2 
;		(repl (repl tr_2 2 (* n 2.5)) 1 (+ xo_2 (* n -9)))))
;	(set-agent-joints dr_2 (setq ra_2 
;		(repl (repl (repl ra_2 6 (* -10 (cos (* n pi)))) 
;			 2 (* n 2.5)) 1 (+ xo_2 (* n -9)))))
	(set-update t)
)
(set-update nil)				;align to table
(set-agent-joints dl_1 (setq la_1 '(220 50 0 90 0 0 0 0 0 0 0)))
(set-agent-joints dt_1 (setq tr_1 '(220 50 0 90 0 0)))
(set-agent-joints dr_1 (setq ra_1 '(220 50 0 90 0 0 0 0 0 0 0)))
;(set-agent-joints dl_2 (setq la_2 '(-220 50 0 -90 0 0 0 0 0 0 0)))
;(set-agent-joints dt_2 (setq tr_2 '(-220 50 0 -90 0 0)))
;(set-agent-joints dr_2 (setq ra_2 '(-220 50 0 -90 0 0 0 0 0 0 0)))
(set-update t)

(drive-agent dr_1 (setq ra_1 '(220 50 0 90 0 13 36 53 -10 -24 0)) 5)
(grasp dr_1 torso_3)

(do
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-look-at (+ 40 (* n 26)) (* n 5) (- 135 (* n 3.5)))
)
(do
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-look-from (+ 40 (* 16 n)) (+ -400 (* 25 n)) 165)
)
;(set-look-at 300 50 100)
;(set-look-from 200 -150 165)

(do					;assemble torso
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dr_1 (list 220 50 0 90 0 (+ 13 (* n 2.4)) (- 36 (* n 3.6))
	  53 (+ n -10) (+ (* n 6) -24) (* n 7.2)))
	(set-update t)
)
(setq ra_1 '(220 50 0 90 0 37 0 53 0 36 72))

(do
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-look-from (+ 200 (* 20 n)) (+ -150 (* 5 n)) 165)
)
;(set-look-from 400 -100 165)

(do					;assemble torso
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 220 50 0 90 (* n 6) n 0 (* n 4.5) 0 0 0))
	(set-agent-joints dt_1 (list 220 50 0 90 (* n 6) 0))
	(set-agent-joints dr_1 (list 220 50 0 90 (* n 6) (+ (* n 7.8) 37)
	  (* n -1.5) (- 53 (* n 2.9)) 0 36 72))
	(set-update t)
)
(setq la_1 '(220 50 0 90 60 10 0 45 0 0 0))
(setq tr_1 '(220 50 0 90 60 0))
(setq ra_1 '(220 50 0 90 60 115 -15 24 0 36 72))

(release dr_1)
(set-position torso_3 (matmult (rotatex 90) (rotatey 90)
	(translate 50 100 280)))

(do					;grasp head
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (+ 220 (* n 4)) (- 50 (* n 5)) 0 90 
	  (- 60 (* n 3.4)) (+ (* n 6) 10) 0 (+ (* n 6) 45) 0 0 0))
	(set-agent-joints dt_1 (list (+ 220 (* n 4)) (- 50 (* n 5)) 0 90 
	  (- 60 (* n 3.4)) (* n -1.5)))
	(set-agent-joints dr_1 (list (+ 220 (* n 4)) (- 50 (* n 5)) 0 90
	  (- 60 (* n 3.4)) (- 115 (* n 5.4)) (+ (* n 3) -15) (+ (* n 1.4) 24)
	  0 (- 36 (* n 10.2)) (- 72 (* n 14.4))))
	(set-update t)
)
(grasp dr_1 head_3)
(setq la_1 '(240 25 0 90 43 40 0 75 0 0 0))
(setq tr_1 '(240 25 0 90 43 -15))
(setq ra_1 '(240 25 0 90 43 88 0 31 0 -15 0))

(do					;move head
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (- 240 (* n 2)) (+ 25 (* n 2.5)) 0 90 
	  (- 43 (* n 4.3)) (+ 40 (* 2 n)) (* n -3.5) (- 75 (* n .5)) 0 0 0))
	(set-agent-joints dt_1 (list (- 240 (* n 2)) (+ 25 (* n 2.5)) 0 90 
	  (- 43 (* n 4.3)) (+ (* n 1.5) -15)))
	(set-agent-joints dr_1 (list (- 240 (* n 2)) (+ 25 (* n 2.5)) 0 90
	  (- 43 (* n 4.3)) (- 88 (* n 2.8)) (* n -3.5) (+ (* n 3.9) 31)
	  (* n -10) (+ (* n 3) -15) (* n 3)))
	(set-update t)
)
(setq la_1 '(220 50 0 90 0 60 -35 70 0 0 0))
(setq tr_1 '(220 50 0 90 0 0))
(setq ra_1 '(220 50 0 90 0 60 -35 70 -100 15 15))

(release dr_1)
(grasp dl_1 head_3)

(do					;move head
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (+ 220 (* n 4)) (+ 50 (* n 5)) 0 90 
	 0 (- 60 (* n 2.8)) (+ (* 16 n) -35) (- 70 (* n 2)) (* n -16) 
	 (* n 5.4) 0))
	(set-agent-joints dt_1 (list (+ 220 (* n 4)) (+ 50 (* n 5)) 0 90 
	  0 (* n 1.5)))
	(set-agent-joints dr_1 (list (+ 220 (* n 4)) (+ 50 (* n 5)) 0 90
	  0 60 (+ (* n 7) -35) 70 -100 15 15))
	(set-update t)
)
(setq la_1 '(240 75 0 90 0 46 45 60 -80 27 0))
(setq tr_1 '(240 75 0 90 0 15))
(setq ra_1 '(240 75 0 90 0 60 0 70 -100 15 15))

(do					;move head
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 240 75 0 90 (* n 4) 
	 (+ 46 (* n 3.2)) (- 45 (* n 4.2)) (- 60 (* 6.4 n)) 
	 (+ (* n 3.2) -80) 29 (* n -.8)))
	(set-agent-joints dt_1 (list 240 75 0 90 (* n 4) 15))
	(set-agent-joints dr_1 (list 240 75 0 90 (* n 4)
	 60 0 70 -100 15 15))
	(set-update t)
)
(setq la_1 '(240 75 0 90 20 62 24 28 -64 29 -4))
(setq tr_1 '(240 75 0 90 20 15))
(setq ra_1 '(240 75 0 90 20 60 0 70 -100 15 15))

(release dl_1)

(set-position head_3 (matmult (rotatex -90) (translate 300 -100 99)))
(drive-agent dl_1 '(240 75 0 90 20 46 28 50 0 0 0) 3)

(do					;get arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (- 240 (* n 2)) (- 75 (* n 9.5)) 0 90 
	 (- 20 n) (- 46 (* n 1.3)) (- 28 (* n 2.8)) (- 50 (* n .8)) 0 0 0))
	(set-agent-joints dt_1 (list (- 240 (* n 2)) (- 75 (* n 9.5)) 0 90
	 (- 20 n) (- 15 (* n 1.5))))
	(set-agent-joints dr_1 (list (- 240 (* n 2)) (- 75 (* n 9.5)) 0 90 
	 (- 20 n) (- 60 (* n 1.5)) 0 70 (+ (* n 10) -100) (- 15 (* n 1.5)) 
	 (- 15 (* n 1.5))))
	(set-update t)
)
(setq la_1 '(220 -20 0 90 10 30 0 42 0 0 0))
(setq tr_1 '(220 -20 0 90 10 0))
(setq ra_1 '(220 -20 0 90 10 45 0 70 0 0 0))

(do					;move to upper arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 220 -20 0 (+ (* n 1.9) 90) 
	 (+ (* n 2) 10) 30 (* n -4) 42 0 0 0))
	(set-agent-joints dt_1 (list 220 -20 0 (+ (* n 1.9) 90) 
	 (+ (* n 2) 10) (* n -1.9)))
	(set-agent-joints dr_1 (list 220 -20 0 (+ (* n 1.9) 90) 
	 (+ (* n 2) 10) (+ (* n 5) 45) 0 (- 70 (* n 7)) 0 0 0))
	(set-update t)
)
(setq la_1 '(220 -20 0 109 30 30 -40 42 0 0 0))
(setq tr_1 '(220 -20 0 109 30 -19))
(setq ra_1 '(220 -20 0 109 30 95 0 0 0 0 0))

(grasp dl_1 ruj_3)
(grasp dr_1 rua_3)
(drive-agent dr_1 '(220 -20 0 109 30 95 0 10 86 5 3) 6)

(do					;assemble upper joint and arm
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 220 (+ (* n 4) -20) 0 (- 109 (* n 3.8)) 
	 (- 30 (* n 5)) (- 30 n) (+ (* n 8) -40) (+ (* n .4) 42) 0 0 0))
	(set-agent-joints dt_1 (list 220 (+ (* n 4) -20) 0 (- 109 (* n 3.8)) 
	 (- 30 (* n 5)) (+ (* n 1.9) -19)))
	(set-agent-joints dr_1 (list 220 (+ (* n 4) -20) 0 (- 109 (* n 3.8)) 
	 (- 30 (* n 5)) (- 95 (* n 11.2)) (* n -4.4) 10 86 5 3))
	(set-update t)
)
(setq la_1 '(220 0 0 90 5 25 0 44 0 0 0))
(setq tr_1 '(220 0 0 90 5 0))
(setq ra_1 '(220 0 0 90 5 39 -22 10 86 5 3))

(release dl_1)
(release dr_1)
(set-position ruj_3 (translate 260 20 95))
(set-position rua_3 (matmult (rotatex 90) (translate 260 95 -15)))

(do					;move to grasp lower joint
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 220 0 0 90 
	 (+ 5 (* n 3)) 25 0 (+ (* n 5.2) 44) 0 0 0))
	(set-agent-joints dt_1 (list 220 0 0 90 
	 (+ 5 (* n 3)) 0))
	(set-agent-joints dr_1 (list 220 0 0 90 (+ 5 (* n 3)) 
	 (+ 39 (* n 3.6)) (+ (* n 5.4) -22) (+ 10 (* n 1.2)) 
	 (- 86 (* n 17.2)) (+ (* n 11) 5)  (- 3 (* n .6))))
	(set-update t)
)
(setq la_1 '(220 0 0 90 20 25 0 70 0 0 0))
(setq tr_1 '(220 0 0 90 20 0))
(setq ra_1 '(220 0 0 90 20 57 5 16 0 60 0))

(grasp dr_1 rlj_3)
(drive-agent dr_1 '(220 0 0 90 20 57 5 30 0 20 0) 3)
(drive-agent dr_1 '(220 0 0 90 20 20 -24 46 0 -37 -4) 6)
(release dr_1)
(set-position rlj_3 (matmult (rotatey 90) (translate -95 -7 258.5)))
(drive-agent dr_1 '(220 0 0 90 20 20 -24 66 0 -37 -4) 3)

(do					;move to lower arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 220 0 0 90 
	 (+ 20 (* n 4)) (- 25 (* n 2.5)) 0 (+ 70 (* n 5)) 0 0 0))
	(set-agent-joints dt_1 (list 220 0 0 90 
	 (+ 20 (* n 4)) 0))
	(set-agent-joints dr_1 (list 220 0 0 90 
	 (+ 20 (* n 4)) (+ 20 (* n 12)) (+ (* n 2.9) -24) (- 66 (* n 6.6)) 
	 0 (+ (* n 3.7) -37) (+ (* n .4) -4)))
	(set-update t)
)
(setq la_1 '(220 0 0 90 60 0 0 120 0 0 0))
(setq tr_1 '(220 0 0 90 60 0))
(setq ra_1 '(220 0 0 90 60 140 5 0 0 0 0))

(grasp dr_1 rla_3)
(drive-agent dr_1 '(220 0 0 90 60 143 7 4 90 16 0) 5)

(do					;assemble lower arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 220 0 0 90 
	 (- 60 (* n 5.5)) (* n 2) 0 (- 120 (* n 9)) 0 0 0))
	(set-agent-joints dt_1 (list 220 0 0 90 
	 (- 60 (* n 5.5)) 0))
	(set-agent-joints dr_1 (list 220 0 0 90 (- 60 (* n 5.5))
	 (- 143 (* n 10)) 7 4 90 16 0))
	(set-update t)
)
(setq la_1 '(220 0 0 90 5 20 0 30 0 0 0))
(setq tr_1 '(220 0 0 90 5 0))
(setq ra_1 '(220 0 0 90 5 43 7 4 90 16 0))

(release dr_1)
(set-position rla_3 (matmult (rotatex 90) (translate 260 95 11)))
(setq r_3 (add-objects ruj_3 rua_3 rlj_3 rla_3))
(set-update nil)
  (c_gray r_3)
  (remove-objects ruj_3)		;for some reason it will
  (remove-objects rua_3)		; only remove one at a time
  (remove-objects rlj_3)
  (remove-objects rla_3)
  (use-objects r_3)
(set-update t)

(do					;get second arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (+ 220 (* n 2)) (* n -2) 0 (+ (* n 1.9) 90) 
	 (+ (* n 2.5) 5) (+ n 20) (* n -4)(+ 32 (* n 1.2)) 0 0 0))
	(set-agent-joints dt_1 (list (+ 220 (* n 2)) (* n -2) 0 (+ (* n 1.9) 90) 
	 (+ (* n 2.5) 5) (* n -1.9)))
	(set-agent-joints dr_1 (list (+ 220 (* n 2)) (* n -2) 0 (+ (* n 1.9) 90) 
	 (+ (* n 2.5) 5) (+ 43 (* n 5.2)) (- 7 (* n .7)) 0 0 0 0))
	(set-update t)
)
(setq la_1 '(240 -20 0 109 30 30 -40 42 0 0 0))
(setq tr_1 '(240 -20 0 109 30 -19))
(setq ra_1 '(240 -20 0 109 30 95 0 0 0 0 0))

(grasp dl_1 luj_3)
(grasp dr_1 lua_3)
(drive-agent dr_1 '(240 -20 0 109 30 95 0 10 86 5 3) 6)

(do					;assemble upper joint and arm
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 240 (+ (* n 4) -20) 0 (- 109 (* n 3.8)) 
	 (- 30 (* n 5)) (- 30 n) (+ (* n 8) -40) (+ (* n .4) 42) 0 0 0))
	(set-agent-joints dt_1 (list 240 (+ (* n 4) -20) 0 (- 109 (* n 3.8)) 
	 (- 30 (* n 5)) (+ (* n 1.9) -19)))
	(set-agent-joints dr_1 (list 240 (+ (* n 4) -20) 0 (- 109 (* n 3.8)) 
	 (- 30 (* n 5)) (- 95 (* n 11.2)) (* n -4.4) 10 86 5 3))
	(set-update t)
)
(setq la_1 '(240 0 0 90 5 25 0 44 0 0 0))
(setq tr_1 '(240 0 0 90 5 0))
(setq ra_1 '(240 0 0 90 5 39 -22 10 86 5 3))

(release dl_1)
(release dr_1)
(set-position luj_3 (translate 280 20 95))
(set-position lua_3 (matmult (rotatex 90) (translate 280 95 -15)))

(do					;move to grasp lower joint
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 240 0 0 90 
	 (+ 5 (* n 3)) 25 0 (+ (* n 5.2) 44) 0 0 0))
	(set-agent-joints dt_1 (list 240 0 0 90 
	 (+ 5 (* n 3)) 0))
	(set-agent-joints dr_1 (list 240 0 0 90 (+ 5 (* n 3)) 
	 (+ 39 (* n 3.6)) (+ (* n 5.4) -22) (+ 10 (* n 1.2)) 
	 (- 86 (* n 17.2)) (+ (* n 11) 5)  (- 3 (* n .6))))
	(set-update t)
)
(setq la_1 '(240 0 0 90 20 25 0 70 0 0 0))
(setq tr_1 '(240 0 0 90 20 0))
(setq ra_1 '(240 0 0 90 20 57 5 16 0 60 0))

(grasp dr_1 llj_3)
(drive-agent dr_1 '(240 0 0 90 20 57 5 30 0 20 0) 3)
(drive-agent dr_1 '(240 0 0 90 20 20 -24 46 0 -37 -4) 6)
(release dr_1)
(set-position llj_3 (matmult (rotatey 90) (translate -95 -7 278.5)))
(drive-agent dr_1 '(240 0 0 90 20 20 -24 66 0 -37 -4) 3)

(do					;move to lower arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 240 0 0 90 
	 (+ 20 (* n 4)) (- 25 (* n 2.5)) 0 (+ 70 (* n 5)) 0 0 0))
	(set-agent-joints dt_1 (list 240 0 0 90 
	 (+ 20 (* n 4)) 0))
	(set-agent-joints dr_1 (list 240 0 0 90 
	 (+ 20 (* n 4)) (+ 20 (* n 12)) (+ (* n 2.9) -24) (- 66 (* n 6.6)) 
	 0 (+ (* n 3.7) -37) (+ (* n .4) -4)))
	(set-update t)
)
(setq la_1 '(240 0 0 90 60 0 0 120 0 0 0))
(setq tr_1 '(240 0 0 90 60 0))
(setq ra_1 '(240 0 0 90 60 140 5 0 0 0 0))

(grasp dr_1 lla_3)
(drive-agent dr_1 '(240 0 0 90 60 143 7 4 90 16 0) 5)

(do					;assemble lower arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 240 0 0 90 
	 (- 60 (* n 5.5)) (* n 2) 0 (- 120 (* n 9)) 0 0 0))
	(set-agent-joints dt_1 (list 240 0 0 90 
	 (- 60 (* n 5.5)) 0))
	(set-agent-joints dr_1 (list 240 0 0 90 (- 60 (* n 5.5))
	 (- 143 (* n 10)) 7 4 90 16 0))
	(set-update t)
)
(setq la_1 '(240 0 0 90 5 20 0 30 0 0 0))
(setq tr_1 '(240 0 0 90 5 0))
(setq ra_1 '(240 0 0 90 5 43 7 4 90 16 0))

(release dr_1)
(set-position lla_3 (matmult (rotatex 90) (translate 280 95 11)))
(setq l_3 (add-objects luj_3 lua_3 llj_3 lla_3))
(set-update nil)
  (c_gray l_3)
  (remove-objects luj_3)
  (remove-objects lua_3)
  (remove-objects llj_3)
  (remove-objects lla_3)
  (use-objects l_3)
(set-update t)

(do					;move to grasp right arm
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (- 240 (* n 4)) 0 0 90 
	 5 (+ (* n 4) 20) 0 (- 30 (* n 4)) 0 0 0))
	(set-agent-joints dt_1 (list (- 240 (* n 4)) 0 0 90 
	 5 0))
	(set-agent-joints dr_1 (list (- 240 (* n 4)) 0 0 90 
	 5 (+ 43 (* n .4)) 8 (- 4 (* n .8)) 0 0 0))
	(set-update t)
)
(setq la_1 '(220 0 0 90 5 40 0 10 0 0 0))
(setq tr_1 '(220 0 0 90 5 0))
(setq ra_1 '(220 0 0 90 5 45 8 0 0 0 0))

(grasp dl_1 r_3)

(drive-agent dt_1 '(220 0 0 90 5 45) 4)

(do					;move to assemble right arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 220 (* n 6.9) 0 90 
	 5 40 0 10 0 0 0))
	(set-agent-joints dt_1 (list 220 (* n 6.9) 0 90 
	 5 (- 45 (* n 4.5))))
	(set-agent-joints dr_1 (list 220 (* n 6.9) 0 90 
	 5 45 8 0 0 0 0))
	(set-update t)
)
(setq la_1 '(220 69 0 90 5 40 0 10 0 0 0))
(setq tr_1 '(220 69 0 90 5 0))
(setq ra_1 '(220 69 0 90 5 45 8 0 0 0 0))

(do					;assemble right arm
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 220 69 0 90 
	 (+ 5 (* n 3)) (+ 40 (* n 2.6)) 0 (+ 10 (* n 4)) 0 0 0))
	(set-agent-joints dt_1 (list 220 69 0 90 
	 (+ 5 (* n 3)) 0))
	(set-agent-joints dr_1 (list 220 69 0 90 
	 (+ 5 (* n 3)) (+ 45 ( * n 2.4)) 8 (* n 4.4) 0 0 0))
	(set-update t)
)
(setq la_1 '(220 69 0 90 20 53 0 30 0 0 0))
(setq tr_1 '(220 69 0 90 20 0))
(setq ra_1 '(220 69 0 90 20 57 8 22 0 0 0))

(release dl_1)
(set-position r_3 (translate 15 69 5))

(do					
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 220 69 0 90 
	 (- 20 (* n 3)) (- 53 (* n 8.6)) 0 (+ 30 (* n 6)) 0 0 0))
	(set-agent-joints dt_1 (list 220 69 0 90 
	 (- 20 (* n 3)) (* n -9)))
	(set-agent-joints dr_1 (list 220 69 0 90 
	 (- 20 (* n 3)) (- 57 (* n 9.4)) (- 8 (* n 1.6)) 
	 (+ 22 (* n 7.6)) 0 0 0))
	(set-update t)
)
(setq la_1 '(220 69 0 90 5 10 0 60 0 0 0))
(setq tr_1 '(220 69 0 90 5 -45))
(setq ra_1 '(220 69 0 90 5 10 0 60 0 0 0))

(do					;move to align with left arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 220 (- 69 (* n 6.9))
	 0 90 5 (+ 10 (* n 3)) 0 (- 60 (* n 3)) 0 0 0))
	(set-agent-joints dt_1 (list 220 (- 69 (* n 6.9)) 
	 0 90 5 (+ (* n 4.5) -45)))
	(set-agent-joints dr_1 (list 220 (- 69 (* n 6.9))
	 0 90 5 (+ 10 (* n 2)) 0 (- 60 (* n 3)) 0 0 0))
	(set-update t)
)
(setq la_1 '(220 0 0 90 5 30 0 30 0 0 0))
(setq tr_1 '(220 0 0 90 5 0))
(setq ra_1 '(220 0 0 90 5 30 0 30 0 0 0))

(do					;move to grasp left arm
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (+ 220 (* n 4)) 0 0 90 
	 5 (+ (* n 2) 30) 0 (- 30 (* n 4)) 0 0 0))
	(set-agent-joints dt_1 (list (+ 220 (* n 4)) 0 0 90 
	 5 0))
	(set-agent-joints dr_1 (list (+ 220 (* n 4)) 0 0 90 
	 5 (+ 30 (* n 3)) (* n 1.6) (- 30 (* n 6)) 0 0 0))
	(set-update t)
)
(setq la_1 '(240 0 0 90 5 40 0 10 0 0 0))
(setq tr_1 '(240 0 0 90 5 0))
(setq ra_1 '(240 0 0 90 5 45 8 0 0 0 0))

(grasp dl_1 l_3)

(do					;move to walk
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (- 240 (* n 3)) 0 0 (- 90 (* n 9)) 
	 (- 5 (* n .5)) (- 40 (* n 3)) (* n .4) (+ 10 (* n 5))
	 (* n -.4) 0 0))
	(set-agent-joints dt_1 (list (- 240 (* n 3)) 0 0 (- 90 (* n 9)) 
	 (- 5 (* n .5)) 0))
	(set-agent-joints dr_1 (list (- 240 (* n 3)) 0 0 (- 90 (* n 9)) 
	 (- 5 (* n .5)) (- 45  (* n 3.5)) (- 8 (* n .4)) (* n 6)
	 0 0 0))
	(set-update t)
)
(setq la_1 '(210 0 0 0 0 10 4 60 -4 0 0))
(setq tr_1 '(210 0 0 0 0 0))
(setq ra_1 '(210 0 0 0 0 10 4 60 0 0 0))

(do
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-camera 50 0 (- 100 (* n 10)))
)
;(set-camera 50 0 50)

(do					;walk to corner
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 210 (* n -5) 0 0 0 10 4 60 -4 0 0))
	(set-agent-joints dt_1 (list 210 (* n -5) 0 0 0 0))
	(set-agent-joints dr_1 (list 210 (* n -5) 0 0 0 10 4 60 0 0 0))
	(set-update t)
)
(setq la_1 '(210 -50 0 0 0 10 4 60 -4 0 0))
(setq tr_1 '(210 -50 0 0 0 0))
(setq ra_1 '(210 -50 0 0 0 10 4 60 0 0 0))

(do					;walk around corner
	((n 189 (+ n 9)))
	((> n 270)t)
	(setq m (* n .01745))
	(set-update nil)
	(set-agent-joints dl_1 (list (+ 250 (* (cos m) 40)) (+ (* (sin m) 40) -50)
	 0 (- n 180) 0 10 4 60 -4 0 0))
	(set-agent-joints dt_1 (list (+ 250 (* (cos m) 40)) (+ (* (sin m) 40) -50)
	 0 (- n 180) 0 0))
	(set-agent-joints dr_1 (list (+ 250 (* (cos m) 40)) (+ (* (sin m) 40) -50)
	 0 (- n 180) 0 10 4 60 0 0 0))
	(set-update t)
)
(setq la_1 '(250 -90 0 90 0 10 4 60 -4 0 0))
(setq tr_1 '(250 -90 0 90 0 0))
(setq ra_1 '(250 -90 0 90 0 10 4 60 0 0 0))

(do					;walk to next corner
	((n 1 (+ n 1)))
	((> n 20)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (+ 250 (* n 5)) -90 0 90 0 10 4 60 -4 0 0))
	(set-agent-joints dt_1 (list (+ 250 (* n 5)) -90 0 90 0 0))
	(set-agent-joints dr_1 (list (+ 250 (* n 5)) -90 0 90 0 10 4 60 0 0 0))
	(set-update t)
)
(setq la_1 '(350 -90 0 90 0 10 4 60 -4 0 0))
(setq tr_1 '(350 -90 0 90 0 0))
(setq ra_1 '(350 -90 0 90 0 10 4 60 0 0 0))

(do					;walk around corner
	((n 279 (+ n 9)))
	((> n 360)t)
	(setq m (* n .01745))
	(set-update nil)
	(set-agent-joints dl_1 (list (+ 350 (* (cos m) 40)) (+ (* (sin m) 40) -50)
	 0 (- n 180) 0 10 4 60 -4 0 0))
	(set-agent-joints dt_1 (list (+ 350 (* (cos m) 40)) (+ (* (sin m) 40) -50)
	 0 (- n 180) 0 0))
	(set-agent-joints dr_1 (list (+ 350 (* (cos m) 40)) (+ (* (sin m) 40) -50)
	 0 (- n 180) 0 10 4 60 0 0 0))
	(set-update t)
)
(setq la_1 '(390 -50 0 180 0 10 4 60 -4 0 0))
(setq tr_1 '(390 -50 0 180 0 0))
(setq ra_1 '(390 -50 0 180 0 10 4 60 0 0 0))

(do					;walk to attach left arm
	((n 1 (+ n 1)))
	((> n 20)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 390 (+ (* n 5) -50) 0 180 0 10 4 60 -4 0 0))
	(set-agent-joints dt_1 (list 390 (+ (* n 5) -50) 0 180 0 0))
	(set-agent-joints dr_1 (list 390 (+ (* n 5) -50) 0 180 0 10 4 60 0 0 0))
	(set-update t)
)
(setq la_1 '(390 50 0 180 0 10 4 60 -4 0 0))
(setq tr_1 '(390 50 0 180 0 0))
(setq ra_1 '(390 50 0 180 0 10 4 60 0 0 0))

(do					;walk and orient to attach left arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 390 (+ (* n 1.4) 50) 0 (+ 180 (* n 9)) 0 10 
	 4 60 -4 0 0))
	(set-agent-joints dt_1 (list 390 (+ (* n 1.4) 50) 0 (+ 180 (* n 9)) 0 0))
	(set-agent-joints dr_1 (list 390 (+ (* n 1.4) 50) 0 (+ 180 (* n 9)) 0 10 
	 4 60 0 0 0))
	(set-update t)
)
(setq la_1 '(390 64 0 270 0 10 4 60 -4 0 0))
(setq tr_1 '(390 64 0 270 0 0))
(setq ra_1 '(390 64 0 270 0 10 4 60 0 0 0))

(do					;orient left arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (+ 390 n) 64 0 270 0 (+ 10 (* n 3.5))
	 (- 4 (* n 3.5)) 60 (+ (* n 9.5) -4) (* n -4.5) (* n -2.7)))
	(set-agent-joints dt_1 (list (+ 390 n) 64 0 270 0 0))
	(set-agent-joints dr_1 (list (+ 390 n) 64 0 270 0 (+ 10 (* n 3.5))
	 (- 4 (* n 3.5)) 60 (* n 9.1) (* n 4.5) (* n -2.7)))
	(set-update t)
)
(setq la_1 '(400 64 0 270 0 45 -31 60 91 -45 -27))
(setq tr_1 '(400 64 0 270 0 0))
(setq ra_1 '(400 64 0 270 0 45 -31 60 91 45 -27))

(release dl_1)
(grasp dr_1 l_3)

(do					;orient left arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 400 64 0 270 0 (- 45 (* n 3.5))
	 (+ (* n 3.5) -31) 60 (- 91 (* n 9.1)) (+ (* n 4.5) -45)
	 (+ (* n 2.7) -27)))
	(set-agent-joints dt_1 (list 400 64 0 270 0 0))
	(set-agent-joints dr_1 (list 400 64 0 270 0 (- 45 (* n 3.5))
	 (+ (* n 3.5) -31) 60 (+ (* n 9.5) 91) (- 45 (* n 3.1)) (+ (* n 2.9) -27)))
	(set-update t)
)
(setq la_1 '(400 64 0 270 0 10 4 60 0 0 0))
(setq tr_1 '(400 64 0 270 0 0))
(setq ra_1 '(400 64 0 270 0 10 4 60 186 14 2))

(do					;assemble left arm
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (- 400 (* n 4)) 66 0 270 (* n .5) 
	 10 4 60 0 0 0))
	(set-agent-joints dt_1 (list (- 400 (* n 4)) 66 0 270 (* n .5) 0))
	(set-agent-joints dr_1 (list (- 400 (* n 4)) 66 0 270 (* n .5)
	 10 4 60 186 14 2))
	(set-update t)
)
(setq la_1 '(360 66 0 270 5 10 4 60 0 0 0))
(setq tr_1 '(360 66 0 270 5 0))
(setq ra_1 '(360 66 0 270 5 10 4 60 186 14 2))

(release dr_1)
(set-position l_3 (translate 45 69 5))

(do					;raise arms
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 360 66 0 270 (- 5 n)
	 10 4 (+ 60 (* n 4)) 0 0 0))
	(set-agent-joints dt_1 (list 360 66 0 270 (- 5 n) 0))
	(set-agent-joints dr_1 (list 360 66 0 270 (- 5 n)
	 10 4 (+ 60 (* n 4)) 0 0 0))
	(set-update t)
)
(setq la_1 '(360 66 0 270 0 10 4 80 0 0 0))
(setq tr_1 '(360 66 0 270 0 0))
(setq ra_1 '(360 66 0 270 0 10 4 80 0 0 0))

(do					;raise arms
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 360 66 0 270 0
	 (+ 10 (* n 16)) (- 4 (* n .8)) (- 80 (* n 16)) 0 0 0))
	(set-agent-joints dr_1 (list 360 66 0 270 0
	 (+ 10 (* n 16)) (- 4 (* n .8)) (- 80 (* n 16)) 0 0 0))
	(set-update t)
)
(setq la_1 '(360 66 0 270 0 90 0 0 0 0 0))
(setq tr_1 '(360 66 0 270 0 0))
(setq ra_1 '(360 66 0 270 0 90 0 0 0 0 0))

(do
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-look-from (- 400 (* n 20)) -100 (+ 165 (* n 6)))
	(set-camera 50 0 (+ 50 (* n 10)))
	(set-update t)
)
;(set-look-from 300 -100 195)
;(set-camera 50 0 100)

;	third robot
(setq dt_3 (make-trunk))		
(c_gray dt_3)
(c_red eye_1)
(setq dl_3 (make-larm))
(c_gray dl_3)
(c_cyan eye_1)
(setq dr_3 (make-rarm))
(c_gray dr_3)
(c_red eye_1)

(setq la_3 '(0 0 0 0 0 0 0 0 0 0 0))	; inital joint angles
(setq ra_3 '(0 0 0 0 0 0 0 0 0 0 0))
(setq tr_3 '(0 0 0 0 0 0))

(setq eye_3 (add-objects eye))		;give robot 'vision'
(c_gray eye_3)
(c_cyan eye_1)

(use-objects dl_3 dt_3 dr_3 eye_3)
(grasp dt_3 eye_3)

(set-update nil)

(set-agent-joints dl_3 (setq la_3 '(300 50 0 0 -90 0 0 0 0 0 0)))
(set-agent-joints dt_3 (setq tr_3 '(300 50 0 0 -90 0)))
(set-agent-joints dr_3 (setq ra_3 '(300 50 0 0 -90 0 0 0 0 0 0)))

(remove-objects l_3)
(remove-objects r_3)
(remove-objects torso_3)
(remove-objects head_3)

(set-update t)

(c_blue dl_3)
(c_muave dt_3)
(c_blue dr_3)
(c_gray dl_3)
(c_gray dt_3)
(c_gray dr_3)
(c_blue dl_3)
(c_muave dt_3)
(c_blue dr_3)
(c_yellow eye_3)

(do					;lower arms
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (+ 360 (* n 2)) 66 0 270 0
	 (- 90 (* n 18)) 0 (* n 16) 0 0 0))
	(set-agent-joints dt_1 (list (+ 360 (* n 2)) 66 0 270 0 0))
	(set-agent-joints dr_1 (list (+ 360 (* n 2)) 66 0 270 0
	 (- 90 (* n 18)) 0 (* n 16) 0 0 0))
	(set-update t)
)
(setq la_1 '(370 66 0 270 0 0 0 80 0 0 0))
(setq tr_1 '(370 66 0 270 0 0))
(setq ra_1 '(370 66 0 270 0 0 0 80 0 0 0))

(do					;finish lowering arms
	((n 1 (+ n 1)))
	((> n 5)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 370 66 0 270 0
	 0 0 (- 80 (* n 15)) 0 0 0))
	;(set-agent-joints dt_1 (list 370 66 0 270 0 0))
	(set-agent-joints dr_1 (list 370 66 0 270 0
	 0 0 (- 80 (* n 15)) 0 0 0))
	(set-update t)
)
(setq la_1 '(370 66 0 270 0 0 0 5 0 0 0))
(setq tr_1 '(370 66 0 270 0 0))
(setq ra_1 '(370 66 0 270 0 0 0 5 0 0 0))

(do					;raise other android
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_3 (list 300 50 0 0 (+ (* n 9) -90)
	 0 0 (* n 4.5) 0 0 0))
	(set-agent-joints dt_3 (list 300 50 0 0 (+ (* n 9) -90)))
	(set-agent-joints dr_3 (list 300 50 0 0 (+ (* n 9) -90)
	 0 0 (* n 4.5) 0 0 0))
	(set-update t)
)
(setq la_3 '(300 50 0 0 0 0 0 45 0 0 0))
(setq tr_3 '(300 50 0 0 0 0))
(setq ra_3 '(300 50 0 0 0 0 0 45 0 0 0))

(c_muave eye_3)
(c_yellow eye_3)

(do					;rotate androids
	((n 10 (+ n 10)))
	((> n 90)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 370 66 0 (+ n 270) 0
	 0 0 5 0 0 0))
	(set-agent-joints dl_3 (list 300 50 0 (- n) 0
	 0 0 45 0 0 0))
	(set-agent-joints dt_1 (list 370 66 0 (+ n 270) 0 0))
	(set-agent-joints dt_3 (list 300 50 0 (- n) 0))
	(set-agent-joints dr_1 (list 370 66 0 (+ n 270) 0
	 0 0 5 0 0 0))
	(set-agent-joints dr_3 (list 300 50 0 (- n) 0
	 0 0 45 0 0 0))
	(set-update t)
)
(setq la_1 '(370 66 0 360 0 0 0 5 0 0 0))
(setq tr_1 '(370 66 0 360 0 0))
(setq ra_1 '(370 66 0 360 0 0 0 5 0 0 0))
(setq la_3 '(300 50 0 -90 0 0 0 45 0 0 0))
(setq tr_3 '(300 50 0 -90 0 0))
(setq ra_3 '(300 50 0 -90 0 0 0 45 0 0 0))

(do					;rotate androids
	((n 1 (+ n 1)))
	((> n 10)t)
	(set-update nil)
	(set-agent-joints dl_1 (list 370 66 0 (* n 9) 0
	 0 0 5 0 0 0))
	(set-agent-joints dl_3 (list (- 300 (* n 6)) 50 0 -90 0
	 0 0 45 0 0 0))
	(set-agent-joints dt_1 (list 370 66 0 (* n 9) 0 0))
	(set-agent-joints dt_3 (list (- 300 (* n 6)) 50 0 -90 0))
	(set-agent-joints dr_1 (list 370 66 0 (* n 9) 0
	 0 0 5 0 0 0))
	(set-agent-joints dr_3 (list (- 300 (* n 6)) 50 0 -90 0
	 0 0 45 0 0 0))
	(set-update t)
)
(setq la_1 '(370 66 0 90 0 0 0 5 0 0 0))
(setq tr_1 '(370 66 0 90 0 0))
(setq ra_1 '(370 66 0 90 0 0 0 5 0 0 0))
(setq la_3 '(240 50 0 -90 0 0 0 45 0 0 0))
(setq tr_3 '(240 50 0 -90 0 0))
(setq ra_3 '(240 50 0 -90 0 0 0 45 0 0 0))

(do					;walk androids
	((n 1 (+ n 1)))
	((> n 16)t)
	(set-update nil)
	(set-agent-joints dl_1 (list (+ 370 (* n 10)) 66 0 90 0
	 (* 10 (cos (* n pi))) 0 5 0 0 0))
	(set-agent-joints dl_3 (list (- 240 (* n 10)) 50 0 -90 0
	 (* 10 (cos (* n pi))) 0 (- 45 (* n 2.5)) 0 0 0))
	(set-agent-joints dt_1 (list (+ 370 (* n 10)) 66 0 90 0 0))
	(set-agent-joints dt_3 (list (- 240 (* n 10)) 50 0 -90 0))
	(set-agent-joints dr_1 (list (+ 370 (* n 10)) 66 0 90 0
	 (* -10 (cos (* n pi))) 0 5 0 0 0))
	(set-agent-joints dr_3 (list (- 240 (* n 10)) 50 0 -90 0
	 (* -10 (cos (* n pi))) 0 (- 45 (* n 2.5)) 0 0 0))
	(set-update t)
)
(setq la_1 '(520 66 0 90 0 0 0 5 0 0 0))
(setq tr_1 '(520 66 0 90 0 0))
(setq ra_1 '(520 66 0 90 0 0 0 5 0 0 0))
(setq la_3 '(90 50 0 -90 0 0 0 7.5 0 0 0))
(setq tr_3 '(90 50 0 -90 0 0))
(setq ra_3 '(90 50 0 -90 0 0 0 7.5 0 0 0))

;(remove-objects dl_1)
;(remove-objects dl_3)
;(remove-objects dr_1)
;(remove-objects dr_3)
;(remove-objects dt_1)
;(remove-objects dt_3)

;positioning sphere
;(setq sph (make-sphere 1 12))
;(set-position sph (translate 310 -30 110))
;(c_yellow sph)
;(use-objects sph)

;(ask-dialog
;	#'(lambda (j1 j2 j3 j4 j5 j6 j7 j8 j9 j10 j11)
;		(set-agent-joints dl_1 (list j1 j2 j3 j4 j5 j6 j7 j8 j9 j10 j11))
;	)
;	"dl_1 control"
;	"x" (nth 0 la_1) "y" (nth 1 la_1) "z" (nth 2 la_1) "Orient" (nth 3 la_1)
;	  "Bend" (nth 4 la_1) "S_fwd" (nth 5 la_1) "S_up" (nth 6 la_1)
;	  "Elbow" (nth 7 la_1) "W_roll" (nth 8 la_1) "W_pitch" (nth 9 la_1)
;	  "W_yaw" (nth 10 la_1)
;)
;(ask-dialog
;	#'(lambda (j1 j2 j3 j4 j5 j6 j7 j8 j9 j10 j11)
;		(set-agent-joints dr_1 (list j1 j2 j3 j4 j5 j6 j7 j8 j9 j10 j11))
;	)
;	"dr_1 control"
;	"x" (nth 0 ra_1) "y" (nth 1 ra_1) "z" (nth 2 ra_1) "Orient" (nth 3 ra_1)
;	  "Bend" (nth 4 ra_1) "S_fwd" (nth 5 ra_1) "S_up" (nth 6 ra_1)
;	  "Elbow" (nth 7 ra_1) "W_roll" (nth 8 ra_1) "W_pitch" (nth 9 ra_1)
;	  "W_yaw" (nth 10 ra_1)
;)

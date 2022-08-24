;(maximize t)
(clear-simulation)
(enable-solid t)
(enable-lighting t)
(set-clipplane 40 20000)
;(set-background-color '(255 255 255))
;(set-collision-check t)

; use pumainv.lsp for inverse kinematics written in LISP
;(load "pumainv.lsp")
; use pumainv.dll for inverse kinematics written in C
(use-kinematics "pumainv.dll")

(set-window-menu
	'(
		("&File"
			"&New" (winexec "notepad")
			"&Open..." 
				(let
					(
						(name (get-file-dialog "LISP Files (*.lsp)|*.lsp" "LISP Files (*.lsp)|*.lsp|All Files (*.*)|*.*||"))
					)
					(cond
						((eq name nil) nil)
						(t (winexec "notepad " name))
					)
				)
			nil
			"&Load..." (load (get-file-dialog "LISP Files (*.lsp)|*.lsp" "LISP Files (*.lsp)|*.lsp|All Files (*.*)|*.*||"))
			"&Clear Simulation" (clear-simulation)
			nil
			"&Print..." (printer-dialog)
			nil
			"E&xit" (exit)
		)
		("&Edit"
			"&Copy" (copy-window t)
			"&Delete" (remove-objects)
		)
		("&Options"
			"&Command Window Font..." (set-font-dialog)
			"&Command Window Height..." (ask-command-lines)
			nil
			"&Wireframe" (enable-solid nil)
			"&Solid" (enable-solid t)
			nil
			"&Lighting" (enable-lighting t)
			"&No lighting" (enable-lighting nil)
			nil
			"&Backface polygon cull" (enable-cull t)
			"No cull" (enable-cull nil)
			nil
			"&Smooth Shading" (enable-smooth-shading t)
			"&Flat Shading" (enable-smooth-shading nil)
			nil
			"&Set View..." (set-view-dialog)
			"&Lights..." (adjust-lights)
		)
		("&Simulation"
			"C&artesian Robot, Normal" (cartesian-robot-normal)
;			"Cartesian &Robot, 3d" (cartesian-robot-3d)
			"&St-Line Tracking, Normal" (3r-demo-normal)
;			"St-&Line Tracking, 3D" (3r-demo-3d)
			"&Bouncing Ball, Normal" (bouncing-ball-normal)
;			"Bou&ncing Ball, 3D" (bouncing-ball-3d)
			"Tracking &Puma, Normal" (tracking-puma-normal)
;			"Tracking P&uma, 3D" (tracking-puma-3d)
;			"&Multiple Views" (multview-puma-tracking)
			"&Welding, Normal" (welding-normal)
;			"W&elding, 3D" (welding-3d)
			"&Tower, Normal" (tower-normal)
;			"T&ower, 3D" (tower-3d)
			"&Insert Box" (insert-box)
			"Insert Sphere" (insert-sphere)
			"&Joint Control" (puma-control)
			"&Cartesian Control" (cartesian-control)
			"&8 Pumas" (puma8)
			"&Fly By, 3D" (demo3d)
			"Bas&ketball" (basketball-demo)
			"Android-&2" (android-demo)
			"Space Station" (load "sstation.lsp")	;this file will execute everything necessary
		)
		("&Help"
			"&Contents" (display-help "robo.chm")
			"&About..." (about-dialog)
		)
	)
)

(new-view)
(maximize-active-window)
(set-double-buffer t)

(load "puma560.lsp")
(load "tower.lsp")
(load "puma8.lsp")
(load "t3.lsp")
(load "tracking.lsp")
;(load "weld.lsp")
;(load "demo.lsp")
(load "demo3d.lsp")

(defun clipplane nil
	(ask-dialog #'mplane "Enter clipping plane" '("Near" 10.0) '("Far" 1000))
)

(defun mplane (near far)
	(if (<= near 0) (return-from mplane "Near plane must be greater than zero"))
	(if (<= far 0) (return-from mplane "Far plane must be greater than zero"))
	(if (<= far near) (return-from mplane "Far plane must be greater than near"))
	(set-clipplane near far)
)

(defun ask-command-lines nil
	(ask-dialog
		#'(lambda (l)
			(if
				(or (< l 0) (> l 24))
				(return "Lines must be (0-24)")
				(set-command-lines l)
			)
		)
		"Set Command Window Height"
		'("Command Window Height (0-24)" 8)
	)
)

(defun ask-eye-sep nil
	(ask-dialog
		#'set-eye-separation
		"Set Eye Separation"
		'("Eye Separation" 15)
	)
)

(defun move-to-grasp (rob obj grasp sol steps)
	(move-inter-to rob (matmult (get-position obj) grasp) sol steps)
)

(defun boxdemo nil
	(remove-objects)
	(setq robot (make-puma560))
	(setq box1 (make-box 10 10 10))
	(setq box2 (make-box 10 10 5))
	(black robot)
	(black box1)
	(black box2)
	(set-position box1 (translate 20 20 0))
	(set-position box2 (translate -20 20 0))
	(setq box1-grasp (matmult (translate 0 0 10) (rotatex 180)))
	(setq box2-grasp (matmult (translate 0 0 5) (rotatex 180)))
	(use-objects robot box1 box2)
	(move-to-grasp robot box1 box1-grasp 2 10)
	(grasp robot box1)
	(move-inter-to robot (matmult (translate 20 -20 10) (rotatex 180)) 2 10)
	(release robot)
	(move-to-grasp robot box2 box2-grasp 2 10)
)

(defun insert-box nil
	(set-look-at 0 0 0)
	(set-look-from 100 100 100)
	(set-camera 100 0 100)
	(setq tbox (make-box 10 10 10))
	(use-objects tbox)
	(if 
		(null
			(ask-dialog #'mbox "Enter box parameters" 
				'("X" 10.0) '("Y" 10.0) '("Z" 10.0)
				'("Ambient - Red" 1.0 .1) '("Ambient - Green" 0.0 .1) '("Ambient - Blue" 0.0 .1)
				'("Diffuse - Red" 1.0 .1) '("Diffuse - Green" 0.0 .1) '("Diffuse - Blue" 0.0 .1)
				'("Specular - Red" 0.0 .1) '("Specular - Green" 0.0 .1) '("Specular - Blue" 0.0 .1)
			)
		)
		(remove-objects tbox)
	)
)

(defun insert-sphere nil
	(set-look-at 0 0 0)
	(set-look-from 100 100 100)
	(set-camera 100 0 100)
	(setq tsphere (make-sphere 10 20))
	(use-objects tsphere)
	(if 
		(null
			(ask-dialog #'msphere "Enter sphere parameters" 
				'("Radius" 10.0)
				'("Facets" 20)
				'("Ambient - Red" 1.0 .1)
				'("Ambient - Green" 0.0 .1)
				'("Ambient - Blue" 0.0 .1)
				'("Diffuse - Red" 1.0 .1)
				'("Diffuse - Green" 0.0 .1)
				'("Diffuse - Blue" 0.0 .1)
				'("Specular - Red" 0.0 .1)
				'("Specular - Green" 0.0 .1)
				'("Specular - Blue" 0.0 .1)
			)
		)
		(remove-objects tsphere)
	)
)

(defun msphere (r f ar ag ab dr dg db sr sg sb)
	(if (<= r 0) (return-from msphere "Radius must be greater than zero"))
	(if (<= f 0) (return-from msphere "Facets must be greater than zero"))
	(if (or (< ar -1) (> ar 1)) (return-from msphere "Colors must be between -1 and 1"))
	(if (or (< ag -1) (> ag 1)) (return-from msphere "Colors must be between -1 and 1"))
	(if (or (< ab -1) (> ab 1)) (return-from msphere "Colors must be between -1 and 1"))
	(if (or (< dr -1) (> dr 1)) (return-from msphere "Colors must be between -1 and 1"))
	(if (or (< dg -1) (> dg 1)) (return-from msphere "Colors must be between -1 and 1"))
	(if (or (< db -1) (> db 1)) (return-from msphere "Colors must be between -1 and 1"))
	(if (or (< sr -1) (> sr 1)) (return-from msphere "Colors must be between -1 and 1"))
	(if (or (< sg -1) (> sg 1)) (return-from msphere "Colors must be between -1 and 1"))
	(if (or (< sb -1) (> sb 1)) (return-from msphere "Colors must be between -1 and 1"))
	(set-update nil)
	(remove-objects tsphere)
	(setq tsphere (make-sphere r f))
	(set-object tsphere :ambient (list ar ag ab) :diffuse (list dr dg db) :specular (list sr sg sb))
	(use-objects tsphere)
	(set-update t)
)

(defun mbox (x y z ar ag ab dr dg db sr sg sb)
	(if (<= x 0) (return-from mbox "X must be greater than zero"))
	(if (<= y 0) (return-from mbox "Y must be greater than zero"))
	(if (<= z 0) (return-from mbox "Z must be greater than zero"))
	(if (or (< ar -1) (> ar 1)) (return-from mbox "Colors must be between -1 and 1"))
	(if (or (< ag -1) (> ag 1)) (return-from mbox "Colors must be between -1 and 1"))
	(if (or (< ab -1) (> ab 1)) (return-from mbox "Colors must be between -1 and 1"))
	(if (or (< dr -1) (> dr 1)) (return-from mbox "Colors must be between -1 and 1"))
	(if (or (< dg -1) (> dg 1)) (return-from mbox "Colors must be between -1 and 1"))
	(if (or (< db -1) (> db 1)) (return-from mbox "Colors must be between -1 and 1"))
	(if (or (< sr -1) (> sr 1)) (return-from mbox "Colors must be between -1 and 1"))
	(if (or (< sg -1) (> sg 1)) (return-from mbox "Colors must be between -1 and 1"))
	(if (or (< sb -1) (> sb 1)) (return-from mbox "Colors must be between -1 and 1"))
	(set-update nil)
	(remove-objects tbox)
	(setq tbox (make-box x y z))
	(set-object tbox :ambient (list ar ag ab) :diffuse (list dr dg db) :specular (list sr sg sb))
	(use-objects tbox)
	(set-update t)
)

(defun nv3 nil
	(prog (eyesep)
		(setq eyesep (ask-dialog nil "Eye Separation" '("Eye Separation" 15.0)))
		(if (null eyesep) (return nil))
		(setq eyesep (car eyesep))
		(new-view3d eyesep)
	)
)

(defun 3r-demo-normal nil
	(clear-simulation)
	(new-view)
	(maximize-active-window)
	(set-update t)
	(load "3r-demo.lsp")
)

(defun 3r-demo-3d nil
	(clear-simulation)
	(new-view3d 100)
	(maximize-active-window)
	(set-update t)
	(load "3r-demo.lsp")
)

(defun cartesian-robot-normal nil
	(clear-simulation)
	(new-view)
	(maximize-active-window)
	(set-update t)
	(load "cartdemo.lsp")
)

(defun cartesian-robot-3d nil
	(clear-simulation)
	(new-view3d 600)
	(maximize-active-window)
	(set-update t)
	(load "cartdemo.lsp")
)

(defun bouncing-ball-normal nil
	(clear-simulation)
	(new-view)
	(maximize-active-window)
	(set-update t)
	(load "balldemo.lsp")
)

(defun bouncing-ball-3d nil
	(clear-simulation)
	(new-view3d 100)
	(maximize-active-window)
	(set-update t)
	(load "balldemo.lsp")
)

(defun tracking-puma-normal nil
	(clear-simulation)
	(new-view)
	(maximize-active-window)
	(set-update t)
	(load "pumtrack.lsp")
)

(defun tracking-puma-3d nil
	(clear-simulation)
	(new-view3d)
	(maximize-active-window)
	(set-update t)
	(load "pumtrack.lsp")
)

(defun welding-normal nil
	(clear-simulation)
	(new-view)
	(maximize-active-window)
	(set-update t)
	(load "weld.lsp")
	(weld)
)

(defun welding-3d nil
	(clear-simulation)
	(new-view3d 15)
	(maximize-active-window)
	(set-update t)
	(load "weld.lsp")
	(weld)
)

(defun tower-normal nil
	(clear-simulation)
	(new-view)
	(maximize-active-window)
	(set-update t)
	(load  "tower.lsp")
	(tower)
)

(defun tower-3d nil
	(clear-simulation)
	(new-view3d 15)
	(maximize-active-window)
	(set-update t)
	(load "tower.lsp")
	(tower)
)

(defun basketball-demo nil
	(clear-simulation)
	(new-view)
	(maximize-active-window)
	(set-update t)
	(load "demo-kth.lsp")
)

(defun android-demo nil
	(clear-simulation)
	(new-view)
	(set-update t)
	(load "demo-rlc.lsp")
)

(defun multview-puma-tracking nil
	(set-update t)
	(load "multview.lsp")
)

(defun puma-control nil
	(clear-simulation)
	(new-view)
	(maximize-active-window)
	(set-update t)
	(set-cam-puma)
	(setq pum1 (set-object (make-puma560) :color '(0 255 255)))
;	(set-position pum1 (translate -50 0 0))
	(setq floor (make-box 26 26 2))
	(set-object floor :color '(255 255 0))
	(set-position floor (translate 0 0 -2))
;	(set-position floor (translate -50 0 -2))
	(remove-objects)
	(use-objects pum1 floor)
	(joint-control pum1)
	(setq pum1 nil)
)
	
(defun joint-control (ob)
	(setq crobot ob)
	(ask-dialog
		#'(lambda (t1 t2 t3 t4 t5 t6)
			(set-agent-joints crobot (list t1 t2 t3 t4 t5 t6))
		)
		"Joint Control"
		'("1" 0.0) '("2" 0.0) '("3" 0.0) '("4" 0.0) '("5" 0.0) '("6" 0.0)
	)
	(setq crobot nil)
)

(defun cartesian-control nil
	(clear-simulation)
	(new-view)
	(maximize-active-window)
	(set-update t)
	(set-cam-puma)
	(setq pum1 (set-object (make-puma560) :color '(255 0 255)))
	(set-position pum1 (translate -50 0 0))
	(set-object pum1 :color '(255 255 0))
	(setq floor (make-box 26 26 2))
	(set-position floor (translate -50 0 -2))
	(set-object floor :color '(0 255 0))
	(use-objects pum1 floor)
	(ask-dialog
		#'cartesian
		"Cartesian Control"
		'("Solution" 0)
;		"X" 13.0 "Y" 14.0 "Z" 1.0 "Roll" 0.0 "Pitch" 180.0 "Yaw" 0.0
		'("X" -37.0) '("Y" 13.0) '("Z" 1.0) '("Roll" 0.0) '("Pitch" 180.0) '("Yaw" 0.0)
	)
)

(defun cartesian (idx x y z roll pitch yaw)
	(if (or (< idx 0) (> idx 7)) (return-from cartesian "Solution (0-7)"))
	(setq sol (get-solution pum1 (matmult (translate x y z) (rpy roll pitch yaw)) idx))
	(if (null sol) (return-from cartesian "Out of Reach"))
	(set-agent-joints pum1 sol)
)

(defun set-cam-puma nil
	(set-look-from 100 100 30)
	(set-look-at 0 0 20)
	(set-camera 50 0 200)
)

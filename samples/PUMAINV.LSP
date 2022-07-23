; compute-parameters returns nil if invalid
; otherwise returns list of parameters
; params is a list of joint parameters
; ->
;(
;	(matrix1 min1 max1 type1) ; joint 1
;	(matrix2 min2 max2 type2) ; joint 2
;)

;;;;;;;;;;;;;;;;;;;;;;
(defun match (x y)
	(cond
		((null y) x)
		( (< (abs (- x y)) .00001) t)
		(t (throw 'match-failed nil))
	)
)

(defun match-joint (joint dhparms)
	(let
		(dh)
		(setq dh (get-d-h joint))
		(if (null dh) (throw 'match-failed nil))
		(mapcar #'match dh dhparms)
	)
)

(defun puma-compute-parameters (params)	; params is list of jointdefs
								; a jointdef is a list of the form (matrix min max type)
								; where type = 0 fixed, 1 prismatic, 2 revolute
	(let
		(
			a2 a3 d2 d4 d6 
			(pre (scale 1 1 1))
			(post (scale 1 1 1))
			(postflg nil)
			(movcnt 0)
			(movlist nil)
		) ; declare parameters
	
		; the following loop checks for 0 or more fixed joints
		; followed by 6 movable joints
		; followed by 0 or more fixed joints
		(do*									;scan joints
			(
				type
				(joints params (cdr joints))	;initialize
			)
			( (null joints) nil) 				;repeat until joint is nil
			(setq type (cadddr (car joints)))
			(if (= type 0) 						; if fixed link
				(if (= movcnt  0) 				; if no movable links yet
					(setq pre (matmult pre (caar joints)))	;then add link to pre matrix
					(progn						;else
						(setq post (matmult post (caar joints)))	;add to post matrix
						(setq postflg t)		; set flag indicating fixed joint after movable joint
					)
				)								
				(if (null postflg)              ; if no fixed joints after movable joints yet
					(progn						; then 
						(setq movlist (cons (caar joints) movlist)) ;add the matrix for the joint to the list
						(setq movcnt (1+ movcnt))	;increment the number of movable joints so far
					)
					(return-from puma-compute-parameters nil)	;failed -- not a match 
				)
			)
		)										; end of do
		; pre contains the 0 or more fixed joint matrices at the beginning of the chain concatenated together
		; post contains the 0 or more fixed joint matrices at the end of the chain concatenated together
		; movlist now contains the joint matrices (m6 m5 m4 m3 m2 m1)
		; movcnt contains number of movable joints

		(if (/= movcnt 6) (return-from puma-compute-parameters nil)) ; if movcnt != 6 return nil

		(setq movlist 
			(catch 'match-failed
				(mapcar 
					#'match-joint 
					movlist
					'(
						(0 nil 0 0)		;d6
						(0 0 0 90)
						(0 nil 0 -90)	;d4
						(0 0 nil 90)	;a3
						(0 nil nil 0)	;d2 a2
						(0 0 0 -90)
					)
				)
			)
		)

		(if (null movlist) (return-from puma-compute-parameters nil)) ; failed

		(setq d6 (cadr (nth 0 movlist)))
		(setq d4 (cadr (nth 2 movlist)))
		(setq a3 (caddr (nth 3 movlist)))
		(setq d2 (cadr (nth 4 movlist)))
		(setq a2 (caddr (nth 4 movlist)))

		(list a2 d2 a3 d4 d6 
			(invert-transform pre) 
			(invert-transform post)
		)	; this list is returned as the list of parameters
	)											; end of let block
)

; get-num-solutions returns number of solutions
(defun puma-get-num-solutions nil
	8
)

(defun puma-get-degrees-of-freedom nil
	6
)

(defun puma-inverse-kinematics (params target)
	(let
		(
			(a2 (nth 0 params))
			(d2 (nth 1 params))
			(a3 (nth 2 params))
			(d4 (nth 3 params))
			(d6 (nth 4 params))
			(pre (nth 5 params))
			(post (nth 6 params))
			mat n s a p6 p			;local variables
		)
		(setq mat (matmult pre target post))
		(setq n (list (tref mat 0 0) (tref mat 1 0) (tref mat 2 0)))
		(setq s (list (tref mat 0 1) (tref mat 1 1) (tref mat 2 1)))
		(setq a (list (tref mat 0 2) (tref mat 1 2) (tref mat 2 2)))
		(setq p6 (list (tref mat 0 3) (tref mat 1 3) (tref mat 2 3)))
		(setq p
			(list
				(- (car p6) (* d6 (car a)))
				(- (cadr p6) (* d6 (cadr a)))
				(- (caddr p6) (* d6 (caddr a)))
			)
		)
;		(setq p
;			(mapcar
;				(function (lambda (x y) (- x (* d6 y))))
;				p6
;				a
;			)
;		)

		(catch 'ik-range
			(let
				(theta0 theta1 theta2 theta3 theta4 theta5 arm elbow wrist)
				(list
					(progn
						(setq arm -1 elbow -1 wrist -1)
						(setq theta0 (puma-compute-theta0 p params arm))
						(setq theta1 (puma-compute-theta1 p params arm elbow))
						(setq theta2 (puma-compute-theta2 p params arm elbow))
						(setq theta3 (puma-compute-theta3 a params wrist theta0 theta1 theta2))
						(setq theta4 (puma-compute-theta4 a theta0 theta1 theta2 theta3))
						(setq theta5 (puma-compute-theta5 n s theta0 theta1 theta2 theta3))
						(mapcar
							(function (lambda (x) (/ (* x 180.0) pi)))
							(list theta0 theta1 theta2 theta3 theta4 theta5)
						)
					)
					(progn
						(setq arm -1 elbow -1 wrist 1)
						(setq theta0 (puma-compute-theta0 p params arm))
						(setq theta1 (puma-compute-theta1 p params arm elbow))
						(setq theta2 (puma-compute-theta2 p params arm elbow))
						(setq theta3 (puma-compute-theta3 a params wrist theta0 theta1 theta2))
						(setq theta4 (puma-compute-theta4 a theta0 theta1 theta2 theta3))
						(setq theta5 (puma-compute-theta5 n s theta0 theta1 theta2 theta3))
						(mapcar
							(function (lambda (x) (/ (* x 180.0) pi)))
							(list theta0 theta1 theta2 theta3 theta4 theta5)
						)
					)
					(progn
						(setq arm -1 elbow 1 wrist -1)
						(setq theta0 (puma-compute-theta0 p params arm))
						(setq theta1 (puma-compute-theta1 p params arm elbow))
						(setq theta2 (puma-compute-theta2 p params arm elbow))
						(setq theta3 (puma-compute-theta3 a params wrist theta0 theta1 theta2))
						(setq theta4 (puma-compute-theta4 a theta0 theta1 theta2 theta3))
						(setq theta5 (puma-compute-theta5 n s theta0 theta1 theta2 theta3))
						(mapcar
							(function (lambda (x) (/ (* x 180.0) pi)))
							(list theta0 theta1 theta2 theta3 theta4 theta5)
						)
					)
					(progn
						(setq arm -1 elbow 1 wrist 1)
						(setq theta0 (puma-compute-theta0 p params arm))
						(setq theta1 (puma-compute-theta1 p params arm elbow))
						(setq theta2 (puma-compute-theta2 p params arm elbow))
						(setq theta3 (puma-compute-theta3 a params wrist theta0 theta1 theta2))
						(setq theta4 (puma-compute-theta4 a theta0 theta1 theta2 theta3))
						(setq theta5 (puma-compute-theta5 n s theta0 theta1 theta2 theta3))
						(mapcar
							(function (lambda (x) (/ (* x 180.0) pi)))
							(list theta0 theta1 theta2 theta3 theta4 theta5)
						)
					)
					(progn
						(setq arm 1 elbow -1 wrist -1)
						(setq theta0 (puma-compute-theta0 p params arm))
						(setq theta1 (puma-compute-theta1 p params arm elbow))
						(setq theta2 (puma-compute-theta2 p params arm elbow))
						(setq theta3 (puma-compute-theta3 a params wrist theta0 theta1 theta2))
						(setq theta4 (puma-compute-theta4 a theta0 theta1 theta2 theta3))
						(setq theta5 (puma-compute-theta5 n s theta0 theta1 theta2 theta3))
						(mapcar
							(function (lambda (x) (/ (* x 180.0) pi)))
							(list theta0 theta1 theta2 theta3 theta4 theta5)
						)
					)
					(progn
						(setq arm 1 elbow -1 wrist 1)
						(setq theta0 (puma-compute-theta0 p params arm))
						(setq theta1 (puma-compute-theta1 p params arm elbow))
						(setq theta2 (puma-compute-theta2 p params arm elbow))
						(setq theta3 (puma-compute-theta3 a params wrist theta0 theta1 theta2))
						(setq theta4 (puma-compute-theta4 a theta0 theta1 theta2 theta3))
						(setq theta5 (puma-compute-theta5 n s theta0 theta1 theta2 theta3))
						(mapcar
							(function (lambda (x) (/ (* x 180.0) pi)))
							(list theta0 theta1 theta2 theta3 theta4 theta5)
						)
					)
					(progn
						(setq arm 1 elbow 1 wrist -1)
						(setq theta0 (puma-compute-theta0 p params arm))
						(setq theta1 (puma-compute-theta1 p params arm elbow))
						(setq theta2 (puma-compute-theta2 p params arm elbow))
						(setq theta3 (puma-compute-theta3 a params wrist theta0 theta1 theta2))
						(setq theta4 (puma-compute-theta4 a theta0 theta1 theta2 theta3))
						(setq theta5 (puma-compute-theta5 n s theta0 theta1 theta2 theta3))
						(mapcar
							(function (lambda (x) (/ (* x 180.0) pi)))
							(list theta0 theta1 theta2 theta3 theta4 theta5)
						)
					)
					(progn
						(setq arm 1 elbow 1 wrist 1)
						(setq theta0 (puma-compute-theta0 p params arm))
						(setq theta1 (puma-compute-theta1 p params arm elbow))
						(setq theta2 (puma-compute-theta2 p params arm elbow))
						(setq theta3 (puma-compute-theta3 a params wrist theta0 theta1 theta2))
						(setq theta4 (puma-compute-theta4 a theta0 theta1 theta2 theta3))
						(setq theta5 (puma-compute-theta5 n s theta0 theta1 theta2 theta3))
						(mapcar
							(function (lambda (x) (/ (* x 180.0) pi)))
							(list theta0 theta1 theta2 theta3 theta4 theta5)
						)
					)
				)
			)
		)
	)		;end let
)			;end puma-inverse-kinematics

(defun checktrig (x)
	(if (> (abs x) 1.) (throw 'ik-range nil))
)

(defun puma-compute-theta0 (p params arm)
	(let
		(
			(pd2 (nth 1 params))		;parameter d2
			(px (car p))				;px
			(py (cadr p))				;py
			 temp
		)
		(setq temp
			(-
				(+ (* px px) (* py py))
				(* pd2 pd2)
			)
		)
		(if (< temp 0.0) (throw 'ik-range nil))
		(setq temp (sqrt temp))
		(atan
			(- (* (- arm) py temp) (* px pd2) )
			(- (* py pd2) (* arm px temp) )
		)
	)
)			;end puma-compute-theta0

(defun puma-compute-theta1 (p params arm elbow)
	(let
		(
			(pa2 (nth 0 params))	; parameter a2
			(pd2 (nth 1 params))	; parameter d2
			(pa3 (nth 2 params))	; parameter a3
			(pd4 (nth 3 params))	; parameter d4
			(px (car p))
			(py (cadr p))
			(pz (caddr p))
			bigr r sina cosa sinb cosb
		)			 
		(setq bigr (+ (* px px) (* py py) (* pz pz) (- (* pd2 pd2))))
		(if (<= bigr 0.0) (throw 'ik-range nil))
		(setq r (+ (* px px) (* py py) (- (* pd2 pd2))))
		(if (<= r 0.0) (throw 'ik-range nil))
		(setq bigr (sqrt bigr))
		(setq r (sqrt r))
		(setq sina (/ (- pz) bigr))
		(setq cosa (/ (* (- arm) r) bigr)) 
		(checktrig sina)
		(checktrig cosa)
		(setq cosb
			(/
				(+ (* pa2 pa2) (* bigr bigr) (- (* pd4 pd4)) (- (* pa3 pa3)))
				(* 2 pa2 bigr)
			)
		)
		(checktrig cosb)
		(setq sinb (sqrt (- 1 (* cosb cosb))))
		(atan
			(+ (* sina cosb) (* arm elbow cosa sinb))
			(- (* cosa cosb) (* arm elbow sina sinb))
		)
	)
)

(defun puma-compute-theta2 (p params arm elbow)
	(let
		(
			(pa2 (nth 0 params))	; parameter a2
			(pd2 (nth 1 params))	; parameter d2
			(pa3 (nth 2 params))	; parameter a3
			(pd4 (nth 3 params))	; parameter d4
			(px (car p))
			(py (cadr p))
			(pz (caddr p))
			bigr sinp cosp sinb cosb d4a3
		)
		(setq bigr (+ (* px px) (* py py) (* pz pz) (- (* pd2 pd2))))
		(if (<= bigr 0.0) (throw 'ik-range nil))
		(setq bigr (sqrt bigr))
		(setq d4a3 (+ (* pd4 pd4) (* pa3 pa3)))
		(setq cosp
			(/
				(+ (* pa2 pa2) d4a3 (- (* bigr bigr)))
				(* 2 pa2 (sqrt d4a3))
			)
		)
		(setq sinp (* arm elbow (sqrt (- 1 (* cosp cosp)))))
		(setq cosb (/ (abs pa3) (sqrt d4a3)))
		(setq sinb (/ pd4 (sqrt d4a3)))
		(checktrig sinp)
		(checktrig cosp)
		(checktrig sinb)
		(checktrig cosb)
		(atan
			(- (* sinp cosb) (* cosp sinb))
			(+ (* cosp cosb) (* sinp sinb))
		)
	)
)

(defun puma-compute-theta3 (pa params wrist theta0 theta1 theta2)
	(let
		(
			(ax (car pa))
			(ay (cadr pa))
			(az (caddr pa))
			(s1 (sin theta0))
			(c1 (cos theta0))
			(s23 (sin (+ theta1 theta2)))
			(c23 (cos (+ theta1 theta2)))
			st ct
		)
		(setq st (* wrist (- (* c1 ay) (* s1 ax))))
		(setq ct (* wrist (+ (* c1 c23 ax) (* s1 c23 ay) (- (* s23 az)))))
		(atan st ct)
	)
)

(defun puma-compute-theta4 (a theta0 theta1 theta2 theta3)
	(let
		(
			(ax (car a))
			(ay (cadr a))
			(az (caddr a))
			(s1 (sin theta0))
			(c1 (cos theta0)) 
			(s4 (sin theta3))
			(c4 (cos theta3)) 
			(s23 (sin (+ theta1 theta2)))
			(c23 (cos (+ theta1 theta2)))
			st ct
		)
		(setq st 
			(+
				(* (- (* c1 c23 c4) (* s1 s4)) ax)
				(* (+ (* s1 c23 c4) (* c1 s4)) ay)
				(- (* c4 s23 az))
			)
		)
		(setq ct (+ (* c1 s23 ax) (* s1 s23 ay) (* c23 az)))
		(atan st ct)
	)
)

(defun puma-compute-theta5 (n s theta0 theta1 theta2 theta3)
	(let
		(
			(nx (car n))
			(ny (cadr n))
			(nz (caddr n))
			(sx (car s))
			(sy (cadr s))
			(sz (caddr s))

			(s1 (sin theta0))
			(c1 (cos theta0)) 
			(s4 (sin theta3))
			(c4 (cos theta3)) 
			(s23 (sin (+ theta1 theta2)))
			(c23 (cos (+ theta1 theta2)))
			st ct
		)

		(setq st 
			(+
				(* (- (- (* s1 c4)) (* c1 c23 s4)) nx)
				(* (- (* c1 c4) (* s1 c23 s4)) ny)
				(* s4 s23 nz)
			)
		)
		(setq ct
			(+
				(* (- (- (* s1 c4)) (* c1 c23 s4)) sx)
				(* (- (* c1 c4) (* s1 c23 s4)) sy)
				(* s4 s23 sz)
			)
		)
		(atan st ct)
	)
)

(use-kinematics-lisp 
	#'puma-inverse-kinematics 
	#'puma-get-num-solutions 
	#'puma-get-degrees-of-freedom 
	#'puma-compute-parameters
)


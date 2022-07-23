(defun move-it (ob)
	(remove-objects)
	(set-position ob (translate 0 0 0))
	(use-objects ob)
	(do ( (angle 0 (+ angle 5)))
		((= angle 360) t)
		(set-agent-joints ob (list angle angle angle angle angle))
		nil
	)
	(set-agent-joints ob (list 0 0 0 0 0 0))
)

(defun simulate (agent ob)
	(remove-objects)
	(set-position agent (translate 0 0 0))
	(use-objects agent ob)
	(do ( (xpos 0 (+ xpos 5)))
		((= xpos 200) t)
		(set-position ob (translate xpos 0 0))
	)
)

(defun color-cycle (ob)
	(remove-objects)
	(set-object ob :color '(255 255 255))
	(set-position ob (translate 0 0 0))
	(use-objects ob t)
	(do ( (xpos 255 (- xpos 1)))
		((= xpos 0) t)
			(set-object ob :color (list xpos xpos xpos))
	)
)

(defun ask-test nil
	(ask-dialog "Test" "int" :int "float" :float "string" :string "symbol" :symbol)
)

(defun twosim (ob)
	(remove-objects)
	(set-position ob (translate 0 0 0))
	(use-objects ob)
	(do ( (angle 0 (+ angle .1)))
		((> angle 6.28) t)
		(set-agent-joints ob
			(list (* 10 angle)
				(list angle angle angle angle angle)
				(list (- angle) (- angle) (- angle) (- angle) (- angle))
			)
		)
	)
	(set-agent-joints ob
		(list 0
			(list 0 0 0 0 0 0)
			(list 0 0 0 0 0 0)
		)
	)
)

(defun sline (ob idsol)
	(remove-objects)
;	(set-position ob (translate 0 0 0))
	(use-objects ob)
	(do ( (ypos 8 (+ ypos .5)))
		((>= ypos 16) t)
		(set-agent-joints ob 
			(get-solution
				ob
				(matmult (translate ypos ypos 0) (rotatex 180))
				idsol)
		)
		nil
	)
	(set-agent-joints ob (list 0 0 0 0 0 0))
)

(defun make-joint (idx theta)
	(do 	(
		(cnt idx (1- cnt))
		(tlist (list theta) (cons nil tlist))
		)
		((= cnt 0) tlist)
	)
)

(defun exercise (ob idx)
	(remove-objects)
	(use-objects ob)
	(drive-agent ob 1 (make-joint idx 0))
	(drive-agent ob 36 (make-joint idx 360))
	(drive-agent ob 1 (make-joint idx 0))
)

(defun exer (ob idx)
	(remove-objects)
	(use-objects ob)
	(do ( (theta 0 (+ theta 5)))
		((> theta 360) t)
		(set-agent-joints ob
			(do 	(
				(cnt idx (1- cnt))
				(tlist (list theta) (cons nil tlist))
				)
				((= cnt 0) tlist)
			)
		)
	)
)


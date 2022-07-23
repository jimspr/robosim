;                             *********
;                             4R_ROBOT.LSP
;                             *********
;      *******
;      File to create a 4-revolute joint robot that will be the
;      actual basketball players.
;      Kevin T. Henegar
;      *******
;
;      *******
;      Base Link
;      *******
;
(defun make-4r_robot nil (make-serial-agent
        (make-fixed-link
                (make-cylinder 75 300) (translate 0 0 -300)
                (make-cylinder 100 125) (translate 0 0 -425)
        )
        (translate 0 0 425)
;
;      *********
;      Link One
;      *********
;
       (make-revolute-link nil nil
               (make-cylinder 75 200) (translate 0 0 -150)
       )
       (translate 0 0 0)
;
;      *********
;      Link Two
;      *********
;
       (make-revolute-link nil nil
               (make-cylinder 75 150) (translate 0 0 -75)
               (make-cylinder 50 100) (matmult (rotatey 90)
                       (translate 0 0 75))
               (make-cylinder 50 150) (translate 225 0 -75)
       )
       (matmult (rotatex 90) (translate 0 125 0))


;
;      ********
;      Link Three
;      ********
;
       (make-revolute-link nil nil
               (make-cylinder 50 200) (matmult (rotatey 90)
                       (translate 0 0 50))
	       (make-cylinder 50 100) (translate 300 0 -50)
       )
       (translate 225 0 0)
;
;      ********
;      Link Four
;      ********
;
       (make-revolute-link nil nil
               (make-cylinder 50 100) (matmult (rotatey 90)
                       (translate 0 0 50))
       )
       (translate 300 0 0)
))


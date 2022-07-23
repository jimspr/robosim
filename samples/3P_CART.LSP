;                        ***********
;                          3p_cart
;                        ***********
;    ********
;    File to create a cartesian 3-prismatic joint robot to move
;    the ball and the players.
;    Kevin T. Henegar
;    ********
;
;    ********
;    Link One
;    ********

(defun make-3p_cart nil (make-serial-agent
         (make-prismatic-link nil nil
                  (make-box 150 150 700) (translate 0 0 0)
         )
         (translate 0 0 0)

;     ********
;     Link Two
;     ********
 
          (make-prismatic-link nil nil
                   (make-box 150 150 700) (translate 0 0 75)
          )
          (matmult (rotatey 90) (translate -775 0 0))

;      ********
;      Link Three
;      ********

          (make-prismatic-link nil nil
                   (make-box 150 150 700) (translate 0 0 75)
          )
          (matmult (rotatex -90)(translate 0 -850 0))
))




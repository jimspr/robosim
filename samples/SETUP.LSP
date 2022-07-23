;                        *********
;                        SETUP.LSP
;                        *********
;
;    **************
;    Setup for demo of two robots playing 
;    one-on-one basketball.
;    Kevin T. Henegar
;    **************
;

(set-look-at 0 0 700)
(set-look-from 0 3500 700)
(set-camera 50 0 100)

;    ********
;    Load 4-r joint robot, UNC
;    ********

(load "4r_robot.lsp")
(setq unc (make-4r_robot))
(set-position unc (translate 1000 0 0))
(set-object unc :color '(0 200 255))
(use-objects unc)

;    ********
;    Load 4-r joint robot, MICH
;    ********

(load "4r_robot.lsp")
(setq mich (make-4r_robot))
(set-position mich (translate -1000 0 0))
(set-object mich :color '(255 255 0))
(use-objects mich)

;    ********
;    Load 3-prismatic link robot, 3p_cart, three times for efficiency
;    ********

(load "3p_cart.lsp")
(setq mover (make-3p_cart))
(set-object mover :color '(0 0 0))
(set-position mover (translate -850 -1500 -1700))
(use-objects mover)

(setq mover2 (make-3p_cart))
(set-object mover2 :color '(0 0 0))
(set-position mover2 (translate -850 -1500 -1700))
(use-objects mover2)

(setq mover3 (make-3p_cart))
(set-object mover3 :color '(0 0 0))
(set-position mover3 (translate -850 -1500 -1700))
(use-objects mover3)


;    ********
;    Create Floor
;    ********

(setq floor1 (make-box 1500 2500 10))
(set-position floor1 (translate 750 0 -10))
(setq floor2 (make-box 1500 2500 10))
(set-position floor2 (translate -750 0 -10))
(use-objects floor1 floor2)
(setq half1 (make-circle 400))
(setq half2 (make-circle 200))
(use-objects half1 half2)

;    ********
;    Create Ball
;    ********

(setq ball (make-sphere 100))
(set-position ball (translate 0 0 100))
(set-object ball :color '(255 0 100))
(use-objects ball) 

;    ********
;    Create Baskets
;    ********


(setq goal1 (make-box 10 600 600))
(set-position goal1 (translate 1500 0 800))
(use-objects goal1)

(setq basket1 (make-truncated-cone 75 150 200))
(set-position basket1 (translate 1350 0 750))
(set-object basket1 :color '(255 0 100))
(use-objects basket1)

(setq support1 (make-box 500 150 150))
(set-position support1 (translate 1750 0 1000))
(set-object support1 :color '(0 0 255))
(use-objects support1)

(setq base1 (make-box 150 150 1150))
(set-position base1 (translate 2075 0 0))
(set-object base1 :color '(0 0 255)) 
(use-objects base1)


(setq goal2 (make-box 10 600 600))
(set-position goal2 (translate -1500 0 800))
(use-objects goal2)

(setq basket2 (make-truncated-cone 75 150 200))
(set-position basket2 (translate -1350 0 750))
(set-object basket2 :color '(255 0 100))
(use-objects basket2)

(setq support2 (make-box 500 150 150))
(set-position support2 (translate -1750 0 1000))
(set-object support2 :color '(0 0 255))
(use-objects support2)

(setq base2 (make-box 150 150 1150))
(set-position base2 (translate -2075 0 0)) 
(set-object base2 :color '(0 0 255))
(use-objects base2)


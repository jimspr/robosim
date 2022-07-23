;                      **************
;                      B_BALLDEMO.LSP
;                      **************
;
;     ***********
;     This file is the demo for the two robots, North Carolina and
;     Michigan, battling for the national championship.
;     ***********
;
;
;     **********
;     Load the court setup
;     **********

(load "setup.lsp")
;
;     **********
;     Jump Ball
;     **********
;

(drive-agent unc '(180 90 -90 90) 1)
(grasp mover unc)
(drive-agent mover '(0 -500 0) 1)
(release mover)
(drive-agent mich '(0 90 -90 90) 1)
(grasp mover mich)
(drive-agent mover '(0 0 0) 1)
(release mover)
(grasp mover ball)
(drive-agent mover '(1500 0 0) 1)
(drive-agent unc '(180 90 0 0) 1)
(drive-agent mover '(850 0 0) 1)
(drive-agent unc '(180 52 0 0) 1)
(release mover)
(drive-agent mover '(0 0 0) 1)
(grasp unc ball)
(drive-agent unc '(0 0 0 0) 1)
(grasp mover mich)
(drive-agent mover '(0 -400 0) 1)
(drive-agent mich '(0 0 -90 90) 1)
(release mover)

;      **********
;      North Carolina begins dribbling
;      **********

(drive-agent mover '(300 0 0) 1)
(drive-agent unc '(180 0 -20 20) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(110 0 0) 1)
(drive-agent mover '(300 0 0) 1)
(release mover)
(grasp unc ball)
(drive-agent unc '(180 0 20 -20) 1)
(drive-agent unc '(180 0 -20 20) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(110 0 0) 1)
(release mover)
(grasp unc ball)
(grasp mover2 unc)
(drive-agent mover2 '(0 -100 200) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(300 0 0) 1)
(release mover)
(grasp unc ball)
(drive-agent unc '(180 0 20 -20) 1)
(drive-agent unc '(180 0 -20 20) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(110 0 0) 1)
(drive-agent mover '(300 0 0) 1)
(release mover)
(grasp unc ball)
(drive-agent unc '(180 0 20 -20) 1)


;    ********
;    Michigan knocks the ball loose
;    ********

(release unc)
(drive-agent mich '(10 0 -20 20) 1)
(drive-agent mich '(20 0 -10 10) 1)
(grasp mover ball)
(drive-agent mover '(-110 -100 200) 1)
(drive-agent mich '(40 -20 0 20) 1)
(grasp mover3 mich)
(drive-agent mover3 '(0 100 100) 1)
(drive-agent unc '(165 -50 0 50) 1)
(drive-agent mich '(40 -40 0 40) 1)
(release mover2)
(drive-agent mover2 '(0 0 0) 1)
(grasp mover2 unc)
(drive-agent mover2 '(0 -150 100) 1)
(release mover)

;     ********
;     North Carolina recovers loose ball
;     ********

(grasp unc ball)
(drive-agent unc '(0 0 0 0) 1)
(drive-agent mover3 '(0 700 700) 1)
(drive-agent unc '(-140 0 -20 20) 1)
(drive-agent unc '(180 0 -20 20) 1)
(drive-agent mover '(350 0 0) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(60 0 0) 1)
(release mover)
(grasp unc ball)
(drive-agent mover2 '(0 -450 0) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(250 100 0) 1)
(release mover)
(grasp unc ball)
(drive-agent unc '(180 0 20 -20) 1)
(drive-agent unc '(180 0 -20 20) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(60 100 0) 1)
(release mover)

;      **********
;      North Carolina moves in for the dunk
;      **********

(grasp unc ball)
(drive-agent mover2 '(0 -800 0) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(250 100 0) 1)
(release mover)
(grasp unc ball)
(drive-agent mich '(-140 0 0 0) 1)
(drive-agent unc '(180 45 0 0) 1)
(drive-agent mover2 '(100 -950 0) 1)
(drive-agent unc '(180 90 20 30) 1)
(drive-agent mover2 '(150 -1150 0) 1)
(drive-agent unc '(180 45 -20 -35) 1)
(release unc)
(drive-agent mover '(0 0 0) 1)
(grasp mover ball)
(drive-agent mover '(-725 0 0) 1)

;      **********
;      Teams set up for next sequence
;      **********

(drive-agent unc '(0 0 0 0) 1)
(drive-agent mover2 '(0 -1150 0) 1)
(drive-agent mover2 '(0 800 0) 5)
(drive-agent mover3 '(0 0 0) 3)
(drive-agent unc '(180 0 0 0) 1)
(drive-agent mich '(0 0 0 0) 1)
(drive-agent mich '(0 0 -20 20) 1)
(drive-agent mover '(-535 1000 -150) 1)
(release mover)
(grasp mich ball)
(drive-agent mich '(0 0 20 -20) 1)
(drive-agent mich '(0 0 -20 20) 1)
(release mich)
(grasp mover ball)
(drive-agent mover '(-725 1000 -150) 1)
(release mover)
(grasp mich ball)
(drive-agent mover3 '(0 300 0) 1)
(release mich)
(grasp mover ball)
(drive-agent mover '(-535 1000 -150) 1)
(release mover)
(grasp mich ball)
(drive-agent mich '(0 0 20 -20) 1)
(drive-agent mich '(0 0 -20 20) 1)
(release mich)
(grasp mover ball)
(drive-agent mover '(-725 1000 -150) 1)
(drive-agent mover2 '(0 600 0) 1)
(drive-agent unc '(180 0 -90 90) 1)
(drive-agent mover '(-535 1000 -150) 1)
(release mover)
(grasp mich ball)
(drive-agent mich '(0 0 20 -20) 1)
(drive-agent mich '(0 0 -20 20) 1)
(release mich)
(grasp mover ball)
(drive-agent mover '(-725 1000 -150) 1)
(release mover)
(grasp mich ball)
(drive-agent mover3 '(0 600 400) 1)
(release mich)
(grasp mover ball)
(drive-agent mover '(-535 1000 -150) 1)
(release mover)
(grasp mich ball)
(drive-agent mich '(0 0 20 -20) 1)
(drive-agent mich '(0 0 -20 20) 1)
(release mich)
(grasp mover ball)
(drive-agent mover '(-725 1000 -150) 1)
(release mover)
(grasp mich ball)
(drive-agent mover3 '(0 900 600) 1)
(release mich)
(grasp mover ball)
(drive-agent mover '(-535 1000 -150) 1)
(release mover)
(grasp mich ball)
(drive-agent mich '(-50 70 70 30) 1)

;      ********
;      North Carolina blocks Michigan shot
;      ********
 
(drive-agent mich '(-20 70 70 30) 1)
(drive-agent mich '(-20 70 30 40) 1)
(release mover2)
(drive-agent mover2 '(0 0 0) 1)
(grasp mover2 unc)
(drive-agent mover2 '(0 -200 200) 1)
(drive-agent unc '(145 45 0 0) 1)
(release mich)
(drive-agent mover '(0 0 0) 1)
(grasp mover ball)
(drive-agent mover '(200 200 -200) 1)
(drive-agent mich '(-20 70 -20 -30) 1)
(drive-agent unc '(145 90 20 10) 1)
(drive-agent mover '(220 600 -300) 1)
(drive-agent mover2 '(400 -200 300) 1)
(drive-agent unc '(145 90 -20 -30) 1)
(drive-agent mover '(-1175 -100 -300) 1)
(drive-agent mover '(-500 -300 -400) 1)

;     *********
;     North Carolina shoots and scores, winning the championship
;     *********

(drive-agent mover2 '(0 -600 0) 1)
(drive-agent mover '(-1175 -500 -500) 1)
(drive-agent unc '(180 0 0 0) 1)
(drive-agent mover2 '(0 -900 -300) 1)
(drive-agent mover '(-775 -700 -675) 1)
(drive-agent unc '(180 0 20 -20) 1)
(drive-agent mover2 '(0 -1050 -300) 1)
(drive-agent mich '(180 0 0 0) 1)
(release mover)
(drive-agent mover '(0 0 0) 1)
(grasp unc ball)
(drive-agent unc '(180 0 -20 20) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(-190 0 0) 1)
(release mover)
(grasp unc ball)
(drive-agent mover2 '(0 -1450 -600) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(0 0 0) 1)
(release mover)
(grasp unc ball)
(drive-agent unc '(135 0 20 -20) 1)
(drive-agent unc '(135 0 -20 20) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(-190 0 0) 1)
(drive-agent mover '(0 0 0) 1)
(release mover)
(grasp unc ball)
(drive-agent unc '(135 90 20 30) 1)
(release unc)
(grasp mover ball)
(drive-agent mover '(100 -200 100) 1)
(drive-agent unc '(135 80 0 -20) 1)
(drive-agent mover '(150 -400 150) 1)
(drive-agent mover '(100 -600 200) 1)
(drive-agent mover '(25 -800 250) 1)
(drive-agent mover '(-100 -875 300) 1)
(drive-agent mover '(-200 -975 350) 1)
(drive-agent mover '(-300 -975 400) 1)
(drive-agent mover '(-1150 -975 400) 1)
(drive-agent unc '(0 90 0 0) 1)
(drive-agent mover2 '(0 0 0) 5)
(drive-agent mover3 '(0 0 0) 3)
(drive-agent mover '(-1150 385 400) 1)


 
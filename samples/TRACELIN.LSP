;	*****
;	TRACELIN.LSP
;	*****

;	Drive commands to move 3r robot to each node along straight-
;	line with joint angles determined from inverse kinematics.
;	This file is used with the files, 3R-DEMO.LSP, and
;	STARTUP.LSP (Technology 2003 Version) for the demo.
	
;	George E. Cook, October 16, 1993.

;	*****

(setq s 10)
(drive-agent 3r '(149 60 -62.6) s)
(drive-agent 3r '(143.7 81.5 -93.6) s)
(drive-agent 3r '(136.3 101 -115.2) s)
(set-look-at 0 840 705)
(set-look-from 0 841 3500)
(drive-agent 3r '(125.5 120.8 -130.3) s)
(drive-agent 3r '(110.1 138.8 -139.3) s)
(drive-agent 3r '(90 148.6 -143.7) s)
(drive-agent 3r '(69 143.7 -145.3) s)
(set-look-at 0 760 585)
(set-look-from 2000 761 585)
(drive-agent 3r '(51.7 124.8 -142.2) s)
(drive-agent 3r '(39.4 101.6 -131.7) s)
(drive-agent 3r '(31 80.5 -113.9) s)
(drive-agent 3r '(25 60.8 -89.5) s)


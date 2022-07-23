;	*****
;	MULTVIEW.LSP
;	*****

;	Multiple views of PUMA560 robot tracking a rectangular
;	base using cartesian motion control.  Other files needed
;	are TRACKING.LSP and STARTUP.LSP (Technology 2003 Version).

;	Prepared for NASA Technology 2003 Conference.

;	George E. Cook, November 27, 1993

;	*****
(clear-simulation)
(new-view)
(set-look-at 0 0 20)
(set-look-from 0 100 20)
(set-camera 100 0 100)
(new-view)
(set-look-at 0 0 20)
(set-look-from 0 1 100)
(set-camera 100 0 100)
(tile-windows-v)
(load "tracking.lsp")
(tracking)
# ntetrex
Ncurses tetris 

Todo:

!- make use of ws consistent
!- move functions for readability

- bug: brick breaks free :D
    - or make it a feature.. extra points?
- fix: interrupt down key

create stage
!- main stage borders
!- counter stage + predictor + level indic

define bricks
!- brick struct
!  - detect col using .. man 3 inch?
!- brick movement
!  - left, right, down
!- brick rotation
!  - add bricks
!  - add current brick to struct
!  - rewrite floor col detection for new bricks
- integrate random brick spawning
- prediction bar
!- create gravity
!float movement increments?
collision detection
! - detect if char tea not ws
!  - detect floor in grav
!  - fix bug: col det on all stones
!  - detect walls in brickMove
line detection
!- detect + remove line
!- expolosion anim
!- add scores
- add dificulty level

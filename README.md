# ntetrex
Ncurses tetris 

Todo:

!create stage
!- main stage borders
!- counter stage + predictor + level indic
define bricks
!- brick struct
- brick placement alg using addch, detect overwrite using .. man 3 inch?
- brick movement
  - left, right, down
  - spacebar = drop down
- brick rotation
  - add bricks
- spawning
- prediction
create gravity
- grav timer? or float movement increments?
collision detection
 - detect if char tea not ws
line detection
- expolosion anim

- set keboard delay repeat to 0?

# *CONNECT 4* - LCOM PROJECT

## Connect 4 - Specification

### Modules:

##### Program:
- State-Machine: The program events are handled according to the current state and the device's interrupt that occurred.  
  - State is on of the following:
    - MAIN_MENU: the program displays the main menu
    - PAUSE_MENU: the program displays the pause menu
    - RULES_MENU: the program displays the rules menu
    - IN_GAME: the players are playing a game
    - ANIMATION_GAME: animation of the game is occurring
    - END_GAME: the program displays the end of the game menu
    - QUIT: the program ends

##### Timer:
- Frame Rate: 60 FPS
- Used to implement animation of the game pieces

##### Graphics:
- Video mode: 0x14C
  - Resolution: 1152x864
  - Colors with 32bits (8:)8:8:8

- Implementation of Triple-Buffering with page flipping

##### RTC:
- Used to display the current date and the current time in every menu

##### Mouse and Keyboard:
- Handle the interrupts from the keyboard actions and mouse movements or clicks

##### Sprites:
- Struct sprite that saves the following information:
  - (uint8_t *) map of the sprite that is loaded using the xpm_load()
  - (xpm_image_t) image of the sprite that stores the information about an image.
  - position (x,y) of the up left corner to display the sprite

##### Menus:
- Each menu as differents sprites:
  - Background sprite
  - Mouse pointer sprite
  - Sprite for each button of the menu
  - Ask the rtc for the date and draw each number or symbol

##### Game:
- Struct game that saves the following information:
  - (uint8_t **) board that saves the state of the board during the game
  - turn corresponding to the current to make a move
  - column where the piece is drawn before making a move
  - (char *) display buffer, an auxiliary video buffer where the board with all the pieces are saved and 'drawn' to this buffer to optimize the drawing of the board by simple copying from this buffer to video memory

##### Animation:
- Struct animation that saves the following information:
  - sprite to animate
  - start x-axis coordenate of the display
  - start y-axis coordenate of the display
  - final y-axis coordenate of the display  

Note: final x-axis coordenate is not needed due to the fact that the animation of the pieces falling change only the y-axis coordenate

#### LCOM Project for group t11g01
##### Group Members:

Fernando Rego - **up201905951@edu.fe.up.pt**  
Gonçalo Ferreira - **up202004761@edu.fe.up.pt**  
João Duarte - **up201707984@edu.fc.up.pt**  
Pedro Macedo - **up202007531@edu.fe.up.pt**  




# *CONNECT 4* - LCOM PROJECT


## Project Proposal

Our idea to this project is to implement the traditional connect 4 game, a two-player board game in which each player is associated with a color, and then, in alternate turns, the player drops a piece in a column of the board.
The winner is the first player to form a line with 4 contiguous pieces of the respective color.
If the board fills up, the game ends with a draw.

#### Functionalities:

The game will have a simple main menu with the following options:
- Start playing the game
- Go to the rules menu
- Quit the game

The rules menu will present the basic rules of the game as well as the game controls
The game itself will have a simple animation to give the idea that the pieces played are falling on the corresponding column of the board and the players will have the option to pause the game.
The pause menu will have the option to go back to the main menu or to continue the current game.

#### Devices:

| Device | Role | Interrupts |
| :--: | ---- | :--: |
| Timer    | Control the game's frame rate as well as the movement of the pieces' animations | Yes |
| Keyboard | Select the column of each move of the game and some menu controls like ESC to pause the game | Yes |
| Mouse    | Interaction in each menu of the program to choose an option | Yes |
| Video    | Display all the menus and the game itself | No |
| RTC      | Obtains the real date to display in each menu | Yes |

#### Milestones:

| Week | Milestone |
| :--: | ---- |
| 09/05 - 15/05 | Display main menu and control mouse movement and clicks. Handle the events triggered by a click on each button |
| 16/05 - 22/05 | Addition of the rules menu and implementation of the game logic |
| 23/05 - 15/05 | Display the game and implement the pieces's animation |
| 30/05 - 05/06 | Addition of the RTC device to the project and display the current date in each menu |
| 06/06 - 09/06 | Final adjustments of the project |

#### LCOM Project for group t11g01
##### Group Members:

Fernando Rego - up201905951@edu.fe.up.pt  
Gonçalo Ferreira - up202004761@edu.fe.up.pt  
João Duarte - up201707984@edu.fc.up.pt  
Pedro Macedo - up202007531@edu.fe.up.pt  

# chaser-game

## Description
The Chaser game's main idea is that there is an object which is stuck in some place with some other objects.
Some are chasing ,others are to be chased. The game is built using openGL.


## Gameplay
The game is mainly about an object that the player controls.
This object is being chased by a group objects that decrements the players score when they manage to catch him (or kill him in another game mode).
Another group of objects are escaping and the player should try to catch them to increment their score.
Objects motion is divided into two points:
- Objects change their direction of looking as the player always looks the way it
goes, chasers always look towards the player, and chased always look towards
the opposite direction of the player.
- The player moves based on the player control, chasers always move towards the
player, chased always move away.

After 30 seconds of the game start, the player will be granted the ability of performing one of two special power moves, once in the whole game. Special power moves are:
- Double score: for the upcoming 30 seconds, all score acquired (positive or negative) is multiplied by two (using the letter q).
- Chasers stun: for the upcoming 30 seconds, all chasers stop moving (using the letter w).


## Game Modes
The game has two modes:
- Time based: the game lasts for 5 minutes.
- One shot kill: the game lasts till the player gets caught by a chaser.


## Controls
- The player motion control with the mouse.
- Exiting the game with the ESC key or 'z' key.
- Activation of special power moves using 'q' and 'w' keys.
- Switch game mode using 'm' key.

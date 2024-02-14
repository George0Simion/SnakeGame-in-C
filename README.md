			SNAKE GAME
				
	This project is a simple implementation of the classic snake game using c and the ncurses library. the game is played in a terminal window and features a snake that the player controls to eat berries, which increases the snake's length. the game ends if the snake collides with itself or the wall, and features score tracking. the goal is to grow the longest snake possible without running into the walls or the snake's own body.
	
			Features

  -  simple snake control with arrow keys
  -  score tracking displayed at the top of the screen
  -  dynamic snake growth with each berry consumed
  -  self-collision detection
  -  berry placement avoiding the snake's current position
  -  wall wrapping allowing the snake to move from one edge of the screen to the opposite edge
  -  colorful display using ncurses color pairs
  
  			Requirements
  			
  	To run this game, you need a system with ncurses installed. this can typically be done through your system's package manager.  	
  -  for debian-based systems: sudo apt-get install libncurses5-dev libncursesw5-dev
  -  for red hat-based systems: sudo yum install ncurses-devel

			Compilation
			
	Compile the game using gcc or any compatible c compiler, ensuring to link against the ncurses library. for example: 
gcc -o snake snake.c -lncurses

			Running the game
		
	After compilation, you can start the game by running: ./snake . Use the arrow keys to control the direction of the snake. eat berries to increase your score and avoid running into the walls or the snake itself.
	
			Exiting the game
			
	You can exit the game anytime by pressing the escape key.

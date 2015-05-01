### Algorithm - Flood Fill
----------
The algorithm, as it is right now, only works if you manualy insert the size of the maze, 
the starting cell, and finish cell in the source code. It assigns the finish cell a value 
of zero (0), it then assigns every cell around the zero (0) cell (every cell touching the 
zero cell) a value of one (1). All the cells around the ones (1s) get a value of two (2).
This process continues until the entire maze is filled with the incremental values. Now
the Nanomouse begins from the starting cell and follows the path with the smallest cell
values to eventually reach the finish cell (the zero cell).


Current Algorithm Status:

	- The mouse currently seems to be working as it should.
	I'm currently using wireless debugging on my Android phone to enhance the code.

Recommended IDE: 

	- Arduino

	- Official Website:
		http://arduino.cc/en/main/software
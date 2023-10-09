// Updated for Esplora 2013 TeamRursch185
// Updated for DualShock 4 2016 Rursch


// Headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses/ncurses.h>
#include <unistd.h>

// Mathematical constants
#define PI 3.14159

// Screen geometry
// Use ROWS and COLS for the screen height and width (set by system)
// MAXIMUMS
#define NUMCOLS 100
#define NUMROWS 72

// Character definitions taken from the ASCII table
#define AVATAR 'A'
#define WALL '*'
#define EMPTY_SPACE ' '


// 2D character array which the maze is mapped into
char MAZE[NUMROWS][NUMCOLS];


// POST: Generates a random maze structure into MAZE[][]
//You will want to use the rand() function and maybe use the output %100.  
//You will have to use the argument to the command line to determine how 
//difficult the maze is (how many maze characters are on the screen).
void generate_maze(int difficulty);

// PRE: MAZE[][] has been initialized by generate_maze()
// POST: Draws the maze to the screen 
void draw_maze(void);

// PRE: 0 < x < COLS, 0 < y < ROWS, 0 < use < 255
// POST: Draws character use to the screen and position x,y
void draw_character(int x, int y, char use);

// PRE: -1.0 < x_mag < 1.0
// POST: Returns tilt magnitude scaled to -1.0 -> 1.0
// You may want to reuse the roll function written in previous labs.  
float calc_roll(float x_mag);


// Main - Run with './ds4rd.exe -t -g -b' piped into STDIN
int main(int argc, char* argv[])
{
	if (argc <2) { printf("You forgot the difficulty\n"); return 1;}
	int difficulty = atoi(argv[1]); // get difficulty from first command line arg
	int t, b1, b2, b3, b4;
	double gx, gy, gz;
	int x = 50;
	int y = 0;
	double roll;
	//char lastchar;
	int count = 0;
	// setup screen    
	initscr();
	refresh();
	

	// Generate and draw the maze, with initial avatar
	generate_maze(difficulty);
	draw_maze();
	//while (1);

	// Read gyroscope data to get ready for using moving averages.
	
	scanf("%d, %lf, %lf, %lf, %d, %d, %d, %d", &t, &gx, &gy, &gz, &b1, &b2, &b3, &b4);
	
	// Event loop
	do
	{

		// Read data, update average
		scanf("%d, %lf, %lf, %lf, %d, %d, %d, %d", &t, &gx, &gy, &gz, &b1, &b2, &b3, &b4);
		roll = calc_roll(gx);
		// Is it time to move?  if so, then move avatar
		if ((t - count) > 400) {
			draw_character(x, y, AVATAR);
			draw_character(x, y - 1, MAZE[y - 1][x]);
			//left move

			if (MAZE[y + 1][x] == EMPTY_SPACE) {
				y = y + 1;
				//draw_character(x,y, AVATAR);
			}
			if (roll > 0.25 && x > 0 && MAZE[y][x - 1] == EMPTY_SPACE) {
				x = x - 1;
				draw_character(x, y, AVATAR);
				draw_character(x + 1, y, MAZE[y][x + 1]);
			}
			//right
			if (roll < -0.25 && x < 100 && MAZE[y][x + 1] == EMPTY_SPACE) {
				x = x + 1;
				draw_character(x, y, AVATAR);
				draw_character(x - 1, y, MAZE[y][x - 1]);
			}
			
			

			count = t;
		}
		//game over


//	} while (1);
	} while (y < (NUMROWS-1)); // Change this to end game at right time 

	// Print the win message
	endwin();
	
	printf("YOU WIN!\n"); 
	return 0;
	} 



// PRE: 0 < x < COLS, 0 < y < ROWS, 0 < use < 255
// POST: Draws character use to the screen and position x,y
//THIS CODE FUNCTIONS FOR PLACING THE AVATAR AS PROVIDED.
//
//    >>>>DO NOT CHANGE THIS FUNCTION.<<<<
void draw_character(int x, int y, char use)
{
	mvaddch(y,x,use);    
	refresh();
}

void generate_maze(int difficulty) {
	int i;
	int j;
	for (i = 0; i < NUMCOLS; i++) {
		for (j = 0; j < NUMROWS; j++) {
			if (rand() % 100 < difficulty) {
				MAZE[j][i] = WALL;
			}
			else {
				MAZE[j][i] = EMPTY_SPACE;
			}
		}
	}
}

void draw_maze() {
	int i;
	int j;
	for (i = 0; i < NUMCOLS; i++) {
		for (j = 0; j < NUMROWS; j++) {
			draw_character(j, i, MAZE[j][i]);
		}
	}
}

float calc_roll(float x_mag) {
	if (x_mag < -1) {
		x_mag = -1;
	}
	else if (x_mag > 1) {
		x_mag = 1;
	}
	return x_mag;
}







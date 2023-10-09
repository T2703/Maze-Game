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
	// setup screen    
	initscr();
	refresh();

	// Generate and draw the maze, with initial avatar
	generate_maze(difficulty);
	draw_maze();
	int avatar_x = 50;
	int avatar_y = 0;
	int counter = 0; 
	int counter_lose = 0; 
	double roll;
	int t, b1, b2, b3, b4;
	double gx, gy, gz;
	draw_character(avatar_x, avatar_y, AVATAR); //this spawns the avatar in 

	// Event loop
	do
	{	
		
		// Read data, update average
		scanf("%d, %lf, %lf, %lf, %d, %d, %d, %d", &t, &gx, &gy, &gz, &b1, &b2, &b3, &b4); 
		roll = calc_roll(gx); //calculating the roll of the gyro x
		counter += 1; //the counter is here to delay the movement of the avatar so it doesn't move fast
		draw_character(avatar_x, avatar_y, AVATAR); //spawning of the avatar for the loop
		
		//if(counter == 9000) {
			//draw_character(avatar_x, avatar_y, EMPTY_SPACE);
			//avatar_y += 1;
			//counter = 0; 
		// Just some useful stuff that I used 
		
		//}
		// Is it time to move?  if so, then move avatar
		if(counter == 10){
			if(MAZE[avatar_y + 1][avatar_x] == EMPTY_SPACE) { //move down
				draw_character(avatar_x, avatar_y, EMPTY_SPACE); // this empty space removes the preivous avatar
				avatar_y = avatar_y + 1;
				counter_lose = 0;
			}
				
			if(roll > 1 && avatar_x > 0 && MAZE[avatar_y][avatar_x - 1] == EMPTY_SPACE) { //move left
			avatar_x = avatar_x - 1;
			draw_character(avatar_x, avatar_y, AVATAR);
			draw_character(avatar_x + 1, avatar_y, EMPTY_SPACE);
			}
			
			if(roll < -1 && avatar_x < 100 && MAZE[avatar_y][avatar_x + 1] == EMPTY_SPACE) { //move right
			avatar_x = avatar_x + 1;
			draw_character(avatar_x, avatar_y, AVATAR);
			draw_character(avatar_x - 1, avatar_y, EMPTY_SPACE);
			}
			
			if(MAZE[avatar_y][avatar_x + 1] != EMPTY_SPACE && MAZE[avatar_y + 1][avatar_x] != EMPTY_SPACE) { //detects if avatar/player gets stuck
				counter_lose += 1; //the lose counter 
				if(counter_lose == 10) {
					break;
				}
			}
			counter = 0;
			
		}

	} while(avatar_y != 71); //end game at right time 

	// Print the win message
	endwin();
	
	if(avatar_y == 71){ //so this happens when the avatar reaches the position to win
		printf("YOU WIN!\n");
	}
	
	else { //it it is stuck then print the lose 
		printf("YOU LOSE!\n");
	}
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
	for(int i = 0; i < NUMCOLS; i++) {
		for(int j = 0; j < NUMROWS; j++) {
			if(rand() % 100 < difficulty) {
				MAZE[j][i] = WALL;
			}
			else {
				MAZE[j][i] = EMPTY_SPACE;
			}
			
		}
		
	}

	
}

void draw_maze(void) {
	for(int i = 0; i < NUMCOLS; i++) {
		for(int j = 0; j < NUMROWS; j++) {
			draw_character(i, j, MAZE[j][i]);
		}

	}

}

float calc_roll(float x_mag){

	if (x_mag > 1.0) {
		x_mag = 1;
	}
	else if (x_mag < -1.0) {
		x_mag = -1;
	}
	x_mag = asin(x_mag);
	return x_mag;
}
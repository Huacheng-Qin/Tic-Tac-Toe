//October 23, 2018
//CompSci 40S - Huacheng Qin
//Complex toe - A spin off game based on tic tac toe, except the user could
//				increase the board size and the number of boxes a player
//				needs to connect in order to win.

//==============================================================================================

#include <iostream>
#include <curses.h>
#include <curses.h>
#include <time.h>

//==============================================================================================

#define TEXT_WHITE COLOR_PAIR(7)
#define TEXT_RED COLOR_PAIR(1)
#define TEXT_GREEN COLOR_PAIR(2)
#define TEXT_YELLOW COLOR_PAIR(3)
#define TEXT_CYAN COLOR_PAIR(4)
#define TEXT_ORANGE COLOR_PAIR(5)
#define TEXT_GRAY COLOR_PAIR(6)

//Declare constants
const int SIZEb = 20;						//Size of board
const int NUMw = 6;							//Number of connected spots needed to win
enum boxstat{O, X, blank};						//Types characters allowed in a box
enum winstat{undef, win, loss, tie};		//Keeps track of the winner
enum arrow{up, down, left, right, enter};		//Types of input
enum wintype{horiz, verti, diagD, diagU, NONE};	//How the win streak is positioned, used for print()

//==============================================================================================

//Function prototypes
void seedran();										//Seeds the random number generator
void clearaS();										//Clears the screen
void clearB(boxstat board[SIZEb][SIZEb]);				//Clears the board
void menu(int &brdsize, int &winsize);						//Prints menu
void print(boxstat board[SIZEb][SIZEb], int pointer[2],			//Prints board
		bool turnp1, int winpoint[2], wintype &Wtype, int brdsize, int winsize);
void game(boxstat board[SIZEb][SIZEb], int pointer[2],			//Plays 1 round
		bool turnp1, int brdsize, int winsize);	
bool loopcount(int &x, int &y, int max);			//Moves loop counter based on input
arrow input();												//Allows user input
winstat check(boxstat board[SIZEb][SIZEb], int winpoint[2],		//Checks if there is a winner
	wintype &Wtype, int brdsize, int winsize);
		
//==============================================================================================

//Main program
int main(void)
{
	boxstat board[SIZEb][SIZEb];			//A matrix containing input data
	winstat player1 = undef;			//Whether player 1 wins, loses or ties
	char choice;						//User choice
	int pointer[] = {0,0};				//Pointer with x and y value for accessing matrices
	bool turnp1;						//Whether or not it's player1's turn
	int default_winpoint[] = { -1, -1 };	//Default position for winpoint
	wintype default_wintype = NONE;			//Default type for wintype
	int brdsize;						//Size of board
	int winsize;						//Amount of connections needed to win

	//Seed random number generator
	seedran();

    initscr();
	intrflush(stdscr, TRUE);
	keypad(stdscr, TRUE);
    cbreak();
    noecho();
	if (has_colors() == FALSE) {
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	// turn magenta into orange
	init_color(COLOR_MAGENTA, 1000, 580, 86);
	// turn blue into gray
	init_color(COLOR_BLUE, 463, 463, 463);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_BLUE, COLOR_BLACK);

	//Loop until user quits
	do
	{	//Initialize
		pointer[0] = 0;
		pointer[1] = 0;
		turnp1 = true;
		clearB(board);
		menu(brdsize, winsize);
		clear();
		// system("cls");
		print(board, pointer, turnp1, default_winpoint, default_wintype, brdsize, winsize);
		
		//Play game
		game(board, pointer, turnp1, brdsize, winsize);
		
		//Allow the user to play again
		printw("Press Y to play again, press Q to quit.");
		choice = toupper(getch());
		printw("\n");
		//Validate
		while (choice != 'Y' && choice != 'Q')
		{
			choice = toupper(getch());
			printw("\n");
		}//Validate
	} while (choice != 'Q');
	
	//Reset text colour
    endwin();
	return 0;
}//main(void)

//==============================================================================================

//seedran - This function seeds the random number generator
void seedran()
{
	int i;                  //Loop counter

	//Seed the random number generator use the timer
	srand((unsigned)time(NULL));
	for (i = 0; i < rand(); i++)
		rand();
}//seedran()

//==============================================================================================

//clearS - This function clears the screen
void clearS()
{	
	//Loop and print lines until screen is cleared
	for (int x = 0; x < 60; x++)
		printw("\n");
}//clearS()

//==============================================================================================

//clearB - This function clears the board to start a new game
//board - A matrix containing board stats
void clearB(boxstat board[SIZEb][SIZEb])
{
	//Loop through rows
	for (int x = 0; x < SIZEb; x++)
	{	//Loop through columns
		for (int y = 0; y < SIZEb; y++)
		{
			//Set box status to blank
			board[x][y] = blank;
		}//columns
	}//rows
}//clearB()

//==============================================================================================

//menu - This function prints a menu and allows users to choose some settings
//brbsize - Board size
//winsize- Amount of connections needed to win
void menu(int &brdsize, int &winsize)
{
	int DNE = 0;			//Used to call a function, has no real use

	//Loop for board size
	for (int x = SIZEb - 3; x < SIZEb - 2;)
	{	//Clear screen
		clear();
		// system("cls");
		//Print menu
		attron(TEXT_WHITE);
		printw("Choose board size:\n");
		attron(TEXT_YELLOW);
		printw(" > %d\n", SIZEb - x);
		//if the user presses enter
		if (loopcount(x, DNE, SIZEb - 2))
		{
			brdsize = SIZEb - x; break;
		}//if enter
	}//for - board size

	//Loop for win size
	for (int x = -1; x > -2;)
	{
		//Clear screen
		clear();
		// system("cls");
		//Print menu
		attron(TEXT_WHITE);
		printw("How many must be in a row to win?\n");
		attron(TEXT_YELLOW);
		//if the user presses enter
		if (brdsize < 5)
		{
			winsize = brdsize; break;
		}//if board size is small
		else
		{
			if (brdsize < NUMw)
			{	//First time setup
				if (x == -1)
				{
					x = brdsize - 4;
				}//if first time
				printw(" > %d\n", brdsize - x);
				//if the user presses enter
				if (loopcount(x, DNE, brdsize - 3))
				{
					winsize = brdsize - x;
					break;
				}//if enters
			}//if board size is medium
			else
			{	//If board size is not extra large
				if (brdsize < 16)
				{	//first time loop setup
					if (x == -1)
					{
						x = NUMw - 4;
					}//if first time
					printw(" > %d\n", NUMw - x);
					//if the user presses enter
					if (loopcount(x, DNE, NUMw - 3))
					{
						winsize = NUMw - x;
						break;
					}//if enters
				}//if board size is large
				else
				{	//first time loop setup
					if (x == -1)
					{
						x = NUMw - 5;
					}//if first time
					printw(" > %d\n", NUMw - x);
					//if the user presses enter
					if (loopcount(x, DNE, NUMw - 4))
					{
						winsize = NUMw - x;
						break;;
					}//if enters
				}//else board size not large
			}//else board size not medium
		}//else board size not small
	}//for - win size
}//menu()

//==============================================================================================

//print - This function prints the board
//board - A matrix containing board stats
//pointer - Used for inputs, contains x and y values used to access matrices
//turnp1 - Whether or not it's player1's turn
//winpoint - An array containing the x and y values of the win position
//Wtype - Which direction the winstreak is, used for print()
//brbsize - Board size
//winsize- Amount of connections needed to win
void print(boxstat board[SIZEb][SIZEb], int pointer[2], bool turnp1, int winpoint[2],
		wintype &Wtype, int brdsize, int winsize)
{
	//Print board
	attron(TEXT_WHITE);
	for (int i = 0; i <= brdsize * 4; i++)
	{
		printw("-");
	}//print box outline
	printw("\n");
	//Loop through rows
	for (int x = 0; x < brdsize; x++)
	{	//Print box outline
		attron(TEXT_WHITE);
		printw("| ");
		//Loop through columns
		for (int y = 0; y < brdsize; y++)
		{	//Set colour
			if (board[x][y] == X)
			{
				attron(TEXT_CYAN);
			}//if X
			else
			{
				attron(TEXT_ORANGE);
			}//if O
			//Set colour - cursor position
			if (x == pointer[0] && y == pointer[1])
			{	//if blank, show user input
				if (board[x][y] == blank)
				{	//Show potential box content
					attron(TEXT_GRAY);
					if (turnp1)
					{
						printw("X");
					}//if X
					else
					{
						printw("O");
					}//else O
				}//if board is blank
			}//if cursor position
			//Set colour - cursor position but box is filled
			if (x == pointer[0] && y == pointer[1])
			{
				attron(TEXT_RED);
			}//if cursor
			//Set colour - win position
			if (winpoint[0] != -1 && winpoint[1] != -1)
			{
				switch (Wtype)
				{
				case horiz:
					for (int i = 0; i < winsize; i++)
					{
						if (x == winpoint[0] && y == winpoint[1] + i)
						{
							attron(TEXT_GREEN);
						}
					}//for (win)
					break;
				case verti:
					for (int i = 0; i < winsize; i++)
					{
						if (x == winpoint[0] + i && y == winpoint[1])
						{
							attron(TEXT_GREEN);
						}
					}//for (win)
					break;
				case diagD:
					for (int i = 0; i < winsize; i++)
					{
						if (x == winpoint[0] + i && y == winpoint[1] + i)
						{
							attron(TEXT_GREEN);
						}
					}//for (win)
					break;
				case diagU:
					for (int i = 0; i < winsize; i++)
					{
						if (x == winpoint[0] - i && y == winpoint[1] + i)
						{
							attron(TEXT_GREEN);
						}
					}//for (win)
					break;
				}//switch(Wtype)
			}//if win point exists

			//Print based on box status
			switch (board[x][y])
			{
			case O:
				printw("O");
				break;
			case X:
				printw("X");
				break;
			case blank:
				if (!(x == pointer[0] && y == pointer[1]))
					printw(" ");
				break;
			}//switch(board)
			//Print box outline
			attron(TEXT_WHITE);
			printw(" | ");
		}//columns
		printw("\n");
		//Print box outline
		attron(TEXT_WHITE);
		for (int i = 0; i <= brdsize * 4; i++)
		{
			printw("-");
		}//print spaces
		printw("\n");
	}//rows
}//print()

//==============================================================================================

//game - This function plays a single game to the end
//board - A matrix containing board stats
//pointer - Used for inputs, contains x and y values used to access matrices
//turnp1 - Whether or not it's player1's turn
//brbsize - Board size
//winsize- Amount of connections needed to win
void game(boxstat board[SIZEb][SIZEb], int pointer[2], bool turnp1, int brdsize, int winsize)
{
	bool Eout = false;				//Print a line if input is invalid
	int winpoint[] = {-1, -1};		//Pointer for the position where the win streak is
	wintype Wtype = NONE;			//How the win streak is positioned, used for print()

	//Loop for inputs
	for (int N = 1; N > 0; N++)
	{	//Determine input
		if (loopcount(pointer[0],pointer[1], brdsize))
		{	//Check if box is empty
			if (board[pointer[0]][pointer[1]] == blank)
			{	//Check whose turn it is
				if (turnp1)
				{
					board[pointer[0]][pointer[1]] = X;
				}//if
				else
				{
					board[pointer[0]][pointer[1]] = O;
				}//else
				turnp1 = !turnp1;
			}//if box is empty
			else
			{
				Eout = true;
			}//If box already filled
		}//if user pressed enter
		switch (check(board, winpoint, Wtype, brdsize ,winsize))
		{
		case win: 
			clear();
			// system("cls");
			print(board, pointer, turnp1, winpoint, Wtype, brdsize, winsize);
			printw("Player 1 has won!\n");
			return;
		case loss: 
			clear();
			// system("cls");
			print(board, pointer, turnp1, winpoint, Wtype, brdsize, winsize);
			printw("Player 2 has won!\n");
			return;
		case tie: 
			clear();
			// system("cls");
			print(board, pointer, turnp1, winpoint, Wtype, brdsize, winsize);
			printw("It was a tie!\n");
			return;
		case undef: break;
		printw("ERROR: CHECK() RETURNS UNDEFINED VALUE.\n");
		}//switch(check())

		clear();
		// system("cls");
		print(board, pointer, turnp1, winpoint, Wtype, brdsize, winsize);
		//Print if user tries to replace an existing box
		if (Eout)
		{
			printw("There's already something there!\n");
			Eout = false;
		}//if the user tries to replace a box
	}//for
}//game()

//==============================================================================================

//loopcount - This function changes loop counters based on input
//x - Loop counter (rows)
//y - Loop counter (columns)
//max - Maximum limit for counters
bool loopcount(int &x, int &y, int max)
{
	//Call input()
	switch (input())
	{
	case up:
		if (x > 0)
		{
			x--;
		}//if
		break;
	case down:
		if (x < max - 1)
		{
			x++;
		}//if
		break;
	case left:
		if (y > 0)
		{
			y--;
		}//if
		break;
	case right:
		if (y < max - 1)
		{
			y++;
		}//if
		break;
	case enter: return true;
	}//switch(input())
	return false;
}//loopcount()

//==============================================================================================

//input - This function allows the user to use arrow keys to input data
arrow input()
{
	int key;			//The key that the user presses

	//Loop for input
	while (true)
	{	//Check for function key
		key = getch();
		switch (key)
		{
		case 'w':
		case 'W':
		case KEY_UP: return up;
		case 's':
		case 'S':
		case KEY_DOWN: return down;
		case 'a':
		case 'A':
		case KEY_LEFT: return left;
		case 'd':
		case 'D':
		case KEY_RIGHT: return right;
		case ' ':
		case '\n':
		case KEY_ENTER: return enter;
		case 'Q':
		case 'q': 
		endwin();
		exit(-1);
		}//switch(key)
	}//input loop
}//input()

//==============================================================================================

//check - This function checks to see if the win condition is met
//board - A matrix containing board stats
//winpoint - An array containing the x and y values of the win position
//Wtype - Which direction the winstreak is, used for print()
//winsize- Amount of connections needed to win
winstat check(boxstat board[SIZEb][SIZEb], int winpoint[2], wintype &Wtype, int brdsize, int winsize)
{
	boxstat checktype;			//Which type to check the win condition for
	int count;				//Check counter
	bool tiebreak = false;	//Checks whether or not it is a tie

	//Loop through rows
	for (int x = 0; x < brdsize; x++)
	{	//Loop through columns
		for (int y = 0; y < brdsize; y++)
		{	//Check if the box is blank
			if (board[x][y] != blank)
			{	
				//Set checktype
				checktype = board[x][y];
				count = 1;
				
				//Check horizontal connections
				if (y <= brdsize - winsize)
				{	//Loop to check for connections
					for (int i = 1; i < winsize; i++)
					{	//Check if connected
						if (board[x][y + i] == checktype)
						{
							count++;
						}//if connected
						else
						{
							break;
						}//else not connected
					}//for (checkloop)
					//Check if there is a winner
					if (count == winsize)
					{	//Set winstats
						winpoint[0] = x;
						winpoint[1] = y;
						Wtype = horiz;
						//Check which player won
						if (checktype == X)
						{
							return win;
						}//if player 1 won
						else
						{
							return loss;
						}//else player 1 lost
					}//if won
					//Reset check counter
					count = 1;
				}//if there is enough room to win
				
				//Check vertical connections
				if (x <= brdsize - winsize)
				{	//Loop to check for connections
					for (int i = 1; i < winsize; i++)
					{	//Check if connected
						if (board[x + i][y] == checktype)
						{
							count++;
						}//if not connected
					}//for (checkloop)
					//Check if there is a winner
					if (count == winsize)
					{	//Set winstats
						winpoint[0] = x;
						winpoint[1] = y;
						Wtype = verti;
						//Check which player won
						if (checktype == X)
						{
							return win;
						}//if player 1 won
						else
						{
							return loss;
						}//else player 1 lost
					}//if won
					//Reset check counter
					count = 1;
				}//if there is enough room to win

				//Check diagnal down
				if (x <= brdsize - winsize && y <= brdsize - winsize)
				{	//Loop to check for connections
					for (int i = 1; i < winsize; i++)
					{	//Check if connected
						if (board[x + i][y + i] == checktype)
						{
							count++;
						}//if not connected
					}//for (checkloop)
					//Check if there is a winner
					if (count == winsize)
					{	//Set winstats
						winpoint[0] = x;
						winpoint[1] = y;
						Wtype = diagD;
						//Check which player won
						if (checktype == X)
						{
							return win;
						}//if player 1 won
						else
						{
							return loss;
						}//else player 1 lost
					}//if won
					//Reset check counter
					count = 1;
				}//if there is enough room to win

				//Check diagnal up
				if (x >= winsize - 1 && y <= brdsize - winsize)
				{	//Loop to check for connections
					for (int i = 1; i < winsize; i++)
					{	//Check if connected
						if (board[x - i][y + i] == checktype)
						{
							count++;
						}//if not connected
					}//for (checkloop)
					//Check if there is a winner
					if (count == winsize)
					{	//Set winstats
						winpoint[0] = x;
						winpoint[1] = y;
						Wtype = diagU;
						//Check which player won
						if (checktype == X)
						{
							return win;
						}//if player 1 won
						else
						{
							return loss;
						}//else player 1 lost
					}//if won
					//Reset check counter
					count = 1;
				}//if there is enough room to win
			}//if box not blank
			else
			{
				tiebreak = true;
			}//else box is blank
		}//columns
	}//rows

	//Check if tied
	if (tiebreak)
	{
		return undef;
	}//if there are still blank spaces
	else
	{
		return tie;
	}//else tie
}//check()

//End of file
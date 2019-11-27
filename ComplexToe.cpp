//October 23, 2018
//CompSci 40S - Huacheng Qin
//Complex toe - A spin off game based on tic tac toe, except the user could
//				increase the board size and the number of boxes a player
//				needs to connect in order to win.

//==============================================================================================

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include "colours.h"
using namespace std;

//==============================================================================================

//Declare constants
const int SIZEb = 25;						//Size of board
const int NUMw = 6;							//Number of connected spots needed to win
enum box{O, X, blank};						//Types characters allowed in a box
enum winstat{undef, win, loss, tie};		//Keeps track of the winner
enum arrow{up, down, lef, righ, enter};		//Types of input
enum wintype{horiz, verti, diagD, diagU, NONE};	//How the win streak is positioned, used for print()

//==============================================================================================

//Function prototypes
void seedran();										//Seeds the random number generator
void clearS();										//Clears the screen
void clearB(box board[SIZEb][SIZEb]);				//Clears the board
void menu(int &brdsize, int &winsize);						//Prints menu
void print(box board[SIZEb][SIZEb], int pointer[2],			//Prints board
		bool turnp1, int winpoint[2], wintype &Wtype, int brdsize, int winsize);
void game(box board[SIZEb][SIZEb], int pointer[2],			//Plays 1 round
		bool turnp1, int brdsize, int winsize);	
bool loopcount(int &x, int &y, int max);			//Moves loop counter based on input
arrow input();												//Allows user input
winstat check(box board[SIZEb][SIZEb], int winpoint[2],		//Checks if there is a winner
	wintype &Wtype, int brdsize, int winsize);
		
//==============================================================================================

//Main program
int main(void)
{
	box board[SIZEb][SIZEb];			//A matrix containing input data
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
	//Make orange a colour
	change_colour(f_magenta, 255, 165, 0);

	//Loop until user quits
	do
	{	//Initialize
		pointer[0] = 0;
		pointer[1] = 0;
		turnp1 = true;
		clearB(board);
		menu(brdsize, winsize);
		//clearS();
		system("cls");
		print(board, pointer, turnp1, default_winpoint, default_wintype, brdsize, winsize);
		
		//Play game
		game(board, pointer, turnp1, brdsize, winsize);
		
		//Allow the user to play again
		cout << "Press Y to play again, press Q to quit. ";
		choice = toupper(_getch());
		cout << endl;
		//Validate
		while (choice != 'Y' && choice != 'Q')
		{
			choice = toupper(_getch());
			cout << endl;
		}//Validate
	} while (choice != 'Q');
	
	//Reset text colour
	text_colour(f_gray, b_black);
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
		cout << endl;
}//clearS()

//==============================================================================================

//clearB - This function clears the board to start a new game
//board - A matrix containing board stats
void clearB(box board[SIZEb][SIZEb])
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
		//clear();
		system("cls");
		//Print menu
		text_colour(f_white, b_black);
		cout << "Choose board size:" << endl;
		text_colour(f_yellow, b_black);
		cout << " > " << SIZEb - x;
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
		//clear();
		system("cls");
		//Print menu
		text_colour(f_white, b_black);
		cout << "How many must be in a row to win?" << endl;
		text_colour(f_yellow, b_black);
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
				cout << " > " << brdsize - x;
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
					cout << " > " << NUMw - x;
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
					cout << " > " << NUMw - x;
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
void print(box board[SIZEb][SIZEb], int pointer[2], bool turnp1, int winpoint[2],
		wintype &Wtype, int brdsize, int winsize)
{
	//Print board
	text_colour(f_white, b_black);
	for (int i = 0; i <= brdsize * 4; i++)
	{
		cout << "-";
	}//print box outline
	cout << endl;
	//Loop through rows
	for (int x = 0; x < brdsize; x++)
	{	//Print box outline
		text_colour(f_white, b_black);
		cout << "| ";
		//Loop through columns
		for (int y = 0; y < brdsize; y++)
		{	//Set colour
			if (board[x][y] == X)
			{
				text_colour(f_cyan, b_black);
			}//if X
			else
			{
				text_colour(f_magenta, b_black);
			}//if O
			//Set colour - cursor position
			if (x == pointer[0] && y == pointer[1])
			{	//if blank, show user input
				if (board[x][y] == blank)
				{	//Show potential box content
					text_colour(f_dgray, b_black);
					if (turnp1)
					{
						cout << "X";
					}//if X
					else
					{
						cout << "O";
					}//else O
				}//if board is blank
			}//if cursor position
			//Set colour - cursor position but box is filled
			if (x == pointer[0] && y == pointer[1])
			{
				text_colour(f_red, b_black);
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
							text_colour(f_green, b_black);
						}
					}//for (win)
					break;
				case verti:
					for (int i = 0; i < winsize; i++)
					{
						if (x == winpoint[0] + i && y == winpoint[1])
						{
							text_colour(f_green, b_black);
						}
					}//for (win)
					break;
				case diagD:
					for (int i = 0; i < winsize; i++)
					{
						if (x == winpoint[0] + i && y == winpoint[1] + i)
						{
							text_colour(f_green, b_black);
						}
					}//for (win)
					break;
				case diagU:
					for (int i = 0; i < winsize; i++)
					{
						if (x == winpoint[0] - i && y == winpoint[1] + i)
						{
							text_colour(f_green, b_black);
						}
					}//for (win)
					break;
				}//switch(Wtype)
			}//if win point exists

			//Print based on box status
			switch (board[x][y])
			{
			case O:
				cout << "O";
				break;
			case X:
				cout << "X";
				break;
			case blank:
				if (!(x == pointer[0] && y == pointer[1]))
					cout << " ";
				break;
			}//switch(board)
			//Print box outline
			text_colour(f_white, b_black);
			cout << " | ";
		}//columns
		cout << endl;
		//Print box outline
		text_colour(f_white, b_black);
		for (int i = 0; i <= brdsize * 4; i++)
		{
			cout << "-";
		}//print spaces
		cout << endl;
	}//rows
}//print()

//==============================================================================================

//game - This function plays a single game to the end
//board - A matrix containing board stats
//pointer - Used for inputs, contains x and y values used to access matrices
//turnp1 - Whether or not it's player1's turn
//brbsize - Board size
//winsize- Amount of connections needed to win
void game(box board[SIZEb][SIZEb], int pointer[2], bool turnp1, int brdsize, int winsize)
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
			//clearS();
			system("cls");
			print(board, pointer, turnp1, winpoint, Wtype, brdsize, winsize);
			cout << "Player 1 has won!" << endl;
			return;
		case loss: 
			//clearS();
			system("cls");
			print(board, pointer, turnp1, winpoint, Wtype, brdsize, winsize);
			cout << "Player 2 has won!" << endl;
			return;
		case tie: 
			//clearS();
			system("cls");
			print(board, pointer, turnp1, winpoint, Wtype, brdsize, winsize);
			cout << "It was a tie!" << endl;
			return;
		case undef: break;
		default: cout << "ERROR: CHECK() RETURNS UNDEFINED VALUE." << endl;
		}//switch(check())

		//clearS();
		system("cls");
		print(board, pointer, turnp1, winpoint, Wtype, brdsize, winsize);
		//Print if user tries to replace an existing box
		if (Eout)
		{
			cout << "There's already something there!" << endl;
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
	case lef:
		if (y > 0)
		{
			y--;
		}//if
		break;
	case righ:
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
		key = _getch();
		if (key == 0 || key == 0xE0)
		{	//Check for key type
			key = _getch();
			//Return based on key press
			switch (key)
			{
			case 72: return up;
			case 80: return down;
			case 75: return lef;
			case 77: return righ;
			}//switch(key)
		}//if function key
		else
		{	//Allow WASD and SPACE
			switch (key)
			{
			case 119: return up;
			case 115: return down;
			case 97: return lef;
			case 100: return righ;
			case 13:
			case 32: return enter;
			}//switch(key)
		}//if not function key
	}//input loop
}//input()

//==============================================================================================

//check - This function checks to see if the win condition is met
//board - A matrix containing board stats
//winpoint - An array containing the x and y values of the win position
//Wtype - Which direction the winstreak is, used for print()
//winsize- Amount of connections needed to win
winstat check(box board[SIZEb][SIZEb], int winpoint[2], wintype &Wtype, int brdsize, int winsize)
{
	box checktype;			//Which type to check the win condition for
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
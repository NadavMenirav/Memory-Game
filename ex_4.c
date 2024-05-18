/**********************
 * Nadav Menirav
 * 330845678
 * ex_4
 **********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM_BOARD_INDEX 1
#define PLAYER_AMOUNT_INDEX 2
#define PLAYER_NAME_STRING_INDEX 3
#define FIRST_OBJECT_INDEX 4

#define MAX_PLAYER_NAME_LENGTH 20
#define MAX_OBJECT_NAME_LENGTH 7
#define MAX_PLAYERS 10
#define MAX_AMOUNT_PLAYERS 10
#define MAX_BOARD_DIM 30
#define AMOUNT_OBJECTS ((MAX_BOARD_DIM * MAX_BOARD_DIM) / 2)
#define MAX_PLAYER_NAME_STRING_LENGTH ((MAX_PLAYER_NAME_LENGTH + 1) * MAX_PLAYERS)
#define CELL_LENGTH 15

typedef enum {
	false, true
} boolean;

//I used structs instead of arrays to improve the looks and the readability of the code

typedef struct {
	char name[MAX_PLAYER_NAME_LENGTH];
	int score;  //Goes up every time the player gets a match
} Player;

typedef struct {
	char name[MAX_OBJECT_NAME_LENGTH];
	int firstPosition; //position of first cell of the pair
	int secondPosition; //position of second cell of the pair
	boolean isFound; 
} Object;

typedef	struct {
	boolean isValid; //Used to check in main if the command line arguments are valid 
	int dimBoard;
	int numPlayers;
	Player playerList[MAX_PLAYERS];
	Object objectList[AMOUNT_OBJECTS];
} Board;

void createBoard(int argc, char** argv, Board* ptr);
int fillPlayerList(Board* ptr, char* nameString, const char* delim);
void fillObjectListNames(Board* ptr, char** argv);
void fillObjectListPositions(Board* ptr, char** argv);
void invalidInput(Board* str);
boolean isRepeatingPosition(Board* ptr, char** argv);
boolean arePositionsInRange(Board* ptr, char** argv);
void instructions(Board board);
void printInstructionsBoard(Board board);
void printInstructionsObjects(Board board);
void playGame(Board* ptr);
void printCurrentBoard(Board* ptr);
Object* findObjectInPosition(Board* board, int pos);
void printNotFoundObject();
void printFoundObject(Object obj);
boolean isFinished(Board* board);
void playTurn(Board* ptr, Player* player);
void leaderboard(Board* ptr);

int main(int argc, char** argv) { 
	Board board;
	createBoard(argc, argv, &board); //Creates the first board
	if (!board.isValid) //Invalid command line arguments
		return 1;
	instructions(board); //Runs the instructions
	playGame(&board); //Starts the game
	return 0;
}

void createBoard(int argc, char** argv, Board* ptr) {
	/*
	The function checks validity the command line arguments and if they are valid
	it creates the board to start the game 
	*/

	//If there isn't a dimention
	if (argc < 2) {
		invalidInput(ptr);
		return;
	}
	int dimBoard = atoi(argv[DIM_BOARD_INDEX]); //After checking there is a dimension, we convert it to int using atoi
	int playerCount = 0;
	int objectCount = 0;
	char nameString[MAX_PLAYER_NAME_STRING_LENGTH];
	const char delim[2] = "#"; //To be used in the strtok function

	//Wrong amount of arguments 
	if (argc != dimBoard * dimBoard + (dimBoard * dimBoard) / 2 + 4) {
		invalidInput(ptr);
		return;
	}

	strcpy(nameString, argv[PLAYER_NAME_STRING_INDEX]);

	playerCount = fillPlayerList(ptr, nameString, delim); 
	
	if (playerCount != atoi(argv[PLAYER_AMOUNT_INDEX])) {
		invalidInput(ptr);
		return;
	}
	fillObjectListNames(ptr, argv);
	//If we got to this point, argc is correct hence the number of objects and positions are valid
	fillObjectListPositions(ptr, argv);
	if ((isRepeatingPosition(ptr, argv)) || (!arePositionsInRange(ptr, argv))) {
		invalidInput(ptr);
		return;
	}

	//If we got to this point, everything is valid and we shall insert the rest of the values into our board
	ptr->isValid = true;
	ptr->dimBoard = dimBoard;
	ptr->numPlayers = playerCount;
}

int fillPlayerList(Board* ptr, char* nameString, const char* delim) {
	/*The function extracts the names of players from the command line arguments, puts them in the player list,
	and returns the number of names
	*/
	int playerCount = 0;
	nameString = strtok(nameString, delim);
	while (nameString != NULL) {
		strcpy(ptr->playerList[playerCount].name, nameString);
		if (playerCount <= MAX_PLAYERS) //There is space
			ptr->playerList[playerCount].score = 0;
		else { //Amount of players in the string bigger than maximum amount allowed
			invalidInput(ptr);
			return -1;
		}
		nameString = strtok(NULL, delim);
		playerCount++; 
	}
	return playerCount; 
}

void fillObjectListNames(Board* ptr, char** argv) {
	//The function extracts the names of objects from the command line arguments and puts them in the list
	int objectCount = (atoi(argv[DIM_BOARD_INDEX]) * atoi(argv[DIM_BOARD_INDEX])) / 2;
	for (int i = 0; i < objectCount; i++) {
		strcpy(ptr->objectList[i].name, argv[FIRST_OBJECT_INDEX + i]);
		ptr->objectList[i].isFound = false;
	}	
}

void fillObjectListPositions(Board* ptr, char** argv) {
	/*
	The function extracts the positions from the command line arguments and puts them in the list, 
	it doesn't validate them, we will later
	*/
	int objectCount = (atoi(argv[DIM_BOARD_INDEX]) * atoi(argv[DIM_BOARD_INDEX])) / 2;
	int posIndex = FIRST_OBJECT_INDEX + objectCount;
	for (int i = 0; i < objectCount; i++) {
		ptr->objectList[i].firstPosition = atoi(argv[posIndex]);
		ptr->objectList[i].secondPosition = atoi(argv[posIndex + 1]);
		posIndex += 2; //Each object has two positions 
	}
}

boolean isRepeatingPosition(Board* ptr, char** argv) {
	/*
	The function checks if there are repeating positions in the command line arguments and returns a boolean value
	accordingly
	*/
	int posCount = (atoi(argv[DIM_BOARD_INDEX]) * atoi(argv[DIM_BOARD_INDEX])) ; //Number of positions is n^2
	int posIndex = FIRST_OBJECT_INDEX + posCount / 2;
	for (int i = 0; i < posCount; i++) {
		for (int j = i + 1; j < posCount; j++) {
			if (atoi(argv[posIndex + i]) == atoi(argv[posIndex + j])) {
				return true;
			}
		}	
	}
	return false;
}

boolean arePositionsInRange(Board* ptr, char** argv) {
	/*
	The function checks if the positions listed in the command line arguments are in the desired range (0 - (n^2 -1) )
	and returns a boolean value accordingly
	*/
	int n = atoi(argv[DIM_BOARD_INDEX]);
	int objectCount = (n * n) / 2;
	int posIndex = FIRST_OBJECT_INDEX + objectCount; //Index of the first position in the command line arguments
	for (int i = 0; i < objectCount * 2; i++) {
		int num = atoi(argv[posIndex + i]);
		if ((num > n * n - 1) || (num < 0)) {
			return false;
		}
	}
	return true;
}

void invalidInput(Board* ptr) {
	//The function is called when the command line arguments are invalid
	printf("INVALID INPUT\n");
	ptr->isValid = false;
}

void instructions(Board board) {
	//The function prints the game instructions for the user
	printf("Welcome to the Memory Card Game!\n");
	printf("The game will be played on the following board:\n\n");
	printInstructionsBoard(board);
	printf("And the following objects:\n\n");
	printInstructionsObjects(board);
	printf("Let the game begin!\n\n");	
}

void printInstructionsBoard(Board board) {
	//The function prints the template board for the user before the game starts
	int separationLength = 6 * board.dimBoard + 3;
	printf(" |");
	for (int i = 0; i < board.dimBoard; i++) {
		printf(" %d |", i);
	}
	printf("\n");
	for (int i = 0; i < separationLength; i++)
		printf("-");
	printf("\n");
	for (int i = 0; i < board.dimBoard; i++) {
		printf("%d |", i);
		for (int j = 0; j < board.dimBoard; j++) {
			printf("(%d,%d)|", i, j);
		}
		printf("\n");
		for (int j = 0; j < separationLength; j++)
			printf("-");
		printf("\n");
	}
	printf("\n");
}

void printInstructionsObjects(Board board) {
	//The function prints for the user the different objects in the game
	int numOfObjects = (board.dimBoard * board.dimBoard) / 2;
	for (int i = 0; i < numOfObjects; i++) {
		printf("%d. %s\n", i+1, board.objectList[i].name);
	}
	printf("\n");
}

void playGame(Board* ptr) {
	//The function starts the game
    int playerIndex = 0;
	int numOfObjects = (ptr->dimBoard * ptr->dimBoard) / 2;
	printCurrentBoard(ptr);
    while (!isFinished(ptr)) {
        playTurn(ptr, &(ptr->playerList[playerIndex]));
        playerIndex = ((playerIndex + 1) % ptr->numPlayers);
    }
    leaderboard(ptr);
}

void printCurrentBoard(Board* ptr) {
	/*
	The function goes over all the positions and checks if the object that withing them has been already found 
	and prints its cell accordingly
	*/
	int numOfCells = ptr->dimBoard * ptr->dimBoard;
	int separationLength = 16 * ptr->dimBoard + 1;
    printf( "Current board state:\n" );
	for (int i = 0; i < separationLength; i++) {
		printf("-");
	}
	printf("\n");
	printf("|");
	for (int i = 0; i < numOfCells; i++) {
		Object* obj = findObjectInPosition(ptr, i);
		if (obj == NULL) {
			return;
		}
		if (!obj->isFound) {
			printNotFoundObject();
		}
        else {
            printFoundObject(*obj);
        }
		if ((i + 1) % ptr->dimBoard == 0) {
			printf("\n");
			for (int i = 0; i < separationLength; i++) {
				printf("-");
			}
			printf("\n");
            if (i != numOfCells - 1) printf("|");
		}
	}
    printf("\n");
}

Object* findObjectInPosition(Board* board, int pos) {
	//The function gets a board and the deseired position on the board 
	int numOfObjects = (board->dimBoard * board->dimBoard) / 2;
	for (int i = 0; i < numOfObjects; i++) {
		if (board->objectList[i].firstPosition == pos || board->objectList[i].secondPosition == pos)
			return &board->objectList[i];
	}
	return NULL;
}

/*
    If the object is not found, we use this to print X
*/
void printNotFoundObject() {
	for (int i = 0; i < CELL_LENGTH / 2; i++)
		printf(" ");
	printf("X");
	for (int i = 0; i < CELL_LENGTH / 2; i++)
		printf(" ");
	printf("|");
}
/*
    If the object is found, calculate size and print
*/
void printFoundObject(Object obj) {
    int objectNameLength = strlen(obj.name);
    int spaces = CELL_LENGTH - objectNameLength;
    int spacesLeft = (spaces / 2) + (spaces % 2);
    int spacesRight = (spaces - spacesLeft);
    for (int b = 0; b < spacesLeft; b++) printf(" ");
    printf("%s", obj.name);
    for (int b = 0; b < spacesRight; b++) printf(" ");
	printf("|");
}
/* 
    Checks if all Objects are found
    If all objects are found returns true
*/
boolean isFinished(Board* board) {
    int numOfObjects = (board->dimBoard * board->dimBoard) / 2;
    for (int i = 0; i < numOfObjects; i++) {
        if (!(board->objectList[i].isFound)) return false;
    }
    return true;
    
}
// Runs one turn
void playTurn(Board* ptr, Player* player) {
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;
	int index1 = 0;
	int index2 = 0;
	Object* object1 = NULL;
	Object * object2 = NULL;
    boolean finishedTurn = false;
    int numOfCells = ptr->dimBoard * ptr->dimBoard;
    
    while (!finishedTurn) {
        printf("It is %s's turn.\n", (*player).name);
        boolean okCords = false;
        while (!okCords) {
            // Finding ok cords
            printf("Enter the coordinates of the first card (row col): ");
            scanf(" %d %d", &x1, &y1);
            printf("Enter the coordinates of the second card (row col): ");
            scanf(" %d %d", &x2, &y2);
            index1 = (x1 * ptr->dimBoard) + y1;
            index2 = (x2 * ptr->dimBoard) + y2;
            object1 = findObjectInPosition(ptr, index1);
            object2 = findObjectInPosition(ptr, index2);
			if (object1 == NULL || object2 == NULL) {
				printf("Invalid move. Try again.\n");
				continue;
			}
				
            if (
                x1 >= ptr->dimBoard ||
                x1 < 0 ||
                y1 < 0 ||
                y1 >= ptr->dimBoard
            ) {
                printf("Invalid move. Try again.\n");
                continue;
            }
            if (
                x2 >= ptr->dimBoard ||
                x2 < 0 ||
                y2 < 0 ||
                y2 >= ptr->dimBoard
            ) {
                printf("Invalid move. Try again.\n");
                continue;
            }
            if (
                (x1 == x2 && y1 == y2) ||
                object1->isFound ||
                object2->isFound
            ) {
                printf("Invalid move. Try again.\n");
                continue;
            }
            else okCords = true;
            // At this point the move is ok
        }
        if ((*object1).firstPosition == index2 ||
            (*object1).secondPosition == index2
        ) {
            // If match
            (*object1).isFound = true;
            (*player).score++;
            printf("Match!\n");
        }
        else {
            // If its not a match
            finishedTurn = true;
            printf("No match. Try again.\n");
        }
        printf("\nThe scores are:\n");
        for (int i = 0; i < ptr->numPlayers; i++) {
            printf("%s: %d", ptr->playerList[i].name, ptr->playerList[i].score);
            if (i != (ptr->numPlayers - 1)) printf(", ");
        }
        printf("\n");
        if (isFinished(ptr)) return;
        printCurrentBoard(ptr);
    }

}
void leaderboard(Board* ptr) {
	//The function prints the winner of the game
    Player winner = ptr->playerList[0];
    boolean tied = false;

    for (int i = 1; i < ptr->numPlayers; i++) {
        if (ptr->playerList[i].score > winner.score) {
            winner = ptr->playerList[i];
            tied = false;
        }
        if (ptr->playerList[i].score == winner.score) tied = true;
    }
    if (!tied) {
        printf("Congratulations %s! You won!\n", winner.name);
    }
    else {
        printf("It's a tie!\n");
    }
}
#include <stdio.h>
#include <stdlib.h>

/*  Written by Ali Egemen Bilak using VSCode and JDK 21.0.5 on Ubuntu 24.04.1 LTS
    https://github.com/ducktumn
    Last Edit: 29.12.2024
    Made for MEF University COMP205 final project.
*/

#define RESET "\033[0m"
#define ORANGE_BG "\033[48;5;202m"
#define WHITE_TEXT "\033[97m"
#define CYAN_BG "\033[46m"
#define WHITE_BG "\033[47m"
#define BOLD "\033[1m"
#define RED_TEXT "\033[31m"
#define YELLOW "\033[1;33m"

typedef unsigned long long int ull_int;
typedef unsigned short int us_int;

// Hej hej hej sokoły
// https://open.spotify.com/track/16okyAWAlUFsNacLxukOsp?si=5762286771f948a4

char *getBinary(ull_int number, us_int bitCount);
void printGameTable(us_int size, ull_int xStatus, ull_int oStatus);
int checkWin(ull_int status, ull_int *possibilities, us_int countOfPossibilities);
void clearBuffer();
int isMoveValid(us_int size, us_int x, us_int y, ull_int xState, ull_int oState, us_int current);
void printResult(us_int result);
void makeMove(ull_int *status, us_int row, us_int col, us_int size);
void preCalculateMoves(ull_int **possibilities, us_int size, us_int *countOfPossibilities);

int main()
{
    ull_int stateOfX = 0;
    ull_int stateOfO = 0;

    us_int size = 0;
    printf("%sEnter the size of the game (Maximum is 8, Minimum is 3) : %s", WHITE_TEXT, RESET);
    scanf("%hu", &size);
    clearBuffer();
    if (size < 3)
    {
        printf("\n%sInvalid board size!%s\n", RED_TEXT, RESET);
        return 1;
    }

    ull_int *winConditions;
    us_int winConditionCount;
    preCalculateMoves(&winConditions, size, &winConditionCount);
    ull_int *preMoves = (ull_int *)malloc(sizeof(ull_int) * size * size);
    if (preMoves == NULL)
    {
        printf("%sMemory Allocation Fail!%s\n", RED_TEXT, RESET);
        return 1;
    }

    us_int current = 0;
    printf("%sEnter the first player (0 for X, 1 for O) : %s", WHITE_TEXT, RESET);
    scanf("%hu", &current);
    if (current != 1 && current != 0)
        current = 0;
    clearBuffer();
    preMoves[0] = 0UL;
    preMoves[0] = 0UL;

    us_int moveCount = 0;
    us_int gameOver = 0;
    while ((moveCount < size * size) && !gameOver)
    {

        char player = 'O';
        if (current == 0)
            player = 'X';

        us_int decision;
        printf("\n%sWhat would you like to do?\nMove - 0, Undo - 1: %s", WHITE_TEXT, RESET);
        scanf("%hu", &decision);
        clearBuffer();
        while ((decision != 1) && (decision != 0))
        {
            printf("\n%sInvalid input!\n%sWhat would you like to do?\nMove - 0, Undo - 1: %s", RED_TEXT, WHITE_TEXT, RESET);
            scanf("%hu", &decision);
            clearBuffer();
        }
        if (decision && (moveCount > 1))
        {
            if (current == 0)
            {
                current = 1;
                stateOfO = preMoves[moveCount - 2];
                moveCount--;
                printGameTable(size, stateOfX, stateOfO);
            }
            else
            {
                current = 0;
                stateOfX = preMoves[moveCount - 2];
                moveCount--;
                printGameTable(size, stateOfX, stateOfO);
            }
            continue;
        }
        else if ((moveCount == 1) && decision)
        {
            if (current == 0)
            {
                current = 1;
                stateOfO = 0;
                moveCount--;
                printGameTable(size, stateOfX, stateOfO);
            }
            else
            {
                current = 0;
                stateOfX = 0;
                moveCount--;
                printGameTable(size, stateOfX, stateOfO);
            }
            continue;
        }
        else if (decision && (moveCount == 0))
        {
            printf("\n%sCan't undo further!%s\n", RED_TEXT, RESET);
            continue;
        }

        us_int row;
        us_int col;
        printf("\n%sEnter your move for \"%c\" by typing row and column with a space in the middle: %s", WHITE_TEXT, player, RESET);
        scanf("%hu%hu", &row, &col);
        clearBuffer();
        while (!isMoveValid(size, row, col, stateOfX, stateOfO, current))
        {
            printf("\n%sInvalid input!\n%sEnter your move for \"%c\" by typing row and column with a space in the middle: %s", RED_TEXT, WHITE_TEXT, player, RESET);
            scanf("%hu%hu", &row, &col);
            clearBuffer();
        }

        if (current == 0)
        {
            makeMove(&stateOfX, row, col, size);
            preMoves[moveCount + 1] = stateOfX;
            moveCount++;
            if (checkWin(stateOfX, winConditions, winConditionCount))
                gameOver = 1;
            else
                current = 1;
        }

        else
        {
            makeMove(&stateOfO, row, col, size);
            preMoves[moveCount + 1] = stateOfO;
            moveCount++;
            if (checkWin(stateOfO, winConditions, winConditionCount))
                gameOver = 1;
            else
                current = 0;
        }

        printGameTable(size, stateOfX, stateOfO);
    }

    if (gameOver)
        if (current == 0)
            printResult(0);
        else
            printResult(1);
    else
        printResult(2);

    return 0;
}

void printGameTable(us_int size, ull_int xStatus, ull_int oStatus)
{
    printf("\n");
    printf("   ");
    for (int i = 0; i < size; i++)
    {
        printf("%s%s %d %s", BOLD, WHITE_TEXT, i + 1, RESET);
    }
    printf("\n");
    for (int i = 0; i < size; i++)
    {
        printf("%s%s %d %s", BOLD, WHITE_TEXT, i + 1, RESET);
        for (int j = 0; j < size; j++)
        {
            if ((xStatus & 1) == 1)
            {
                printf("%s%s X %s", ORANGE_BG, WHITE_TEXT, RESET);
            }
            else if ((oStatus & 1) == 1)
            {
                printf("%s%s O %s", CYAN_BG, WHITE_TEXT, RESET);
            }
            else
            {
                printf("%s   %s", WHITE_BG, RESET);
            }

            xStatus = xStatus >> 1;
            oStatus = oStatus >> 1;
        }

        printf("\n");
    }
    printf("\n");
}

int checkWin(ull_int status, ull_int *possibilities, us_int countOfPossibilities)
{
    for (int i = 0; i < countOfPossibilities; i++)
    {
        if ((possibilities[i] & status) == possibilities[i])
            return 1;
    }
    return 0;
}

char *getBinary(ull_int number, us_int bitCount)
{
    char *returnPointer = (char *)malloc(bitCount + 1);
    if (returnPointer == NULL)
    {
        printf("%sMemory Allocation Fail!%s\n", RED_TEXT, RESET);
        exit(1);
    }

    for (int i = bitCount - 1; i >= 0; i--)
    {
        if (number & 1)
            returnPointer[i] = '1';
        else
            returnPointer[i] = '0';
        number = number >> 1;
    }

    returnPointer[bitCount] = '\0';

    return returnPointer;
}

void clearBuffer()
{
    while (getchar() != '\n')
        ;
}

int isMoveValid(us_int size, us_int x, us_int y, ull_int xState, ull_int oState, us_int current)
{
    if ((x <= size) && (y <= size))
    {
        us_int tempShiftCount = (x - 1) * size + y - 1;
        ull_int move = 1UL << tempShiftCount;

        if (((xState & move) == 0) && ((oState & move) == 0))
            return 1;
    }
    return 0;
}

void printResult(us_int result)
{
    switch (result)
    {
    case 0:
        printf("\n\"%sX\" won!!!%s\n", YELLOW, RESET);
        break;
    case 1:
        printf("\n\"%sO\" won!!!%s\n", YELLOW, RESET);
        break;
    case 2:
        printf("\n%sIt's a draw!!!%s\n", YELLOW, RESET);
        break;
    default:
        break;
    }
}

void makeMove(ull_int *status, us_int row, us_int col, us_int size)
{
    int index_a = ((row - 1) * size + col);
    ull_int k = 1;
    *status |= (k << (index_a - 1));
}

void preCalculateMoves(ull_int **possibilities, us_int size, us_int *countOfPossibilities)
{
    us_int moveCount = 2 * (size - 2) * size + 2 * (size - 2) + 4 * (size - 3) * (1 + (size - 4) * 0.5);
    *countOfPossibilities = moveCount;
    us_int movesLeft = moveCount;

    ull_int *winConditions = (ull_int *)malloc(sizeof(ull_int) * moveCount);

    if (winConditions == NULL)
    {
        printf("%sMemory Allocation Fail!%s\n", RED_TEXT, RESET);
        exit(1);
    }
    ull_int bit = 1;
    {
        ull_int tempHorizontal = (ull_int)7;
        ull_int tempVertical = bit + (bit << size) + (bit << (2 * size));
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size - 2; j++)
            {
                winConditions[moveCount - movesLeft] = tempHorizontal;
                movesLeft--;
                winConditions[moveCount - movesLeft] = tempVertical;
                movesLeft--;
                if (j != (size - 3))
                {
                    tempHorizontal <<= 1;
                    tempVertical <<= size;
                }
            }
            tempHorizontal <<= 3;
            if (size == 3)
                tempVertical <<= 1;
            else
                tempVertical >>= ((size - 3) * size - 1);
        }
    }
    {
        ull_int firstMainDiagonal = 1 + (bit << (size + 1)) + (bit << (size + 1) << (size + 1));
        ull_int secondMainDiagonal = (bit << (size - 1)) + (bit << (2 * size - 2)) + (bit << (3 * size - 3));
        for (int i = 0; i < size - 2; i++)
        {
            winConditions[moveCount - movesLeft] = firstMainDiagonal;
            movesLeft--;
            winConditions[moveCount - movesLeft] = secondMainDiagonal;
            movesLeft--;
            firstMainDiagonal <<= (size + 1);
            secondMainDiagonal <<= (size - 1);
        }
    }
    {
        ull_int tempUpSubDiagonal = (bit << (2 * size)) + (bit << (size + 1)) + (bit << 2);
        ull_int tempDownSubDiagonal = (bit << (size - 3)) + (bit << (2 * size - 2)) + (bit << (3 * size - 1));

        us_int ascending = 1;
        for (int i = 0; i < 2 * size - 5; i++)
        {
            if (i == (size - 3))
            {
                ascending = 0;
                ull_int firstDescendingBit = bit << (size * (size - 1) + 1);
                tempUpSubDiagonal = firstDescendingBit + (firstDescendingBit >> (size - 1)) + (firstDescendingBit >> (2 * size - 2));
                firstDescendingBit <<= (size - 3);
                tempDownSubDiagonal = firstDescendingBit + (firstDescendingBit >> (size + 1)) + (firstDescendingBit >> (2 * size + 2));
            }
            else if (ascending)
            {
                for (int j = 0; j < i + 1; j++)
                {
                    winConditions[moveCount - movesLeft] = tempUpSubDiagonal;
                    movesLeft--;
                    winConditions[moveCount - movesLeft] = tempDownSubDiagonal;
                    movesLeft--;
                    if (j != i)
                    {
                        tempUpSubDiagonal >>= (size - 1);
                        tempDownSubDiagonal >>= (size + 1);
                    }
                }
                ull_int ascendingBit = bit << ((3 + i) * size);
                tempUpSubDiagonal = ascendingBit + (ascendingBit >> (size - 1)) + (ascendingBit >> (2 * size - 2));
                ascendingBit = bit << ((4 + i) * size - 1);
                tempDownSubDiagonal = ascendingBit + (ascendingBit >> (size + 1)) + (ascendingBit >> (2 * size + 2));
            }
            else
            {
                for (int j = 0; j < ((2 * size - 5) - i); j++)
                {
                    winConditions[moveCount - movesLeft] = tempUpSubDiagonal;
                    movesLeft--;
                    winConditions[moveCount - movesLeft] = tempDownSubDiagonal;
                    movesLeft--;
                    if (j != ((2 * size - 6) - i))
                    {
                        tempUpSubDiagonal >>= (size - 1);
                        tempDownSubDiagonal >>= (size + 1);
                    }
                }
                ull_int ascendingBit = bit << ((size - 1) * size + i - (size - 4));
                tempUpSubDiagonal = ascendingBit + (ascendingBit >> (size - 1)) + (ascendingBit >> (2 * size - 2));
                ascendingBit = bit << ((size - 1) * size + (2 * size - 5 - i));
                tempDownSubDiagonal = ascendingBit + (ascendingBit >> (size + 1)) + (ascendingBit >> (2 * size + 2));
            }
        }
    }
    *possibilities = winConditions;
}

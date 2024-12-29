#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

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
#define PREVIOUS_LINE "\033[F"

typedef unsigned long long int ull_int;
typedef unsigned short int us_int;

// Hej hej hej sokoły
// https://open.spotify.com/track/16okyAWAlUFsNacLxukOsp?si=5762286771f948a4

void printGameTable(us_int size, ull_int xStatus, ull_int oStatus);
int checkWin(ull_int status, ull_int *possibilities, us_int countOfPossibilities);
void clearBuffer();
int isMoveValid(us_int size, us_int x, us_int y, ull_int xState, ull_int oState, us_int current);
void printResult(us_int result);
void makeMove(ull_int *status, us_int row, us_int col, us_int size);
void preCalculateMoves(ull_int **possibilities, us_int size, us_int *countOfPossibilities);
void clearTerminal();
void sleepSecond(int seconds);

int main()
{
    ull_int stateOfX = 0;
    ull_int stateOfO = 0;

    us_int size = 0;
    printf("%s%sEnter the size of the game (Maximum is 8, Minimum is 3): %s", WHITE_TEXT, PREVIOUS_LINE, RESET);
    scanf("%hu", &size);
    clearBuffer();
    clearTerminal();
    while ((size < 3) || (size > 8))
    {
        printf("%sInvalid board size!\n%sEnter the size of the game (Maximum is 8, Minimum is 3): %s", RED_TEXT, WHITE_TEXT, RESET);
        scanf("%hu", &size);
        clearBuffer();
        clearTerminal();
    }

    ull_int *winConditions;
    us_int winConditionCount;
    preCalculateMoves(&winConditions, size, &winConditionCount);
    ull_int *preMoves = (ull_int *)malloc(sizeof(ull_int) * size * size);
    if (preMoves == NULL)
    {
        clearTerminal();
        printf("%sMemory Allocation Fail!%s\n", RED_TEXT, RESET);
        sleepSecond(5);
        return 1;
    }

    int sizeOfInfo = 7;
    if (size > 7)
        sizeOfInfo = 8;
    char *gameInfoReset = (char *)malloc(sizeof(char) * sizeOfInfo);
    if (gameInfoReset == NULL)
    {
        clearTerminal();
        printf("%sMemory Allocation Fail!%s\n", RED_TEXT, RESET);
        sleepSecond(5);
        return 1;
    }
    gameInfoReset[sizeOfInfo - 1] = '\0';
    gameInfoReset[sizeOfInfo - 2] = 'H';
    gameInfoReset[sizeOfInfo - 3] = '1';
    gameInfoReset[sizeOfInfo - 4] = ';';
    gameInfoReset[1] = '[';
    gameInfoReset[0] = '\033';
    if (size > 7)
    {
        int tempNum = size + 2;
        gameInfoReset[2] = (char)((tempNum / 10) + '0');
        gameInfoReset[3] = (char)((tempNum % 10) + '0');
    }
    else
    {
        gameInfoReset[2] = (char)((size + 2) + '0');
    }

    us_int current = 0;
    printf("%sEnter the first player (0 for X, 1 for O): %s", WHITE_TEXT, RESET);
    scanf("%hu", &current);
    clearBuffer();
    clearTerminal();
    while (current != 1 && current != 0)
    {
        printf("%sInvalid player number!\n%sEnter the first player (0 for X, 1 for O): %s", RED_TEXT, WHITE_TEXT, RESET);
        scanf("%hu", &current);
        clearBuffer();
        clearTerminal();
    }

    preMoves[0] = 0UL;

    us_int moveCount = 0;
    us_int gameOver = 0;
    printGameTable(size, 0, 0);
    while ((moveCount < size * size) && !gameOver)
    {
        char player = 'O';
        if (current == 0)
            player = 'X';

        us_int decision;
        printf("%s\nWhat would you like to do?\nMove - 0, Undo - 1: %s", WHITE_TEXT, RESET);
        scanf("%hu", &decision);
        clearBuffer();
        while ((decision != 1) && (decision != 0))
        {
            printf("%s%s\nInvalid input!%12s\n%sWhat would you like to do?\nMove - 0, Undo - 1: %s", RED_TEXT, gameInfoReset, "", WHITE_TEXT, RESET);
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
                clearTerminal();
                printGameTable(size, stateOfX, stateOfO);
            }
            else
            {
                current = 0;
                stateOfX = 0;
                moveCount--;
                clearTerminal();
                printGameTable(size, stateOfX, stateOfO);
            }
            continue;
        }
        else if (decision && (moveCount == 0))
        {
            clearTerminal();
            printGameTable(size, stateOfX, stateOfO);
            printf("%s%s\nCan't undo further!%s", RED_TEXT, gameInfoReset, RESET);
            continue;
        }

        us_int row;
        us_int col;
        clearTerminal();
        printGameTable(size, stateOfX, stateOfO);
        printf("%s%s\nEnter your move for \"%c\" by typing row and column with a space in the middle: %s", gameInfoReset, WHITE_TEXT, player, RESET);
        scanf("%hu%hu", &row, &col);
        clearBuffer();

        while (!isMoveValid(size, row, col, stateOfX, stateOfO, current))
        {
            printf("%s\n%sInvalid input!%75s\n%sEnter your move for \"%c\" by typing row and column with a space in the middle: %s", gameInfoReset, RED_TEXT, "", WHITE_TEXT, player, RESET);
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
        clearTerminal();
        printGameTable(size, stateOfX, stateOfO);
    }

    if (gameOver)
        if (current == 0)
            printResult(0);
        else
            printResult(1);
    else
        printResult(2);
    sleepSecond(5);

    free(winConditions);
    free(preMoves);
    free(gameInfoReset);
    return 0;
}

void printGameTable(us_int size, ull_int xStatus, ull_int oStatus)
{
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
        clearTerminal();
        printf("%sMemory Allocation Fail!%s\n", RED_TEXT, RESET);
        sleepSecond(5);
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

void clearTerminal()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void sleepSecond(int seconds)
{
#ifdef _WIN32
    Sleep(seconds * 1000);
#else
    sleep(seconds);
#endif
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

char *getBinary(unsigned long int number, unsigned int bitCount);
void printGameTable(int size, unsigned long int xStatus, unsigned long int oStatus);
int checkWin(unsigned long int status, int size);
void clearBuffer();
int isMoveValid(int size, unsigned int x, unsigned int y, unsigned long int xState, unsigned long int oState, int current);
void printResult(int result);

int main()
{
    unsigned long int stateOfX = 0;
    unsigned long int stateOfO = 0;

    int size = 0;
    printf("Enter the size of the game (Maximum is 8, Minimum is 2) : ");
    scanf("%d", &size);
    clearBuffer();
    if (size < 2)
        return 1;

    int current = 0;
    printf("Enter the first player (0 for X, 1 for O) : ");
    scanf("%d", &current);
    if (current != 1 || current != 0)
        current = 0;
    clearBuffer();

    int moveCount = 0;
    int gameOver = 0;
    while ((moveCount <= size * size) || !gameOver)
    {
        moveCount++;
        int currentMove = 0;
        char player = 'O';
        if (current % 2 == 0)
            player = 'X';

        unsigned int row;
        unsigned int col;
        printf("\nEnter your move for \"%c\" by typing row and column with a space in the middle: ", player);
        scanf("%u%u", &row, &col);
        clearBuffer();
        while (!isMoveValid(size, row, col, stateOfX, stateOfO, current))
        {
            printf("Invalid input!\nEnter your move for \"%c\" by typing row and column with a space in the middle: ", player);
            scanf("%u%u", &row, &col);
            clearBuffer();
        }

        if (current % 2 == 0)
        {
            stateOfX += pow(2, row * col - 1);
            if (checkWin(stateOfX, size))
                gameOver = 1;
            else
                current = 1;
        }

        else
        {
            stateOfO += pow(2, row * col - 1);
            if (checkWin(stateOfO, size))
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

void printGameTable(int size, unsigned long int xStatus, unsigned long int oStatus)
{
    printf("\n");
    for (int i = 0; i < size; i++)
    {
        for (int i = 0; i < size; i++)
        {
            if ((xStatus & 1) == 1)
                printf(" X ");
            else if ((oStatus & 1) == 1)
                printf(" O ");
            else
                printf(" - ");
            xStatus = xStatus >> 1;
            oStatus = oStatus >> 1;
        }
        printf("\n");
    }
    printf("\n");
}

int checkWin(unsigned long int status, int size)
{
    for (int i = 0; i < size; i++)
    {
        int horizonalCheck = pow(2, size * i) * (pow(2, size) - 1);
        int verticalCheck = pow(2, i) * (pow(8, size) - 1) / 7;
        if ((status & horizonalCheck) == horizonalCheck || (status & verticalCheck) == verticalCheck)
            return 1;
    }
    int firstDiagonal = (pow(pow(2, size + 1), size) - 1) / (pow(2, size + 1) - 1);
    int secondDiagonal = pow(2, size - 1) * (pow(pow(2, size - 1), size) - 1) / (pow(2, size - 1) - 1);

    if ((status & firstDiagonal) == firstDiagonal || (status & secondDiagonal) == secondDiagonal)
        return 1;

    return 0;
}

char *getBinary(unsigned long int number, unsigned int bitCount)
{
    char *returnPointer = (char *)malloc(bitCount + 1);
    if (returnPointer == NULL)
    {
        return NULL;
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

int isMoveValid(int size, unsigned int x, unsigned int y, unsigned long int xState, unsigned long int oState, int current)
{

    if ((x <= size) && (y <= size))
    {
        unsigned int move = pow(2, x * y - 1);
        if ((xState & move == 0) && (oState & move))
            return 1;
    }
    return 0;
}

void printResult(int result)
{
    switch (result)
    {
    case 0:
        printf("\n\"X\" won!!!\n");
        break;
    case 1:
        printf("\n\"O\" won!!!\n");
        break;
    case 2:
        printf("It's a draw!!!\n");
        break;
    default:
        break;
    }
}
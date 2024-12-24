#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

// Hej hej hej sokoły :D
// https://open.spotify.com/track/16okyAWAlUFsNacLxukOsp?si=5762286771f948a4

char *getBinary(unsigned long int number, unsigned short int bitCount);
void printGameTable(unsigned short int size, unsigned long int xStatus, unsigned long int oStatus);
int checkWin(unsigned long int status, unsigned short int size);
void clearBuffer();
int isMoveValid(unsigned short int size, unsigned short int x, unsigned short int y, unsigned long int xState, unsigned long int oState, unsigned short int current);
void printResult(unsigned short int result);

int main()
{
    unsigned long int stateOfX = 0;
    unsigned long int stateOfO = 0;

    unsigned short int size = 0;
    printf("Enter the size of the game (Maximum is 8, Minimum is 3) : ");
    scanf("%hu", &size);
    clearBuffer();
    if (size < 3)
    {
        printf("\nInvalid board size!\n");
        return 1;
    }

    unsigned short int current = 0;
    printf("Enter the first player (0 for X, 1 for O) : ");
    scanf("%hu", &current);
    if (current != 1 && current != 0)
        current = 0;
    clearBuffer();

    unsigned short int moveCount = 0;
    unsigned short int gameOver = 0;
    while ((moveCount < size * size) && !gameOver)
    {
        moveCount++;
        char player = 'O';
        if (current == 0)
            player = 'X';

        unsigned short int row;
        unsigned short int col;
        printf("\nEnter your move for \"%c\" by typing row and column with a space in the middle: ", player);
        scanf("%hu%hu", &row, &col);
        clearBuffer();
        while (!isMoveValid(size, row, col, stateOfX, stateOfO, current))
        {
            printf("Invalid input!\nEnter your move for \"%c\" by typing row and column with a space in the middle: ", player);
            scanf("%hu%hu", &row, &col);
            clearBuffer();
        }

        if (current == 0)
        {
            stateOfX += (unsigned long int)pow(2, (row - 1) * size + col - 1);
            if (checkWin(stateOfX, size))
                gameOver = 1;
            else
                current = 1;
        }

        else
        {
            stateOfO += (unsigned long int)pow(2, (row - 1) * size + col - 1);
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

void printGameTable(unsigned short int size, unsigned long int xStatus, unsigned long int oStatus)
{
    printf("\n");
    for (int i = 0; i < size; i++)
    {
        char *tempString = (char *)malloc(size + 1);
        tempString[size] = '\0';
        for (int j = 0; j < size; j++)
        {
            if ((xStatus & 1) == 1)
                tempString[j] = 'X';
            else if ((oStatus & 1) == 1)
                tempString[j] = 'O';
            else
                tempString[j] = '-';
            xStatus = xStatus >> 1;
            oStatus = oStatus >> 1;
        }
        printf("%s\n", tempString);
        free(tempString);
    }
    printf("\n");
}

int checkWin(unsigned long int status, unsigned short int size)
{
    for (int i = 0; i < size; i++)
    {
        unsigned long int horizontalCheck = (unsigned long int)(pow(2, i * size) * 7);
        unsigned long int tempVerticalR = (unsigned long int)pow(2, size);
        unsigned long int verticalCheck = (unsigned long int)(pow(2, i) * (pow(tempVerticalR, 3) - 1) / (tempVerticalR - 1));
        for (int j = 0; j < size - 2; j++)
        {
            if ((horizontalCheck & status) == horizontalCheck)
                return 1;
            horizontalCheck << 1;
        }
        for (int j = 0; j < size - 2; j++)
        {
            if ((verticalCheck & status) == verticalCheck)
                return 1;
            verticalCheck << size;
        }
    }

    {
        unsigned long int tempDiagonalR = (unsigned long int)pow(2, size + 1);
        unsigned long int rightDiagonalCheck = (unsigned long int)((pow(tempDiagonalR, 3) - 1) / tempDiagonalR - 1);
        for (int i = 0; i < (size - 2); i++)
        {
            if ((rightDiagonalCheck & status) == rightDiagonalCheck)
                return 1;
            rightDiagonalCheck << (size + 1);
        }
        for (int i = 0; i < (size - 3); i++)
        {
            unsigned long int subRightDiagonalCheck = (unsigned long int)(pow(2, i + 1) * (pow(tempDiagonalR, 3) - 1) / (tempDiagonalR - 1));
            unsigned long int secondSubRightDiagonalCheck = (unsigned long int)(pow(2, (i + 1) * size) * (pow(tempDiagonalR, 3) - 1) / (tempDiagonalR - 1));
            for (int j = 0; j < size - 3 - i; j++)
            {
                if ((subRightDiagonalCheck & status) == subRightDiagonalCheck)
                    return 1;
                if ((secondSubRightDiagonalCheck & status) == secondSubRightDiagonalCheck)
                    return 1;
                subRightDiagonalCheck << (size + 1);
                secondSubRightDiagonalCheck << (size + 1);
            }
        }
    }

    {
        unsigned long int tempDiagonalR = (unsigned long int)pow(2, size - 1);
        unsigned long int leftDiagonalCheck = (unsigned long int)(pow(2, size - 1) * ((pow(tempDiagonalR, 3) - 1) / tempDiagonalR - 1));
        for (int i = 0; i < (size - 2); i++)
        {
            if ((leftDiagonalCheck & status) == leftDiagonalCheck)
                return 1;
            leftDiagonalCheck << (size - 1);
        }
        for (int i = 0; i < (size - 3); i++)
        {
            unsigned long int subLeftDiagonalCheck = (unsigned long int)(pow(2, (size - 2 - i)) * (pow(tempDiagonalR, 3) - 1) / (tempDiagonalR - 1));
            unsigned long int secondSubLeftDiagonalCheck = (unsigned long int)(pow(2, (i + 2) * size - 1) * (pow(tempDiagonalR, 3) - 1) / (tempDiagonalR - 1));
            for (int j = 0; j < size - 3 - i; j++)
            {
                if ((subLeftDiagonalCheck & status) == subLeftDiagonalCheck)
                    return 1;
                if ((secondSubLeftDiagonalCheck & status) == secondSubLeftDiagonalCheck)
                    return 1;
                subLeftDiagonalCheck << (size - 1);
                secondSubLeftDiagonalCheck << (size - 1);
            }
        }
    }

    return 0;
}

char *getBinary(unsigned long int number, unsigned short int bitCount)
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

int isMoveValid(unsigned short int size, unsigned short int x, unsigned short int y, unsigned long int xState, unsigned long int oState, unsigned short int current)
{
    if ((x <= size) && (y <= size))
    {
        unsigned long int move = (unsigned long int)pow(2, (x - 1) * size + y - 1);
        if (((xState & move) == 0) && ((oState & move) == 0))
            return 1;
    }
    return 0;
}

void printResult(unsigned short int result)
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

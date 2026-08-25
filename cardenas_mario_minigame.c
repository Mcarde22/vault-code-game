/*
 * Name: Mario Cardenas
 * CSC 3320 - Homework 02
 *
 * This is my vault guessing game program. The computer picks a random
 * 3 digit code and the player has to guess it. After each guess I tell
 * them if they were too high or too low, how many digits they got in
 * the right spot, and how many digits are right but in the wrong spot.
 * Type q to quit early.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define DIGITS 3 /* how many digits the code has */
#define RANGE  10 /* each digit can be 0-9 */
#define TRIES  8 /* number of guesses you get */

void generateSecretCode(int secretDigits[], int digitCount);
int  digitsToInteger(int digits[], int digitCount);
int  readGuess(char inputLine[], int lineSize);
int  parseGuessDigits(char inputLine[], int guessDigits[], int digitCount);
void compareGuess(int guessDigits[], int secretDigits[], int digitCount,
                   int *correctPosition, int *correctDigitWrongPosition);

int main(void)
{
    int secretDigits[DIGITS];
    int secretCode;
    int guessDigits[DIGITS];
    char inputLine[100];
    int triesRemaining;
    int guessCode;
    int correctPosition;
    int correctDigitWrongPosition;
    int hasWon;
    int quitRequested;
    int parseResult;

    /* seed the random numbers so we dont get the same code every time */
    srand((unsigned int) time(NULL));

    /* make the secret code and turn it into a number too */
    generateSecretCode(secretDigits, DIGITS);
    secretCode = digitsToInteger(secretDigits, DIGITS);

    triesRemaining = TRIES;
    hasWon = 0;
    quitRequested = 0;

    /* main game loop, keeps asking until you win, quit, or run out of tries */
    while (triesRemaining > 0 && !hasWon && !quitRequested)
    {
        printf("%d tries remaining. What is the code? ", triesRemaining);

        if (readGuess(inputLine, sizeof(inputLine)) == 0)
        {
            /* nothing left to read so just bail out */
            quitRequested = 1;
            continue;
        }

        /* check if they typed q or Q to quit */
        if (strlen(inputLine) == 1 &&
            (inputLine[0] == 'q' || inputLine[0] == 'Q'))
        {
            quitRequested = 1;
            continue;
        }

        /* make sure what they typed is actually a valid guess */
        parseResult = parseGuessDigits(inputLine, guessDigits, DIGITS);

        if (!parseResult)
        {
            printf("Invalid input. Please enter exactly %d digits, ",
                   DIGITS);
            printf("or 'q' to quit.\n");
            continue;
        }

        guessCode = digitsToInteger(guessDigits, DIGITS);

        /* figure out how many digits are right and where */
        compareGuess(guessDigits, secretDigits, DIGITS,
                     &correctPosition, &correctDigitWrongPosition);

        if (guessCode == secretCode)
        {
            hasWon = 1;
            printf("You opened the vault!\n");
        }
        else
        {
            if (guessCode > secretCode)
            {
                printf("Too high, ");
            }
            else
            {
                printf("Too low, ");
            }

            printf("%d digits are correct and in the right place, ",
                   correctPosition);
            printf("%d digits are correct and in the wrong place.\n",
                   correctDigitWrongPosition);

            triesRemaining--; /* they used up a try */
        }
    }

    /* print the right ending message depending on how the game ended */
    if (quitRequested)
    {
        printf("Thanks for playing! The secret code was %d\n", secretCode);
    }
    else if (!hasWon)
    {
        printf("The vault shuts down permanently due to too many ");
        printf("incorrect attempts. You failed to open the vault!\n");
        printf("The secret code was %d\n", secretCode);
    }

    return 0;
}

/* fills the array with random digits 0-9, this is the secret code */
void generateSecretCode(int secretDigits[], int digitCount)
{
    int i;

    for (i = 0; i < digitCount; i++)
    {
        secretDigits[i] = rand() % RANGE;
    }
}

/* takes the digit array like [4,0,3] and turns it into the number 403 */
int digitsToInteger(int digits[], int digitCount)
{
    int i;
    int value;

    value = 0;

    for (i = 0; i < digitCount; i++)
    {
        value = value * 10 + digits[i];
    }

    return value;
}

/* grabs a line the player typed, returns 0 if we hit end of input */
int readGuess(char inputLine[], int lineSize)
{
    if (fgets(inputLine, lineSize, stdin) == NULL)
    {
        return 0;
    }

    {
        int length;

        length = (int) strlen(inputLine);

        /* fgets leaves the newline on there so get rid of it */
        if (length > 0 && inputLine[length - 1] == '\n')
        {
            inputLine[length - 1] = '\0';
        }
    }

    return 1;
}

/* checks the guess has the right number of digits and no letters */
int parseGuessDigits(char inputLine[], int guessDigits[], int digitCount)
{
    int length;
    int i;

    length = (int) strlen(inputLine);

    if (length != digitCount)
    {
        return 0; /* wrong length, reject it */
    }

    for (i = 0; i < digitCount; i++)
    {
        if (!isdigit((unsigned char) inputLine[i]))
        {
            return 0; /* not a number, reject it */
        }

        guessDigits[i] = inputLine[i] - '0';
    }

    return 1;
}

/* this figures out how many digits match exactly and how many just
 *    match but are in the wrong spot. i used two count arrays so that
 *       repeated digits dont get counted twice, took me a while to get
 *          this part right lol */
void compareGuess(int guessDigits[], int secretDigits[], int digitCount,
                   int *correctPosition, int *correctDigitWrongPosition)
{
    int secretDigitCount[RANGE];
    int guessDigitCount[RANGE];
    int i;
    int digitValue;
    int minCount;

    *correctPosition = 0;
    *correctDigitWrongPosition = 0;

    /* reset the counters */
    for (i = 0; i < RANGE; i++)
    {
        secretDigitCount[i] = 0;
        guessDigitCount[i] = 0;
    }

    /* first pass, check exact matches. anything that doesnt match
 *        exactly gets counted so we can check it in the next part */
    for (i = 0; i < digitCount; i++)
    {
        if (guessDigits[i] == secretDigits[i])
        {
            (*correctPosition)++;
        }
        else
        {
            secretDigitCount[secretDigits[i]]++;
            guessDigitCount[guessDigits[i]]++;
        }
    }

    /* second pass, see how many leftover digits still match but were
 *        just in the wrong position */
    for (digitValue = 0; digitValue < RANGE; digitValue++)
    {
        minCount = secretDigitCount[digitValue];

        if (guessDigitCount[digitValue] < minCount)
        {
            minCount = guessDigitCount[digitValue];
        }

        *correctDigitWrongPosition += minCount;
    }
}

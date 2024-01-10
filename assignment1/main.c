#include "stdio.h"

// Define states
typedef enum
{
    STATE_START,
    STATE_TRAP
} State;

// Define events
typedef enum
{
    A, // a
    B  // b
} Event;

// Function to handle state transitions
void transition(State *currentState, Event event)
{
    // Define transitions
    switch (*currentState)
    {
    case STATE_START:
        *currentState = event == A ? STATE_START : STATE_TRAP;
        break;
    case STATE_TRAP:
        break;
    }
}

int main(int argc, char const *argv[])
{
    printf("%d\n", argc);
    State currentState = STATE_START;

    char str[100];
    scanf("%s", str);

    int i = 0;
    while (str[i] != '\0')
    {
        transition(&currentState, str[i] == 'a' ? A : B);
        i++;
    }

    if (currentState == STATE_TRAP)
        printf("String not valid.\n");
    else
        printf("String is valid.\n");

    return 0;
}
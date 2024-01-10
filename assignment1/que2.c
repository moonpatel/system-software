#include "stdio.h"
#include "stdlib.h"

#define DEBUG 0
#define PRINT_LOGS 1

struct FSM
{
    size_t numStates;
    size_t numEvents;
    size_t numFinalStates;
    int currentState;
    enum CharEnum *charEnum;
    int *finalStates;
    int **transitionMatrix;
    void (*StateTransitionFunction)(struct FSM *fsm, int event);
};

// check whether string is accpeted by FSM or not.
int isAccepted(struct FSM *fsm)
{
    for (int i = 0; i < fsm->numFinalStates; i++)
    {
        if (fsm->finalStates[i] == fsm->currentState)
            return 1;
    }
    return 0;
}

void printTransitionLogs(struct FSM *fsm, int event, int newState)
{
    if (PRINT_LOGS)
    {
        if (newState == fsm->numStates - 1)
            printf("On state: %d, input: %d, go to state: trap state\n", fsm->currentState, event);
        else
            printf("On state: %d, input: %d, go to state: %d\n", fsm->currentState, event, newState);
    }
}

void genericTransitionFunction(struct FSM *fsm, int event)
{
    int newState = fsm->transitionMatrix[fsm->currentState][event];
    printTransitionLogs(fsm, event, newState);
    size_t numStates = fsm->numStates;
    size_t numEvents = fsm->numEvents;
    fsm->currentState = fsm->transitionMatrix[fsm->currentState][event];
};

struct FSM generateFSM(size_t numStates, size_t numEvents, size_t numFinalStates, int *finalStates, int **transitionMatrix)
{
    struct FSM fsm;
    fsm.currentState = 0;
    fsm.numStates = numStates;
    fsm.numEvents = numEvents;
    fsm.numFinalStates = numFinalStates;
    fsm.finalStates = finalStates;
    fsm.StateTransitionFunction = genericTransitionFunction;
    fsm.transitionMatrix = (int **)malloc((fsm.numStates) * sizeof(int *));

    for (int i = 0; i < fsm.numStates; i++)
    {
        fsm.transitionMatrix[i] = (int *)malloc(fsm.numEvents * sizeof(int));
        for (int j = 0; j < fsm.numEvents; j++)
        {
            fsm.transitionMatrix[i][j] = transitionMatrix[i][j];
        }
    }

    return fsm;
}

int main(int argc, char const *argv[])
{
    // get the transition table
    FILE *filePtr;
    filePtr = fopen("transitions.txt", "r");

    if (filePtr == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        exit(1);
    }

    /*
    numStates numEvents numFinalStates
    Array of final States...
    Transitions for state 0
    Transitions for state 1
    Transitions for state 2
    .
    .
    .
    */
    // read transition FSM parameters from transitions.file
    int numStates, numEvents, finalState, numFinalStates;
    fscanf(filePtr, "%d %d %d", &numStates, &numEvents, &numFinalStates);
    int *finalStates = (int *)malloc(numFinalStates * sizeof(int));
    int **transitionMatrix = (int **)malloc((numStates) * sizeof(int *));

    for (int i = 0; i < numFinalStates; i++)
        fscanf(filePtr, "%d", &finalStates[i]);
    for (int i = 0; i < numStates; i++)
    {
        transitionMatrix[i] = (int *)malloc(numEvents * sizeof(int));
        for (int j = 0; j < numEvents; j++)
            fscanf(filePtr, "%d", &transitionMatrix[i][j]);
    }

    // create FSM
    struct FSM fsm1 = generateFSM(numStates, numEvents, numFinalStates, finalStates, transitionMatrix);

    while (1)
    {
        printf("______________________________________________________________________\n");
        // input string
        char str[100];
        scanf("%s", str);

        if (*str == 'q')
        {
            printf("Quitting...\n");
            break;
        }

        int i = 0;
        while (str[i] != '\0')
        {
            genericTransitionFunction(&fsm1, str[i] == '0' ? 0 : 1);
            i++;
        }

        // right the program assumes there is only one final state
        // treat last state as trap state
        if (isAccepted(&fsm1))
            printf("Valid.\n");
        else
            printf("Not valid.\n");

        // reset current state of the FSM
        fsm1.currentState = 0;
    }

    fclose(filePtr);

    return 0;
}
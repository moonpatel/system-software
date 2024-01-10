#include "stdio.h"
#include "stdlib.h"

struct FSM
{
    size_t numStates;
    size_t numEvents;
    int currentState;
    enum CharEnum *charEnum;
    int **transitionMatrix;
    void (*StateTransitionFunction)(struct FSM *fsm, int event);
};

void genericTransitionFunction(struct FSM *fsm, int event)
{
    size_t numStates = fsm->numStates;
    size_t numEvents = fsm->numEvents;
    fsm->currentState = fsm->transitionMatrix[fsm->currentState][event];
};

struct FSM generateFSM(size_t numStates, size_t numEvents, int transitionMatrix[3][2])
{
    struct FSM fsm;
    fsm.currentState = 0;
    fsm.numStates = numStates;
    fsm.numEvents = numEvents;
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
    char str[100];
    scanf("%s", str);

    // a*b+
    int transitionMatrix1[3][2] = {
        {0, 1},
        {2, 1},
        {2, 2}};
    struct FSM fsm1 = generateFSM(3, 2, transitionMatrix1);

    int i = 0;
    while (str[i] != '\0')
    {
        genericTransitionFunction(&fsm1, str[i] == 'a' ? 0 : 1);
        i++;
    }

    // right the program assumes there is only one final state
    if (fsm1.currentState != 1)
        printf("Not valid.\n");
    else
        printf("Valid.\n");

    return 0;
}
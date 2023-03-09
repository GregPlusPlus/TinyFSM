#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define FSM_CREATE_STATE(s, doS, entryS, exitS, trans) do{    \
    s.doState = doS;                                        \
    s.entryState = entryS;                                  \
    s.exitState = exitS;                                    \
    s.Ntrans = sizeof(trans)/sizeof(FSM_Transition_t);      \
    s.transitions = trans;                                  \
    s.name = #s;                                            \
} while(0)

#define FSM_CREATE_TRANSITION(nextS, cond, v)   \
    {.nextState = &nextS,                        \
    .condition = cond,                          \
    .val = v,                                   \
    .condName = #cond}

typedef struct FSM_Transition_t FSM_Transition_t;

typedef struct
{
    uint32_t startT;

    void (*entryState)(void);
    void (*doState)(uint32_t);
    void (*exitState)(uint32_t);

    size_t Ntrans;
    FSM_Transition_t *transitions;

    char *name;
} FSM_State_t;

struct FSM_Transition_t {
    bool val;
    bool (*condition)(uint32_t);
    FSM_State_t *nextState;

    char *condName;
};

typedef struct {
    FSM_State_t *state;
} FSM_t;

void FSM_init(FSM_t *fsm, FSM_State_t *start, uint32_t t);
void FSM_run(FSM_t *fsm, uint32_t t);

#endif // FSM_H

#include "fsm.h"

#define FSM_LIB_DBG_LVL 0       // 0 -> None ; Levels : 1 -> Display State Entry/Exit ; 2 -> Level 1 + Display Condition results

static void FSM_trans(FSM_t *fsm, uint32_t t);
static void FSM_next(FSM_t *fsm, FSM_State_t *next, uint32_t t);
static void FSM_enterNext(FSM_t *fsm, FSM_State_t *next, uint32_t t);

void FSM_init(FSM_t *fsm, FSM_State_t *start, uint32_t t) {
    FSM_enterNext(fsm, start, t);
}

void FSM_run(FSM_t *fsm, uint32_t t) {
    if(!fsm->state) {
        return;
    }

    if(fsm->state->doState) {
            fsm->state->doState(t - fsm->state->startT);
    }

    FSM_trans(fsm, t);
}

void FSM_trans(FSM_t *fsm, uint32_t t) {
    for(size_t i = 0; i < fsm->state->Ntrans; i ++) {

        FSM_Transition_t *trans = &fsm->state->transitions[i];
        bool (*cond)(uint32_t) = trans->condition;

        FSM_State_t *nextState = NULL;

        if(!cond) {
            nextState = trans->nextState;
        } else {
            bool condRes = cond(t - fsm->state->startT);
            
#if FSM_LIB_DBG_LVL >= 2
        printf("FSM LIB ~ Condition '%s' = %d\r\n", trans->condName, condRes);
#endif

            if(condRes == trans->val) {
                nextState = trans->nextState;
            }
        }

        if(nextState) {
            FSM_next(fsm, nextState, t);

            return;
        }

    }
}

static void FSM_next(FSM_t *fsm, FSM_State_t *next, uint32_t t) {
    if(!next || !fsm->state) {
        return;
    }

#if FSM_LIB_DBG_LVL >= 1
        printf("FSM LIB ~ Exit state %s\r\n", fsm->state->name);
#endif

    if(fsm->state->exitState) {
        fsm->state->exitState(t - fsm->state->startT);
    }

    FSM_enterNext(fsm, next, t);
}

void FSM_enterNext(FSM_t *fsm, FSM_State_t *next, uint32_t t) {
    fsm->state = next;
    
    fsm->state->startT = t;

#if FSM_LIB_DBG_LVL >= 1
        printf("FSM LIB ~ Enter state %s\r\n", fsm->state->name);
#endif

    if(fsm->state->entryState) {
        fsm->state->entryState();
    }
}

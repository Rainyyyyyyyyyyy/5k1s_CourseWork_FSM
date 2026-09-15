#include "FSM.h"


int main(){
    FSM fsm(1, {0, 1, 2, 3}, {1, 2, 3, 0}, {0, 1, 0, 1}, {1, 0, 1, 0});
    fsm.output();
    //fsm.ProcessWord({0, 1, 0, 1});
    

    return 0;
}
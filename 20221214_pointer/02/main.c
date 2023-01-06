#include "stdio.h"
#include "stdlib.h"

#define PLAYER_COUNT 10

typedef int (*Func)(int, double);

int main(){
    int *players = calloc(PLAYER_COUNT,sizeof(int) );
    
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        players[i] = i;
    }
    free(players);
    
    return 0;
}

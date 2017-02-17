#include <stdio.h>

struct stones{
    char body;
    int x;
    int y;
};

struct brick{
    int x;
    int y;
    struct stones *stones;
};

struct bricks{
    struct brick *brick;
};

void initBricks(struct bricks *bricks){
    int stX=3;
    int stY=10;

    struct stones stones[]={
        {.body='X', .x=stX, .y=stY},
        {.body='O', .x=stX+1, .y=stY},
        {.body='X', .x=stX+2, .y=stY},
        {.body='o', .x=stX+3, .y=stY}
    };

    struct brick brick1={
        .stones=stones
    };

    bricks[0].brick=&brick1;
};

int main(void){
    struct bricks bricks[4];
    initBricks(bricks);

    printf("test: %c\n", bricks[0].brick[0].stones[0].body);
    return 0;
}

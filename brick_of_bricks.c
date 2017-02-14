#include <stdio.h>

struct brick{
    char body[20];
    int x;
};

struct bricks{
    struct brick *brick;
};

void printBricks(struct bricks *bricks){
    printf("%s\n\n",bricks[0].brick[0].body);
    printf("%s\n\n",bricks[0].brick[1].body);
    printf("%s\n\n",bricks[0].brick[2].body);
    printf("%s\n\n",bricks[0].brick[3].body);
}

int main(void){
    
    struct brick brick1[]={
        {.body="@\n@\n@@", .x=0},
        {.body="  @\n@@@", .x=1},
        {.body="@\n@@@", .x=1},
        {.body="@@@\n  @", .x=1}
    }; 
    struct brick brick2[]={
        {.body="****", .x=0},
        {.body="----", .x=1}
    }; 

    struct bricks bricks[]={
        {.brick=brick1},
        {.brick=brick2}
    };

    printBricks(bricks);

    //printf("%s\n",bricks[0].brick[1].body);


    //printBricks(bricks);
    return 0;
}

#include <stdio.h>

struct stone{
    int x;
    int y;
};

typedef struct brick{
    int x;
    int y;
    char body;
    struct stone stn[4];
}brick;

typedef struct bricks{
    struct brick brick[2];
}b;

void initBricks(b *bricksP){
    
    int sX=3;
    int sY=10;

    brick br1={
        .body='X',
        .x=sX,
        .y=sY,
        .stn[0].x=0,
        .stn[0].y=0,
        .stn[1].x=1,
        .stn[1].y=0,
        .stn[2].x=2,
        .stn[2].y=0,
        .stn[3].x=3,
        .stn[3].y=0,
    };

    b bricks={
        .brick[0]=br1,
        //.brick[1]=br2
    };

    *bricksP=bricks;
}

int main(void){

    b b;
    initBricks(&b);

    printf("test0: x=%i\n",b.brick[0].x);

    while(1){
      printf("br1, stone1: x=%i, y=%i. :: %c\n",(b.brick[0].x + b.brick[0].stn[1].x), 
                                                (b.brick[0].y + b.brick[0].stn[1].y), 
                                                b.brick[0].body 
                                                );
      b.brick[0].y++;
        sleep(1);
    }
}

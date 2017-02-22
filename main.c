#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#define DELAY1 20000


void drawStage(int w, int h, _Bool intro);

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

struct bricks{
    struct brick brick[2];
};

void initBricks(struct bricks *bricksP){

    int sX=7;
    int sY=2;

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

    struct bricks brcks={
        .brick[0]=br1,
        //.brick[1]=br2
    };

    *bricksP=brcks;
}

void moveBrick(struct bricks *bricksP){
    
}

void printObjs(struct bricks *bricksP){
    //drawStage(35, 20, 0);
    int i;
    for(i=0;i<4;i++){
        int y = bricksP[0].brick[0].y + bricksP[0].brick[0].stn[i].y;
        int x = bricksP[0].brick[0].x + bricksP[0].brick[0].stn[i].x;
        mvprintw(y, x, "%c", bricksP[0].brick[0].body);
    }

    refresh();
}

int main(void){

    drawStage(35, 20, 1);
    
    struct bricks b;
    initBricks(&b);

    while(1){
        printObjs(&b);
        //usleep(800000);
        sleep(1);
    }

    endwin();

    return 0;
}

void drawStage(int w, int h, _Bool intro){
    int delay;
    int i;

    if(intro){
        delay=10000;
        //Ncurses init
        initscr();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(FALSE);
        timeout(0); 
    }else{
        delay=0;
    }

    clear();

    //draw labels
    mvprintw(0, (w/2), "NtetreX");
    int labX=((w + (w / 8)));
    mvprintw(0, labX, "Level: %d",0);
    mvprintw(2, labX, "Score: %d",0);

    //draw stage bottom
    int j;
    i=h;
    for(j=(w + (w / 2));j>=0;j-=3){
        mvprintw(((i - h) + 1), j, "---");
        mvprintw(i, j, "OOO");
        refresh();
    }
    mvprintw(((i - h) + 1), 0, "---");
    mvprintw(i, 0, "OOO");

    //draw stage walls
    for(i=0;i<h;i++){
        mvprintw(i, 0, "|X|");
        mvprintw(i, w, "|X|");
        mvprintw(i, (w + (w / 2)), "|X|");
        usleep(delay);
        refresh();
    }

    refresh();
}


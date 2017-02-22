#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#define DELAY1 20000


void drawStage(int w, int h, _Bool intro);

struct stone{
    float x;
    float y;
};

typedef struct brick{
    float x;
    float y;
    char body;
    struct stone stn[4];
}brick;

typedef struct bricks{
    struct brick brick[2];
}brcks;

void initBricks(brcks *bP){

    float sX=7.0;
    float sY=2.0;

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

    brcks brcks={
        .brick[0]=br1,
        //.brick[1]=br2
    };

    *bP=brcks;
}

void printBrick(brcks *bP){
    int i;
    for(i=0;i<4;i++){
        int y = bP[0].brick[0].y + bP[0].brick[0].stn[i].y;
        int x = bP[0].brick[0].x + bP[0].brick[0].stn[i].x;
        mvprintw(y, x, "%c", bP[0].brick[0].body);
    }
    refresh();
}

void gravBrick(brcks *bP){
    int i;
    for(i=0;i<4;i++){
        int y = bP[0].brick[0].y + bP[0].brick[0].stn[i].y;
        int x = bP[0].brick[0].x + bP[0].brick[0].stn[i].x;
        mvprintw(y, x, " ");
    }
    bP[0].brick[0].y+=0.05;
}

void moveBrick(brcks *bP, int mv){
    if(mv == KEY_UP){
        //rotate
        int i;
        if(bP[0].brick[0].stn[1].x != 0){
            for(i=0;i<4;i++){
                bP[0].brick[0].stn[i].y=i;
                bP[0].brick[0].stn[i].x=0;
            }
        }else{
            for(i=0;i<4;i++){
                bP[0].brick[0].stn[i].y=0;
                bP[0].brick[0].stn[i].x=i;
            }
        }
    }else if(mv == KEY_DOWN){
        bP[0].brick[0].y++;
    }else if(mv == KEY_LEFT){
        bP[0].brick[0].x--;
    }else if(mv == KEY_RIGHT){
        bP[0].brick[0].x++;
    }else{
        
    }   
    printBrick(bP);
}

int main(void){
    drawStage(35, 20, 1);
    
    brcks b;
    initBricks(&b);

    while(1){
        timeout(0);
        int mv = getch();
        moveBrick(&b, mv);

        printBrick(&b);
        gravBrick(&b);
        //int mv = 1;
        usleep(30000);
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


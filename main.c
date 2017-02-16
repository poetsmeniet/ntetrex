#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#define DELAY1 20000

struct brick{
    char body[20];
    int x;
    int y;
};

struct bricks{
    struct brick *brick;
};

void drawStage(int w, int h, _Bool intro);


void initBricks(struct bricks *bricks){
    struct brick brick1[]={
        {.body="o\no\noo", .x=10, .y=3},
        {.body="  ◻\n◻◻◻", .x=10},
        {.body="◻◻\n ◻\n ◻", .x=10},
        {.body="◻◻◻\n◻", .x=10}
    };
    struct brick brick2[]={
        {.body="◻◻◻◻", .x=0},
        {.body="◻\n◻\n◻\n◻", .x=1}
    };

    bricks[0].brick=brick1;
    bricks[1].brick=brick2;
}

void moveBrick(struct bricks *bricks){
    
}

void printObjs(struct bricks *bricks){
    //mvprintw(10,10,bricks->brick[0].body);
    drawStage(35, 20, 0);
    mvprintw(10,10,"thomas\n");
    refresh();
    //drawStage(35, 20, 0);
}

int main(void){

    drawStage(35, 20, 1);
    
    struct bricks bricks;
    initBricks(&bricks);


    while(1){
        printObjs(&bricks);

        usleep(800000);
    }

    endwin();

    return 0;
}

void drawStage(int w, int h, _Bool intro){
    int delay;
    int i;

    if(intro){
        delay=40000;
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


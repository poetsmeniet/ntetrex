#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#define DELAY1 20000

void drawStage(int w, int h, _Bool intro);

struct brick{
    char body[20];
};

struct bricks{
    struct brick brick;

void placeBrick(){
    struct brick brick1[2]={
        {.body="◻◻◻◻"},
        {.body="test"}
    };

    //mvprintw(4,8,brick1[1].body);
    mvprintw(4,8,"tjek");
}

int main(void){

    drawStage(35, 20, 1);
    placeBrick();

    sleep(200);
    endwin();

    return 0;
}

void drawStage(int w, int h, _Bool intro){
    int delay;
    int i;

    if(intro)
        delay=40000;
    else
        delay=0;

    //Ncurses init
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    timeout(0); 
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

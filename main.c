#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#define DELAY1 20000

void drawStage(int w, int h, _Bool intro);

int main(void){

    //getmaxyx(stdscr, max_y, max_x);

    drawStage(35, 20, 1);

    sleep(2);
    endwin();

    return 0;
}

void drawStage(int w, int h, _Bool intro){
    int delay;
    int i;

    if(intro)
        delay=30000;
    else
        delay=0;

    //Ncurses init
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    timeout(0); 
    
    clear();

    //draw stage walls
    for(i=0;i<h;i++){
        mvprintw(i, 0, "|X|");
        mvprintw(i, w, "|X|");
        mvprintw(i, (w + (w / 2)), "|X|");
        usleep(delay);
        refresh();
    }

    //draw stage bottom
    int j;
    for(j=(w + (w / 2));j>=0;j-=3){
        mvprintw(((i - h) + 1), j, "---");
        mvprintw(i, j, "OOO");
        refresh();
    }
    mvprintw(((i - h) + 1), 0, "---");
    mvprintw(i, 0, "OOO");

    usleep(delay);
    
    char label[]="NtetreX";
    mvprintw(0, (w/2), label);
    mvprintw(0, (w + (w / 2)) - 10, "Score %d",((w - (w / 2))/2) );

    refresh();
}

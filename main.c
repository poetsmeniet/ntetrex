#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#define DELAY1 40000

struct stone{
    float x;
    float y;
};

typedef struct brick{
    float x;
    float y;
    int id;
    char body;
    struct stone stn[4];
}brick;

typedef struct bricks{
    int curBr;
    int score;
    struct brick brick[4];
}brcks;

void drawStage(int w, int h, _Bool intro, brcks *bP);
void moveBrick(brcks *bP, int mv);
void initBricks(brcks *bP);
void printBrick(brcks *bP);
void gravBrick(brcks *bP);
void detectLine(int width, int height, brcks *bP);
void drawLabels(int w, int h, brcks *bP);

int main(void){
    brcks b;
    initBricks(&b);

    int width = 15;
    int height = 20;
    drawStage(width, height, 1, &b);

    while(1){
        timeout(0);
        int mv = getch();
        moveBrick(&b, mv);
        printBrick(&b);
        gravBrick(&b);
        detectLine(width, height, &b);
        drawLabels(width, height, &b);
        usleep(30000);
    }

    endwin();

    return 0;
}

void drawLabels(int w, int h, brcks *bP){
    //draw labels
    mvprintw(0, (w/2)-1, "NtetreX");
    int labX = ((w + (w / 8)));
    mvprintw(0, labX+2, " %d",bP[0].score);
}

void drawStage(int w, int h, _Bool intro, brcks *bP){
    int delay;
    int i;

    if(intro){
        delay = 10000;
        initscr();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(FALSE);
        timeout(0); 
    }else{
        delay = 0;
    }

    clear();

    //draw labels
    mvprintw(0, (w/2)-1, "NtetreX");
    int labX = ((w + (w / 8)));
    mvprintw(0, labX+2, " %d",bP[0].score);

    //draw stage bottom
    int j;
    i = h;
    for(j = (w + (w / 2)-2);j >= 0;j -= 3){
        mvprintw(((i - h) + 1), j, "---");
        mvprintw(i, j, "OOO");
        mvprintw(i+1, j, "OOO");
        refresh();
    }
    //mvprintw(((i - h) + 1), 0, "");
    mvprintw(i, 0, "OOO");
    mvprintw(i+1, 0, "OOO");

    //draw stage walls
    for(i = 0;i < h;i++){
        mvprintw(i, 0, "|X|");
        mvprintw(i, w, "|X|");
        mvprintw(i, (w + (w / 2)), "|");
        usleep(delay);
        refresh();
    }

    refresh();
}

void initBricks(brcks *bP){
    float sX = 7.0;
    float sY = 2.0;

    brick br1 = {
        .id = 0,
        .body = 'O',
        .x = sX,
        .y = sY,
        .stn[0].x = 0,
        .stn[0].y = 0,
        .stn[1].x = 1,
        .stn[1].y = 0,
        .stn[2].x = 2,
        .stn[2].y = 0,
        .stn[3].x = 3,
        .stn[3].y = 0,
    };

    brick br2 = {
        .id = 1,
        .body = 'O',
        .x = sX,
        .y = sY,
        .stn[0].x = 0,
        .stn[0].y = 0,
        .stn[1].x = 0,
        .stn[1].y = 1,
        .stn[2].x = 1,
        .stn[2].y = 0,
        .stn[3].x = 1,
        .stn[3].y = 1,
    };

    brick br3 = {
        .id = 2,
        .body = 'O',
        .x = sX,
        .y = sY,
        .stn[0].x = 0,
        .stn[0].y = 0,
        .stn[1].x = 1,
        .stn[1].y = 0,
        .stn[2].x = 1,
        .stn[2].y = 1,
        .stn[3].x = 1,
        .stn[3].y = 2,
    };

    brcks brcks = {
        .curBr = 0,
        .score = 0,
        .brick[0] = br1,
        .brick[1] = br2,
        .brick[2] = br3
    };

    *bP = brcks;
}

void printBrick(brcks *bP){
    int i;
    int cB = bP->curBr;
    for(i = 0;i < 4;i++){
        int y = bP[0].brick[cB].y + bP[0].brick[cB].stn[i].y;
        int x = bP[0].brick[cB].x + bP[0].brick[cB].stn[i].x;
        mvprintw(y, x, "%c", bP[0].brick[cB].body);
    }
    refresh();
}

void gravBrick(brcks *bP){
    //detect colision with floor & other bricks
    int i;
    int col = 0;
    int cB = bP->curBr;
    for(i = 0;i < 4;i++){
        float nY = (bP[0].brick[cB].y + bP[0].brick[cB].stn[i].y) + 1;
        float nX = (bP[0].brick[cB].x + bP[0].brick[cB].stn[i].x);
        if(mvinch(nY,nX) == 'O' && \
                nY !=  (bP[0].brick[cB].y + bP[0].brick[cB].stn[i+1].y) && \
                nY !=  (bP[0].brick[cB].y + bP[0].brick[cB].stn[i-1].y) \
                )
            col++;
    }
    //move brick
    if(col == 0){
        int i;
        for(i = 0;i < 4;i++){
            int y = bP[0].brick[cB].y + bP[0].brick[cB].stn[i].y;
            int x = bP[0].brick[cB].x + bP[0].brick[cB].stn[i].x;
            mvprintw(y, x, " ");
        }
        bP[0].brick[cB].y += 0.05;
    }else{
        //respawn
        bP->curBr = (cB + 1) % 3;
        bP[0].brick[cB].y = 2;
        bP[0].brick[cB].x = 7;
    }
}

void moveBrick(brcks *bP, int mv){
    //vertica colisions
    int i;
    int colR = 0;
    int colL = 0;
    int cB = bP->curBr;
    for(i = 0;i < 4;i++){
        float nY = (bP[0].brick[cB].y + bP[0].brick[cB].stn[i].y);
        float nX = (bP[0].brick[cB].x + bP[0].brick[cB].stn[i].x) + 1;
        if(mvinch(nY,nX) == '|' || mvinch(nY,nX) == 'O')
            colR++;

        nY = (bP[0].brick[cB].y + bP[0].brick[cB].stn[i].y);
        nX = (bP[0].brick[cB].x + bP[0].brick[cB].stn[i].x) - 1;
        if(mvinch(nY,nX) == '|' || mvinch(nY,nX) == 'O')
            colL++;
    }

    //movement
    if(mv == KEY_UP){
        //rotate
        int i;
        switch(bP[0].brick[cB].id){
            case 0:
                if(bP[0].brick[cB].stn[1].x !=  0){
                    for(i = 0;i < 4;i++){
                        bP[0].brick[cB].stn[i].x -=  i;
                        bP[0].brick[cB].stn[i].y +=  i;
                    }
                }else{
                    for(i = 0;i < 4;i++){
                        bP[0].brick[cB].stn[i].x +=  i;
                        bP[0].brick[cB].stn[i].y -=  i;
                    }
                }
                break;
        case 2:
            //turn ccw A->B
            if(bP[0].brick[cB].stn[0].x == 0 && bP[0].brick[cB].stn[0].y == 0){
                bP[0].brick[cB].stn[0].y += 1;
                bP[0].brick[cB].stn[1].x -= 1;
                bP[0].brick[cB].stn[2].y -= 1;
                bP[0].brick[cB].stn[3].x += 1;
                bP[0].brick[cB].stn[3].y -= 2;
                mvprintw(23,3,"hmm: A-B");
            }else if(bP[0].brick[cB].stn[0].x == 0 && bP[0].brick[cB].stn[0].y == 1){
                //turn ccw B->C
                bP[0].brick[cB].stn[0].x += 1;
                bP[0].brick[cB].stn[0].y += 1;
                bP[0].brick[cB].stn[1].y += 2;
                bP[0].brick[cB].stn[2].x -= 1;
                bP[0].brick[cB].stn[2].y += 1;
                bP[0].brick[cB].stn[3].x -= 2;
                mvprintw(23,3,"hmm: B-C");
            }else if(bP[0].brick[cB].stn[0].x == 1 && bP[0].brick[cB].stn[0].y == 2){
                //turn ccw C->D
                bP[0].brick[cB].stn[0].x += 1;
                bP[0].brick[cB].stn[0].y -= 2;
                bP[0].brick[cB].stn[1].x += 2;
                bP[0].brick[cB].stn[1].y -= 1;
                bP[0].brick[cB].stn[2].x += 1;
                bP[0].brick[cB].stn[3].y += 1;
                mvprintw(23,3,"hmm: C-D");
            }else if(bP[0].brick[cB].stn[0].x == 2 && bP[0].brick[cB].stn[0].y == 0){
                //turn ccw D->A
                bP[0].brick[cB].stn[0].x -= 2;
                bP[0].brick[cB].stn[1].x -= 1;
                bP[0].brick[cB].stn[1].y -= 1;
                bP[0].brick[cB].stn[3].x += 1;
                bP[0].brick[cB].stn[3].y += 1;
                mvprintw(23,3,"hmm: D-A");
            }
            break;
        default:
            break;
        }
    }else if(mv == KEY_DOWN){
        bP[0].brick[cB].y++;
    }else if(mv == KEY_LEFT && colL == 0){
        bP[0].brick[cB].x--;
    }else if(mv == KEY_RIGHT && colR == 0){
        bP[0].brick[cB].x++;
    }else{
        
    }   

    printBrick(bP);
}

void detectLine(int width, int height, brcks *bP){
    int i;
    int j = 0;
    int cnt = 0;
    for(i = 0;i <= width;i++){
        if(mvinch(j, i) == 'O')
            cnt++;

        if(i >=  width){
            if(cnt == 11){
                //change chars
                mvprintw(j, 0, "|X|____________|X|    |");
                refresh();
                usleep(DELAY1);
                mvprintw(j, 0, "|X|xxxxxxxxxxxx|X|    |");
                refresh();
                usleep(DELAY1);
                mvprintw(j, 0, "|X|XXXXXXXXXXXX|X|    |");
                refresh();
                usleep(DELAY1);
                mvprintw(j, 0, "|X|XXXX    XXXX|X|    |");
                refresh();
                usleep(DELAY1);
                mvprintw(j, 0, "|X|XX        XX|X|    |");
                refresh();
                usleep(DELAY1);
                mvprintw(j, 0, "|X|X          X|X|    |");
                refresh();
                usleep(DELAY1);

                //delete line
                move(j, 0);
                deleteln();

                //add line at top
                move(2, 0);
                insertln();
                mvprintw(2, 0, "|X|            |X|    |");

                //increase score
                bP->score++;
            }
            i = 3;
            j++;
            cnt = 0;
        }
        if(j == height)
            break;
    }
}

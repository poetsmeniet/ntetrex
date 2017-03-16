#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
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
    int tjek;
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

void printBrick(brcks *bP){
    int i=1;
    int cB = bP->curBr;

    //print brick, stone coords are relative to each other
    int x, y;
    for(i = 0;i < 4;i++){
        y = bP[0].brick[cB].y + bP[0].brick[cB].stn[0].y + bP[0].brick[cB].stn[i].y;
        x = bP[0].brick[cB].x + bP[0].brick[cB].stn[0].x + bP[0].brick[cB].stn[i].x;
        mvprintw(y, x, "%c", bP[0].brick[cB].body);
    }
    refresh();
}

void gravBrick(brcks *bP){
    int i, j;
    int col = 0;
    int cB = bP->curBr;
    int tjek=0;
    int nY;
    int cX;
    for(i = 0;i < 4;i++){
        nY = (bP[0].brick[cB].y + bP[0].brick[cB].stn[0].y + bP[0].brick[cB].stn[i].y) + 1;
        cX = (bP[0].brick[cB].x + bP[0].brick[cB].stn[0].x + bP[0].brick[cB].stn[i].x);

        int stoneY;
        int stoneX;

        for(j = 0;j < 4;j++){
            stoneY = bP[0].brick[cB].y + bP[0].brick[cB].stn[0].y + bP[0].brick[cB].stn[j].y;
            stoneX = bP[0].brick[cB].x + bP[0].brick[cB].stn[0].x + bP[0].brick[cB].stn[j].x;
            
            //detect collision
            if(nY == stoneY+1 && cX == stoneX && mvinch(nY, cX) != 'O')
                tjek++;
        }
    }
    
    if(tjek < bP->tjek)
        col++;

    bP->tjek = tjek;

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
        initBricks(bP);

        //determine "random" x for stone index 1
        time_t t;
        srand((unsigned) time(&t));
        int r = rand() % 5;
        r=1;
        switch(r){
            case 0:
                bP[0].brick[cB].stn[2].y -= 2;
                break;
            case 1:
                bP[0].brick[cB].stn[1].x = 0;
                bP[0].brick[cB].stn[1].y = -1;
                bP[0].brick[cB].stn[2].x = 0;
                bP[0].brick[cB].stn[2].y = 1;
                bP[0].brick[cB].stn[3].x = 0;
                bP[0].brick[cB].stn[3].y = 2;
                break;
            case 2:
                bP[0].brick[cB].stn[1].x = -1;
                bP[0].brick[cB].stn[1].y = 0;
                bP[0].brick[cB].stn[2].x = 0;
                bP[0].brick[cB].stn[2].y = 1;
                bP[0].brick[cB].stn[3].x = 1;
                bP[0].brick[cB].stn[3].y = 1;
                break;
            case 3:
                bP[0].brick[cB].stn[3].x -= 1;
                bP[0].brick[cB].stn[3].y += 1;
                break;
            case 4:
                bP[0].brick[cB].stn[1].x = 0;
                bP[0].brick[cB].stn[1].y = -1;
                bP[0].brick[cB].stn[2].x = 1;
                bP[0].brick[cB].stn[2].y = 0;
                bP[0].brick[cB].stn[3].x = 0;
                bP[0].brick[cB].stn[3].y = 1;
                break;
            case 5:
                break;
            default:
                break;
        }
    }

    if(bP[0].brick[cB].x > 22){
        mvprintw((bP[0].brick[cB].y + 1), bP[0].brick[cB].x, "I'm FREE!");
        refresh();
        sleep(1);
    	drawStage(15, 20, 1, bP);
        bP->score += 4;
        bP->curBr = rand() % 3;
        bP[0].brick[cB].y = 2;
        bP[0].brick[cB].x = 7;
    }
}

void drawLabels(int w, int h, brcks *bP){
    //draw labels
    mvprintw(0, (w/2)-1, "NtetreX");
    int labX = ((w + (w / 8)));
    mvprintw(0, labX+2, " %d",bP[0].score);
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
        if(mvinch(nY, nX) == '|' || mvinch(nY, nX) == 'O')
            colR++;

        nY = (bP[0].brick[cB].y + bP[0].brick[cB].stn[i].y);
        nX = (bP[0].brick[cB].x + bP[0].brick[cB].stn[i].x) - 1;
        if(mvinch(nY, nX) == '|' || mvinch(nY, nX) == 'O')
            colL++;
    }

    //rotate
    if(mv == KEY_UP){
        for(i = 1;i < 4;i++){
            float cX = bP[0].brick[cB].stn[i].x;
            float cY = bP[0].brick[cB].stn[i].y;
            bP[0].brick[cB].stn[i].x = cY;
            bP[0].brick[cB].stn[i].y = (cX * -1);
        }
        bP->tjek = 0;

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
        .stn[2].x = 0,
        .stn[2].y = 1,
        .stn[3].x = 1,
        .stn[3].y = 1,
    };

    brcks brcks = {
        .curBr = 0,
        .tjek = 0,
        .score = bP->score,
        .brick[0] = br1,
    };

    *bP = brcks;
}

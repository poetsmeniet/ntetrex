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
    float speed;
    struct brick br[4];
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
    b.speed = 0.05;

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
        usleep(DELAY1);
    }

    endwin();

    return 0;
}

void printBrick(brcks *bP){
    int i=1;
    int cB = bP->curBr;

    //Print brick, stone coords are relative to 0
    int x, y;
    for(i = 0;i < 4;i++){
        y = bP[0].br[cB].y + bP[0].br[cB].stn[0].y + bP[0].br[cB].stn[i].y;
        x = bP[0].br[cB].x + bP[0].br[cB].stn[0].x + bP[0].br[cB].stn[i].x;
        mvaddch(y, x, ACS_CKBOARD);
    }
    refresh();
}

void gravBrick(brcks *bP){
    int i, j, nY, cX;
    int col = 0;
    int cB = bP->curBr;
    int tjek = 0;
    int lnY=0; //largest next Y
    for(i = 0;i < 4;i++){
        nY = (bP[0].br[cB].y + bP[0].br[cB].stn[0].y + bP[0].br[cB].stn[i].y) + 1;
        if(nY > lnY)
            lnY = nY;

        cX = (bP[0].br[cB].x + bP[0].br[cB].stn[0].x + bP[0].br[cB].stn[i].x);

        int stoneY;
        int stoneX;

        for(j = 0;j < 4;j++){
            stoneY = bP[0].br[cB].y + bP[0].br[cB].stn[0].y + bP[0].br[cB].stn[j].y;
            stoneX = bP[0].br[cB].x + bP[0].br[cB].stn[0].x + bP[0].br[cB].stn[j].x;
            
            //Detect collision that is not brick
            if(nY == stoneY+1 && cX == stoneX && mvinch(nY, cX) != ACS_CKBOARD)
                tjek++;
        }
    }

    //detect game end
    if(mvinch(lnY, cX) == ACS_CKBOARD && nY < 6){
        mvprintw(20,1,"Game end, well done!!");
        refresh();
        sleep(500);
        exit(0);
    }
    
    //Colision detected
    if(tjek < bP->tjek || nY >= 21)
        col++;
     
    bP->tjek = tjek;

    //Move brick
    if(col == 0){
        int i;
        for(i = 0;i < 4;i++){
            int y = bP[0].br[cB].y + bP[0].br[cB].stn[i].y;
            int x = bP[0].br[cB].x + bP[0].br[cB].stn[i].x;
            mvprintw(y, x, " ");
        }
        bP[0].br[cB].y += bP->speed;
    }else{
        initBricks(bP);

        //Determine "random" x for stone index 1
        time_t t;
        srand((unsigned) time(&t));
        int r = rand() % 5;
        switch(r){
            case 0:
                bP[0].br[cB].stn[2].y -= 2;
                break;
            case 1:
                bP[0].br[cB].stn[1].x = 0;
                bP[0].br[cB].stn[1].y = -1;
                bP[0].br[cB].stn[2].x = 0;
                bP[0].br[cB].stn[2].y = 1;
                bP[0].br[cB].stn[3].x = 0;
                bP[0].br[cB].stn[3].y = 2;
                break;
            case 2:
                bP[0].br[cB].stn[1].x = -1;
                bP[0].br[cB].stn[1].y = 0;
                bP[0].br[cB].stn[2].x = 0;
                bP[0].br[cB].stn[2].y = 1;
                bP[0].br[cB].stn[3].x = 1;
                bP[0].br[cB].stn[3].y = 1;
                break;
            case 3:
                bP[0].br[cB].stn[3].x -= 1;
                bP[0].br[cB].stn[3].y += 1;
                break;
            case 4:
                bP[0].br[cB].stn[1].x = 0;
                bP[0].br[cB].stn[1].y = -1;
                bP[0].br[cB].stn[2].x = 1;
                bP[0].br[cB].stn[2].y = 0;
                bP[0].br[cB].stn[3].x = 0;
                bP[0].br[cB].stn[3].y = 1;
                break;
            case 5:
                break;
            default:
                break;
        }
    }
}

void drawLabels(int w, int h, brcks *bP){
    mvprintw(0, (w/2)-1, "NtetreX");
    int labX = ((w + (w / 8)));
    mvprintw(0, labX+2, " %d",bP[0].score);
}

void moveBrick(brcks *bP, int mv){
    //Vertica colisions
    int i;
    int colR = 0;
    int colL = 0;
    int cB = bP->curBr;
    for(i = 0;i < 4;i++){
        float nY = (bP[0].br[cB].y + bP[0].br[cB].stn[i].y);
        float nX = (bP[0].br[cB].x + bP[0].br[cB].stn[i].x) + 1;
        if(mvinch(nY, nX) == '|' || mvinch(nY, nX) == ACS_CKBOARD)
            colR++;

        nY = (bP[0].br[cB].y + bP[0].br[cB].stn[i].y);
        nX = (bP[0].br[cB].x + bP[0].br[cB].stn[i].x) - 1;
        if(mvinch(nY, nX) == '|' || mvinch(nY, nX) == ACS_CKBOARD)
            colL++;
    }

    //Detect wall colision on rotate
    int vrcol = 0;
    int pMx1 = (bP[0].br[cB].x - bP[0].br[cB].stn[1].y );
    int pPx1 = (bP[0].br[cB].x - bP[0].br[cB].stn[3].y );
    int pMx2 = (bP[0].br[cB].x + bP[0].br[cB].stn[1].y );
    int pPx2 = (bP[0].br[cB].x + bP[0].br[cB].stn[3].y );

    if(mvinch(bP[0].br[cB].y, pMx1) != ' ' || \
            mvinch(bP[0].br[cB].y, pPx1) != ' ')
            vrcol = 1;

    if(mvinch(bP[0].br[cB].y, pMx2) != ' ' || \
            mvinch(bP[0].br[cB].y, pPx2) != ' ')
            vrcol = 1;
    
    //Apply rotation matrix, 90d cc
    if(mv == KEY_UP && vrcol == 0){
        for(i = 1;i < 4;i++){
            float cX = bP[0].br[cB].stn[i].x;
            float cY = bP[0].br[cB].stn[i].y;
            bP[0].br[cB].stn[i].x = cY;
            bP[0].br[cB].stn[i].y = (cX * -1);
        }
        bP->tjek = 0;

    }else if(mv == KEY_DOWN && bP[0].br[cB].y > 3){
        bP[0].br[cB].y++;
    }else if(mv == KEY_LEFT && colL == 0){
        bP[0].br[cB].x--;
    }else if(mv == KEY_RIGHT && colR == 0){
        bP[0].br[cB].x++;
    }else{
        //Done
    }   

    printBrick(bP);
}

void detectLine(int width, int height, brcks *bP){
    int i;
    int j = 0;
    int cnt = 0;
    for(i = 0;i <= width;i++){
        if(mvinch(j, i) == ACS_CKBOARD)
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

                //increase speed
                bP->speed += 0.005;
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

        int x, y, c;
        c=0;y=10;x=5;
        start_color();
        init_pair (c, x, y);
        attrset (COLOR_PAIR(c));

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
        mvaddch(i, j, ACS_CKBOARD);
        mvaddch(i, j+1, ACS_CKBOARD);
        mvaddch(i, j+2, ACS_CKBOARD);
        mvprintw(i+1, j, "OOO");
        refresh();
    }
    mvaddch(i, 0, ACS_CKBOARD);
    mvaddch(i, 1, ACS_CKBOARD);
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
        .speed = bP->speed,
        .br[0] = br1,
    };

    *bP = brcks;
}

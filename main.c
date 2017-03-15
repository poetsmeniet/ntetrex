#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
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
    float tjek;
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
        //usleep(300000);
    }

    endwin();

    return 0;
}

void printBrick(brcks *bP){
    int i=1;
    int cB = bP->curBr;

    //print bricks' stones
    int x, y;
    for(i = 0;i < 4;i++){
        y = bP[0].brick[cB].stn[i].y;
        x = bP[0].brick[cB].stn[i].x;
        mvprintw(y, x, "%c", bP[0].brick[cB].body);
    }
    mvprintw(3, 25, "%f @@ %f", bP[0].brick[cB].stn[0].x, bP[0].brick[cB].stn[0].y);
    refresh();
}

void gravBrick(brcks *bP){
    int i, j;
    int col = 0;
    int cB = bP->curBr;
    //get highest y 

    int hYo = 0;
    for(i = 0;i < 4;i++){
        if( bP[0].brick[cB].stn[i].y > hYo)
            hYo = bP[0].brick[cB].stn[i].y;
    }
    
    int cnt;
    float tjek=0;
    float nY;
    float cX;
    for(i = 0;i < 4;i++){
        nY = (bP[0].brick[cB].stn[i].y) + 1;
        cX = (bP[0].brick[cB].stn[i].x);

        float stoneY;
        float stoneX;

        for(j = 0;j < 4;j++){
            stoneY = bP[0].brick[cB].stn[j].y;
            stoneX = bP[0].brick[cB].stn[j].x;
            
            //detect collision
            if(nY == stoneY + 1 && cX == stoneX && mvinch(nY, cX) != 'O')
                tjek += 1;
        }
    }
    cnt=nY;

    if(tjek != bP->tjek && cnt > 8)
        //col++;

    bP->tjek = tjek;

    //testing
    //if(cnt >=20)
        //col++;

    //move brick
    if(col == 0){
        int i;
        for(i = 0;i < 4;i++){
            int y = bP[0].brick[cB].stn[i].y;
            int x = bP[0].brick[cB].stn[i].x;
            mvprintw(y, x, " ");

            //increment stones pos
            bP[0].brick[cB].stn[i].y += 0.05;
        }
        //deprecate:
        bP[0].brick[cB].y += 0.05;
    }else{
        //respawn
        mvprintw(2,25,"respawned @ %i",time(NULL));
        initBricks(bP);

        //determine "random" x for stone index 1
        time_t t;
        srand((unsigned) time(&t));
        int r = rand() % 5;
        //int r=5;
        switch(r){
            case 0:
                bP[0].brick[cB].stn[2].y -= 2;
                break;
            case 1:
                bP[0].brick[cB].stn[1].x -= 1;
                bP[0].brick[cB].stn[1].y += 2;
                bP[0].brick[cB].stn[3].x -= 1;
                bP[0].brick[cB].stn[3].y += 2;
                break;
            case 2:
                bP[0].brick[cB].stn[2].x += 2;
                break;
            case 3:
                bP[0].brick[cB].stn[3].x -= 1;
                bP[0].brick[cB].stn[3].y += 1;
                break;
            case 4:
                bP[0].brick[cB].stn[1].x -= 1;
                bP[0].brick[cB].stn[1].y += 2;
                break;
            case 5:
                break;
            default:
                break;
        }
    }

    //if(bP[0].brick[cB].x > 22){
    //    mvprintw((bP[0].brick[cB].y + 1), bP[0].brick[cB].x, "I'm FREE!");
    //    refresh();
    //    sleep(3);
    //	drawStage(15, 20, 1, bP);
    //    bP->score += 4;
    //    bP->curBr = rand() % 3;
    //    bP[0].brick[cB].y = 2;
    //    bP[0].brick[cB].x = 7;
    //}
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
        float nY = (bP[0].brick[cB].stn[i].y);
        float nX = (bP[0].brick[cB].stn[i].x) + 1;
        if(mvinch(nY, nX) == '|' || mvinch(nY, nX) == 'O')
            colR++;

        //nY = (bP[0].brick[cB].stn[i].y);
        nX = (bP[0].brick[cB].stn[i].x) - 1;
        if(mvinch(nY, nX) == '|' || mvinch(nY, nX) == 'O')
            colL++;
    }

    //movement
    if(mv == KEY_UP){
        //rotate
        int j;
        float newX;
        float newY;
        float angle=-90.00;
        for(j = 0;j < 4;j++){
            //apply rotation matrix
            // x = x cos angle - y sin angle
            // y = x sin angle + y cos angle
        
            //newX = (bP[0].brick[cB].stn[j].x * cos(angle)) - (bP[0].brick[cB].stn[j].y * sin(angle));
            //newY = (bP[0].brick[cB].stn[j].x * sin(angle)) + (bP[0].brick[cB].stn[j].y * cos(angle));
            //mvprintw(6+j, 25, "[%d] current x: %f newX: %f, current y: %f newY: %f", j, bP[0].brick[cB].stn[3].x, newX, bP[0].brick[cB].stn[3].y, newY);

            //bP[0].brick[cB].stn[j].x = newX;
            //bP[0].brick[cB].stn[j].y = newY;

            //# new_x = (0)(old_x)+(-1)(old_y)
            //# new_y = (1)(old_x)+(0)(old_y)

            newX = (0 * bP[0].brick[cB].stn[j].x + (-1 * bP[0].brick[cB].stn[j].y));
            newY = (1 * bP[0].brick[cB].stn[j].x) + (0 * bP[0].brick[cB].stn[j].y);

            mvprintw(6+j, 25, "[%d] current x, y: %f, %f, new x, y: %f, %f", j, bP[0].brick[cB].stn[j].x, bP[0].brick[cB].stn[j].y, newX, newY);

            bP[0].brick[cB].stn[j].x = newX;
            bP[0].brick[cB].stn[j].y = newY;
        }
    }else if(mv == KEY_DOWN){
        bP[0].brick[cB].y++;
        for(i = 0;i < 4;i++){
            bP[0].brick[cB].stn[i].y++;
        }
    }else if(mv == KEY_LEFT && colL == 0){
        bP[0].brick[cB].x--;
        for(i = 0;i < 4;i++){
            bP[0].brick[cB].stn[i].x--;
        }
    }else if(mv == KEY_RIGHT && colR == 0){
        bP[0].brick[cB].x++;
        for(i = 0;i < 4;i++){
            bP[0].brick[cB].stn[i].x++;
        }
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
    //float sX = 25.0;
    //float sY = 20.0;

    brick br1 = {
        .id = 0,
        .body = 'O',
        .x = sX,
        .y = sY,
        .stn[0].x = sX,
        .stn[0].y = sY,
        .stn[1].x = sX + 1,
        .stn[1].y = sY,
        .stn[2].x = sX,
        .stn[2].y = sY + 1,
        .stn[3].x = sX + 1,
        .stn[3].y = sY + 1,
    };

    brcks brcks = {
        .curBr = 0,
        .tjek = 0,
        .score = 0,
        .brick[0] = br1,
    };

    *bP = brcks;
}

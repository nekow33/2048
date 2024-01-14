
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

unsigned int g_PosX = 0;
unsigned int g_PosY = 0;

#define SIDELEN 4

typedef enum tagGameState {
    GAME_INIT = 0,
    GAME_RUN,
    GAME_END
} GameState;

typedef enum tagGameMode {
    GAME_MODE_NORMAL = 0,
    GAME_MODE_ENDLESS,
    GAME_MODE_BUTT
} GameMode;

typedef struct tagGameRunModeTbl {
    char mode;
    void (*GameRunByModeFunc)(void);
} GameRunModeTbl;

typedef struct tagGameActionTbl {
    int key;
    void (*GameRunAction)(void);
} GameActionTbl;

void gameRunModeNormal();
void gameRunModeEndless();
void gameRunModeQuit();
void gameCleanPane();
void gameActionUp();
void gameActionDown();
void gameActionLeft();
void gameActionRight();
void gameActionQuit();

GameRunModeTbl g_GameRunModeTbl[] = {
    {'1', gameRunModeNormal},
    {'2', gameRunModeEndless},
    {'q', gameRunModeQuit},
};

GameActionTbl g_GameActiontbl[] = {
    {'k', gameActionUp}, {'w', gameActionUp}, {KEY_UP, gameActionUp},

    {'j', gameActionDown}, {'s', gameActionDown}, {KEY_DOWN, gameActionDown},

    {'h', gameActionLeft}, {'a', gameActionLeft}, {KEY_LEFT, gameActionLeft},

    {'l', gameActionRight}, {'d', gameActionRight}, {KEY_RIGHT, gameActionRight},

    {'q', gameActionQuit},
};

unsigned int g_GamePane[SIDELEN][SIDELEN] = {0};
unsigned int g_GameState = GAME_INIT;
unsigned int g_GameMode = GAME_MODE_NORMAL;
unsigned int g_GameNormalValue = 64;

#define BREAK_IF(cond) if ((cond) == true) {break;}

void gameActionMerge(unsigned int *p1, unsigned int *p2, unsigned int *p3, unsigned int *p4)
{
    int ar[SIDELEN] = { *p1, *p2, *p3, *p4 };
    int i = 0, j = 0;
    while (i < SIDELEN) {
        for (j = i + 1; j < SIDELEN; j++) {
            if (ar[i] == 0 && ar[j] != 0) {
                break;
            } else if (ar[i] != 0 && ar[j] == ar[i]) {
                break;
            }
        }
        if (j < SIDELEN) ar[i] += ar[j], ar[j] = 0;
        i++;
    }
    *p1 = ar[0], *p2 = ar[1], *p3 = ar[2], *p4 = ar[3];
    return;
}

void gameActionUp()
{
    for (int j = 0; j < SIDELEN; j++) {
        gameActionMerge(&(g_GamePane[0][j]), &(g_GamePane[1][j]), &(g_GamePane[2][j]), &(g_GamePane[3][j]));
    }
}

void gameActionDown()
{
    for (int j = 0; j < SIDELEN; j++) {
        gameActionMerge(&(g_GamePane[3][j]), &(g_GamePane[2][j]), &(g_GamePane[1][j]), &(g_GamePane[0][j]));
    }
}

void gameActionLeft()
{
    for (int j = 0; j < SIDELEN; j++) {
        gameActionMerge(&(g_GamePane[j][0]), &(g_GamePane[j][1]), &(g_GamePane[j][2]), &(g_GamePane[j][3]));
    }\
}

void gameActionRight()
{
    for (int j = 0; j < SIDELEN; j++) {
        gameActionMerge(&(g_GamePane[j][3]), &(g_GamePane[j][2]), &(g_GamePane[j][1]), &(g_GamePane[j][0]));
    }
}

void gameActionQuit()
{
    g_GameState = GAME_INIT;
}
int gamePaneValueMaxWidth()
{
    int x = 0;
    for (int i = 0; i < SIDELEN; i++) {
        for (int j = 0; j < SIDELEN; j++) {
            if (g_GamePane[i][j] > x) x = g_GamePane[i][j];
        }
    }
    int rslt = 1;
    while (x/10) {
        x = x / 10;
        rslt++;
    }
    return rslt;
}

void gameDrawValueBorder(int valueWidth)
{
    addch('|');
    for (int i = 0; i < SIDELEN; i++) {
        for (int j = 0; j < valueWidth+2; j++)
            addch('-');
        if (i < SIDELEN - 1) addch('-');
    }
    addch('|');
}

void gameDrawValueLine(int row, int valueWidth)
{
    addch('|');
    for (int i = 0; i < SIDELEN; i++) {
        if (g_GamePane[row][i] == 0) {
            for (int j = 0; j < valueWidth + 2; j++) addch(' ');
        } else {
            char ar[16] = {0};
            char format[6] = {0};
            sprintf(format, " %%%uu ", valueWidth);
            sprintf(ar, format, g_GamePane[row][i]);
            for (int j = 0; j < valueWidth + 2; j++) addch(ar[j]);
        }
        addch('|');
    }
}

void gameGenerate()
{
    int pos = rand() % (SIDELEN * SIDELEN);
    int row = pos / SIDELEN;
    int col = pos % SIDELEN;
    int val = (rand() % (SIDELEN * SIDELEN)) < 10 ? 2 : 4;
    if (g_GamePane[row][col] == 0) {
        g_GamePane[row][col] = val;
    } else {
        gameGenerate();
    }
    return;
}

void gameDrawPane()
{
    int valueWidth = gamePaneValueMaxWidth();
    for (int i = 0; i < 2 * SIDELEN + 1; i++) {
        move(g_PosY + i, g_PosX);
        if (i % 2) {
            gameDrawValueLine(i/2, valueWidth);
        } else {
            gameDrawValueBorder(valueWidth);
        }
    }
}

void gameDraw()
{
    clear();
    gameGenerate();
    gameDrawPane();
    refresh();
}

int gameCheckEnd()
{
    int endflag = 1, winflag = 0;
    for (int i = 0; i < SIDELEN; i++) {
        for (int j = 0; j < SIDELEN; j++) {
            if (g_GamePane[i][j] == 0) {
                endflag = 0;
            }
            if (g_GameMode == GAME_MODE_NORMAL && g_GamePane[i][j] == g_GameNormalValue) {
                winflag = 1;
            }
        }
    }
    if (winflag) {
    }
    if (winflag == 0 && endflag == 1) {
    }
    return winflag + endflag;
}


void gameLoop()
{
    g_GameState = GAME_RUN;
    while (g_GameState == GAME_RUN) {
        gameDraw();
        char key = getch();
        for (int i = 0; i < sizeof(g_GameActiontbl) / sizeof(GameActionTbl); i++) {
            if (g_GameActiontbl[i].key == key) {
                g_GameActiontbl[i].GameRunAction();
            }
        }
        if (0 != gameCheckEnd() || g_GameState != GAME_RUN) {
            gameCleanPane();
            break;
        }
    }
}

void gameRunModeNormal()
{
    g_GameMode = GAME_MODE_NORMAL;
    gameLoop();
    return;
}

void gameRunModeEndless()
{
    g_GameMode = GAME_MODE_ENDLESS;
    gameLoop();
    return;
}

void gameRunModeQuit()
{
    g_GameState = GAME_END;
}

void gameInitCurses(void)
{
    initscr();
    cbreak();
    nonl();
    noecho();
    intrflush(stdscr, true);
    keypad(stdscr, true);
    curs_set(0);
    refresh();
}

void gameInitPos(void)
{
    getmaxyx(stdscr, g_PosY, g_PosX);
    g_PosX = ((g_PosX / 2) > 8) ? (g_PosX / 2 - 8) : 0;
    g_PosY = ((g_PosY / 2) > 10) ? (g_PosY / 2 - 10) : 0;
}

void gameInit(void)
{
    gameInitCurses();
    gameInitPos();
    g_GameState = GAME_INIT;
}

void gameCleanPane(void)
{
    memset(g_GamePane, 0, sizeof(g_GamePane));
    clear();
}

void gameShowStart(void)
{
    mvaddstr(g_PosY + 0, g_PosX/2, "\t\tGAME 2048\n");
    mvaddstr(g_PosY + 1, g_PosX/2, "\t\tSlect Game Mode\n");
    mvaddstr(g_PosY + 2, g_PosX/2, "\t\t1. 2048");
    mvaddstr(g_PosY + 3, g_PosX/2, "\t\t2. Endless");
    mvaddstr(g_PosY + 4, g_PosX/2, "\t\t3. Quit");
}

unsigned int gameStart(void) 
{
    while (g_GameState != GAME_END) {
        gameCleanPane();
        gameShowStart();
        int key = getch();
        for (int i = 0; i < sizeof(g_GameRunModeTbl) / sizeof(GameRunModeTbl); i++) {
            if (g_GameRunModeTbl[i].mode == key) {
                g_GameRunModeTbl[i].GameRunByModeFunc();
                break;
            }
        }
    }
    return 0;
}

void gameClean(void)
{
    clear();
    refresh();
    endwin();
}

int main(void)
{
    gameInit();
    gameStart();
    gameClean();
    return 0;
}

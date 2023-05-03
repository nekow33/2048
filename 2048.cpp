
#include "2048.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <curses.h>

using namespace std;


// 随机数宏定义
#define random() rand()%(ROW * COL)

std::string Game_2048::getHorizontalBorder()
{
    string ans = "";
    for (int i = 0; i < 4 + (COL - 1) * 3; ++i)
        ans += "-";
    int length = this->maxValueLength();
    for (int i = 0; i < length * COL; ++i)
       ans += "-";
    return ans;
}

int Game_2048::maxValueLength()
{
    int mx = pos[0][0];
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            if (mx < pos[i][j]) {
                mx = pos[i][j];
            }
        }
    }
    return to_string(mx).size();
}

int Game_2048::maxValue()
{
    int ans = 0;
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            ans = ans < pos[i][j] ? pos[i][j] : ans;
        }
    }
    return ans;
}

void Game_2048::up()
{
    for (int col = 0; col < COL; ++col) {
        for (int i = 0; i < ROW; ++i) {
            int t = pos[i][col];
            int j = i + 1;
            while (j < ROW && pos[j][col] == 0)
                ++j;
            if (j < ROW) {
                if (t == 0) {
                    pos[i][col] = pos[j][col];
                    pos[j][col] = 0;
                    i -= 1;
                } else if (t == pos[j][col]) {
                    pos[i][col] *= 2;
                    pos[j][col] = 0;
                }
            }
        }
    }
}

void Game_2048::down()
{
    for (int col = 0; col < COL; ++col) {
        for (int i = ROW - 1; i >= 0; --i) {
            int t = pos[i][col];
            int j = i - 1;
            while (j >= 0 && pos[j][col] == 0)
                --j;
            if (j >= 0) {
                if (t == 0) {
                    pos[i][col] = pos[j][col];
                    pos[j][col] = 0;
                    i += 1;
                } else if (pos[j][col] == t) {
                    pos[i][col] *= 2;
                    pos[j][col] = 0;
                }
            }
        }
    }
}

void Game_2048::left()
{
    for (int row = 0; row < ROW; ++row) {
        for (int i = 0; i < COL; ++i){
            int t = pos[row][i];
            int j = i + 1;
            while (j < COL && pos[row][j] == 0)
                ++j;
            if (j < COL) {
                if (t==0){
                    pos[row][i] = pos[row][j];
                    pos[row][j] = 0;
                    i -= 1;
                } else if (pos[row][j] == t) {
                    pos[row][i] *= 2;
                    pos[row][j] = 0;
                }
            }
        }
    }
}

void Game_2048::right()
{
    for (int row = 0; row < ROW; ++row) {
        for (int i = COL - 1; i >= 0; --i){
            int t = pos[row][i];
            int j = i - 1;
            while (j >= 0 && pos[row][j] == 0)
                --j;
            if (j >= 0) {
                if (t == 0) {
                    pos[row][i] = pos[row][j];
                    pos[row][j] = 0;
                    i += 1;
                } else if (pos[row][j] == t) {
                    pos[row][i] *= 2;
                    pos[row][j] = 0;
                }
            }
        }
    }
}

void Game_2048::draw()
{
    // Linux
    clear();
    vector<string> rowStr;
    int length = this->maxValueLength();
    for (int i = 0; i < 2 * ROW + 1; ++i) {
        if (i % 2 == 0) {
            rowStr.push_back(this->getHorizontalBorder());
        } else {
            string _row = "";
            int row = i / 2;
            _row += "|";
            for (int j = 0; j < COL; ++j) {
                string tmp;
                if (pos[row][j] == 0) {
                    tmp = "";
                    for (int i = 0; i < length; ++i)
                        tmp += " ";
                } else {
                    tmp = to_string(pos[row][j]);
                    if (tmp.size() < length) {
                        string _t = "";
                        for (int i = 0; i < length - tmp.size(); ++i)
                            _t += " ";
                        tmp = _t + tmp;
                    }
                }
                _row = _row + " " + tmp + " |";
            }
            rowStr.push_back(_row);
        }
        
        int idx = 0;
        for (auto r: rowStr) {
            // cout << r;
            move(s_y + idx++, s_x);
            for (auto ch: r) {
                addch(ch);
            }
        }
        refresh();
    }
}

bool Game_2048::check()
{
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            if (pos[i][j] == 0)
                return true;
        }
    }
    return false;
}

void Game_2048::generate()
{
    // srand((int)time(0));
    int p = rand() % (this->ROW * this->COL);
    int row = p / ROW;
    int col = p % COL;
    int val = (rand() % (ROW * COL)) < 12 ? 2 : 4;
    // cout << row << " " << col << endl;
    if (pos[row][col] == 0)
        pos[row][col] = val;
    else 
        generate();
    return ;
}

void Game_2048::clean()
{
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j)
            this->pos[i][j] = 0;
    }
    this->endless = false;
}

void Game_2048::run()
{
START:
    this->clean();
    clear();
    mvaddstr(s_y + 0, s_x - 10, "\t\tGAME 2048\n");
    mvaddstr(s_y + 1, s_x - 10, "\t\tSelect mode");
    mvaddstr(s_y + 2, s_x - 10, "\t\t1. 2048 ");
    mvaddstr(s_y + 3, s_x - 10, "\t\t2. Endless ");
    mvaddstr(s_y + 4, s_x - 10, "\t\tq. Quit ");
    refresh();
    int key = 0;
    key = getch();
    switch (key) {
        case '1':
            this->endless = false;
            break;
        case '2':
            this->endless = true;
            break;
        case 'q':
            return;
        default:
            mvaddstr(s_y + 6, s_x -18, "Please Input 1 or 2 ! Press any key to continue");
            getch();
            goto START;

    }

    refresh();
    generate();
    generate();
    draw();

    do {
        key = getch();
        switch (key) {
            case KEY_UP:
            case 'k':
            case 'w':
                this->up();
                break;
            case KEY_DOWN:
            case 'j':
            case 's':
                this->down();
                break;
            case KEY_RIGHT:
            case 'l':
            case 'd':
                this->right();
                break;
            case KEY_LEFT:
            case 'h':
            case 'a':
                this->left();
                break;
            case 'q':
                clear();
                return;
            default:
                continue;
        }

        if (this->endless==false && this->maxValue() == 2048) {
            printw("\t You Sucess !!! \n");
            printw("\t Press r to play again. \n");
            key = getch();
            if (key == 'r')
                goto START;
            return;
        }
        
        if (this->check()) {
            this->generate();
            this->draw();
        } else {
            printw("\n\t GAME OVER !!! \n\tScore: %d\n\tPress r to Play Again\n", this->maxValue());
            key = getch();
            if (key == 'r')
                goto START;
            return;
        }
    } while (1);
}

void Game_2048::setStartPos(int y, int x)
{
    this->s_x = x - 8;
    this->s_y = y - 10;
}

void init()
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

int main()
{
    init();
    Game_2048 game;
    int y, x;
    getmaxyx(stdscr, y, x);
    game.setStartPos(y / 2, x / 2);
    game.run();
    refresh();
    endwin();
}

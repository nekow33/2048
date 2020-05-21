
#include "2048.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <ctime>

using namespace std;


// 随机数宏定义
#define random() rand()%(ROW * COL)

std::string Game_2048::getHorizontalBorder()
{
    string ans = "";
    for (int i = 0; i < 4 * COL + 1; ++i)
        ans += "-";
    int length = this->maxValueLength();
    for (int i = 0; i < length * ROW; ++i)
       ans += "-";
    return ans;
}

int Game_2048::maxValueLength()
{
    int mx = pos[0][0];
    for (int i = 0; i < ROW; ++i)
    {
        for (int j = 0; j < COL; ++j)
        {
            if (mx < pos[i][j])
                mx = pos[i][j];
        }
    }
    return to_string(mx).size();
}

int Game_2048::maxValue()
{
    int ans = 0;
    for (int i = 0; i < ROW; ++i)
    {
        for (int j = 0; j < COL; ++j)
        {
            ans = ans < pos[i][j] ? pos[i][j] : ans;
        }
    }
    return ans;
}

void Game_2048::up()
{
    for (int col = 0; col < COL; ++col)
    {
        for (int i = 0; i < ROW; ++i)
        {
            int t = pos[i][col];
            int j = i + 1;
            while (j < ROW && pos[j][col] == 0)
                ++j;
            if (j < ROW)
            {
                if (t == 0)
                {
                    pos[i][col] = pos[j][col];
                    pos[j][col] = 0;
                    i -= 1;
                }
                else if (t == pos[j][col])
                {
                    pos[i][col] *= 2;
                    pos[j][col] = 0;
                }
            }
        }
    }
}

void Game_2048::down()
{
    for (int col = 0; col < COL; ++col)
    {
        for (int i = ROW - 1; i >= 0; --i)
        {
            int t = pos[i][col];
            int j = i - 1;
            while (j >= 0 && pos[j][col] == 0)
                --j;
            if (j >= 0)
            {
                if (t == 0)
                {
                    pos[i][col] = pos[j][col];
                    pos[j][col] = 0;
                    i += 1;
                }
                else if (pos[j][col] == t)
                {
                    pos[i][col] *= 2;
                    pos[j][col] = 0;
                }
            }
        }
    }
}

void Game_2048::left()
{
    for (int row = 0; row < ROW; ++row)
    {
        for (int i = 0; i < COL; ++i)
        {
            int t = pos[row][i];
            int j = i + 1;
            while (j < COL && pos[row][j] == 0)
                ++j;
            if (j < COL)
            {
                if (t==0)
                {
                    pos[row][i] = pos[row][j];
                    pos[row][j] = 0;
                    i -= 1;
                }
                else if (pos[row][j] == t)
                {
                    pos[row][i] *= 2;
                    pos[row][j] = 0;
                }
            }

        }
    
    }
}

void Game_2048::right()
{
    for (int row = 0; row < ROW; ++row)
    {
        for (int i = COL - 1; i >= 0; --i)
        {
            int t = pos[row][i];
            int j = i - 1;
            while (j >= 0 && pos[row][j] == 0)
                --j;
            if (j >= 0)
            {
                if (t == 0)
                {
                    pos[row][i] = pos[row][j];
                    pos[row][j] = 0;
                    i += 1;
                }
                else if (pos[row][j] == t)
                {
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
    system("clear");
    int length = this->maxValueLength();
    for (int i = 0; i < 2 * ROW + 1; ++i)
    {
        if (i % 2 == 0)
        {
            cout << this->getHorizontalBorder();
        }
        else
        {
            int row = i / 2;
            cout << "|";
            for (int j = 0; j < COL; ++j)
            {
                string tmp;
                if (pos[row][j] == 0)
                {
                    tmp = "";
                    for (int i = 0; i < length; ++i)
                        tmp += " ";
                }
                else
                {
                    tmp = to_string(pos[row][j]);
                    if (tmp.size() < length)
                    {
                        string _t = "";
                        for (int i = 0; i < length - tmp.size(); ++i)
                            _t += " ";
                        tmp = _t + tmp;
                    }
                }
                cout << " "  << tmp << " |";
            }
        }
        cout << endl;
    }
}

bool Game_2048::check()
{
    for (int i = 0; i < ROW; ++i)
    {
        for (int j = 0; j < COL; ++j)
        {
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
    for (int i = 0; i < ROW; ++i)    
    {
        for (int j = 0; j < COL; ++j)
            this->pos[i][j] = 0;
    }
    this->endless = false;
}

void Game_2048::run()
{
START:
    this->clean();
    system("clear");
    cout << "\t\tGAME 2048" << endl;
    cout << "\t\tSelect mode" << endl;
    cout << "\t\t1. 2048 " << endl;
    cout << "\t\t2. Endless " << endl;
    cout << "\t\tq. Quit " << endl;

    char key = 0;
    cin >> key;
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
            cout << "Please Input 1 or 2 ! Press any key to continue" << endl;
            cin >> key;
            goto START;

    }

    generate();
    generate();
    draw();
    while (true)
    {
        char key = 0;
        while (cin >> key)
        {
            switch (key)
            {
            case 'w':
                this->up();
                break;
            case 's':
                this->down();
                break;
            case 'd':
                this->right();
                break;
            case 'a':
                this->left();
                break;
            case 'q':
                system("clear");
                return;
            default:
                break;
            }
            if (this->endless==false && this->maxValue() == 2048)
            {
                cout << "\t You Sucess !!!" << endl;
                cout << "\t Press r to play again. " << endl;
                cin >> key;
                if (key == 'r')
                    goto START;
                return;
            }
            if (this->check())
            {
                this->generate();
                this->draw();
            }
            else
            {
                cout << "\tGame Over !!! \n" 
                     << "\tScore: " << this->maxValue()  << "\n"
                     << "\tPress r to Play Again. Press q to quit or Ctrl-c" << endl;
                cin >> key;
                if (key == 'r')
                    goto START;
                return;
            }   
        }
    }
}

int main()
{
    Game_2048 game;
    game.run();
}

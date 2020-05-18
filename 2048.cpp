#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <ctime>

using namespace std;

#define ROW 4
#define COL 4

// 随机数宏定义
#define random() rand()%(ROW * COL)

class Game_2048
{
public:
    void run();

private:
    int pos[ROW][COL] = {
        {0, 0, 4, 0},
        {0, 0, 2, 0},
        {0, 0, 2, 2},
        {0, 0, 4, 2}};
    inline string cacHor()
    {
        string ans = "";
        for (int i = 0; i < 4 * COL + 1; ++i)
        {
            ans += "-";
        }
        int l = vl() - 1;
        for (int i = 0; i < l * ROW; ++i)
            ans += '-';
        return ans;
    }
    string hor = cacHor();

    int vl()
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

    void upCol()
    {
        for (int col = 0; col < COL; ++col)
        {
            for (int i = 0; i < ROW; ++i)
            {
                int t = pos[i][col];
                int j = i + 1;
                while (j < ROW && pos[j][col] == 0)
                {
                    ++j;
                }
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

    void downCol()
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

    void rightRow()
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

    void leftRow()
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

    void draw()
    {
        system("clear");
        int l = vl();
        for (int i = 0; i < 2 * ROW + 1; ++i)
        {
            if (i % 2 == 0)
            {
                cout << cacHor();
            }
            else
            {
                int row = i / 2;
                cout << "|";
                for (int j = 0; j < COL; ++j)
                {
                    if (pos[row][j] == 0)
                    {
                        string tmp = "";
                        for (int i = 0; i < l; ++i)
                        {
                            tmp += " ";
                        }
                        cout << " "  << tmp << " |";
                    }
                    else
                    {
                        string tmp = to_string(pos[row][j]);
                        if (tmp.size() < l)
                        {
                            string _t = "";
                            for (int i = 0; i < l - tmp.size(); ++i)
                            {
                                _t += " ";
                            }
                            tmp = _t + tmp;
                        }
                        cout << " "  << tmp << " |";
                    }
                }
                
            }
            cout << endl;
        }
    }

    bool check()
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

    void generate()
    {
        int p = random();
        int row = p / ROW;
        int col = p % COL;
        if (pos[row][col] == 0)
        {
            int v = random() < 12 ? 2 : 4;
            pos[row][col] = v;
        }
        else 
        {
            this->generate();
        }
    }
};

void Game_2048::run()
{
    this->draw();
    while (true)
    {
        char key = 0;
        while (cin >> key)
        {
            switch (key)
            {
            case 'w':
                this->upCol();
                break;
            case 's':
                this->downCol();
                break;
            case 'd':
                this->rightRow();
                break;
            case 'a':
                this->leftRow();
                break;
            case 'q':
                system("clear");
                return;
            default:
                break;
            }
            if (this->check())
            {
                this->generate();
                this->draw();
            }
            else
            {
                cout << "\tYou LOSE !!! \n\tpress q to quit or Ctrl-c" << endl;
                cin >> key;
                return ;
            }
                        
        }
    }
}

int main()
{
    srand((int)time(0));
    Game_2048 game;
    game.run();
    return 0;
}

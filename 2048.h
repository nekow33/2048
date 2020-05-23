
#ifndef GAME_2048
#define GAME_2048
#include <string>
#include <curses.h>
class Game_2048
{
public:
    Game_2048()
    {}
    void setStartPos(int, int);
    void run();
private:
    static const int ROW = 4;
    static const int COL = 4;
    int s_x = 0, s_y = 0;
    bool endless = false;
    int pos[ROW][COL] = { 0 };
    std::string getHorizontalBorder();
    int maxValueLength();
    int maxValue();
    void up();
    void down();
    void left();
    void right();
    void draw();
    bool check();
    void generate();
    void clean();
};

#endif

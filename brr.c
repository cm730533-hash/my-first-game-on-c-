#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

int ball_x, ball_y, ball_dx, ball_dy;
int pad1_y, pad2_y, score1, score2;
int max_y, max_x;

void setup() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    getmaxyx(stdscr, max_y, max_x);

    ball_x = max_x / 2;
    ball_y = max_y / 2;
    ball_dx = 1;
    ball_dy = 1;

    pad1_y = max_y / 2;
    pad2_y = max_y / 2;
    score1 = 0;
    score2 = 0;
}

void draw() {
    clear();
    
    for(int i = 0; i < max_y; i++) mvprintw(i, max_x / 2, "|");

    
    for(int i = -2; i <= 2; i++) {
        mvprintw(pad1_y + i, 2, "]"); 
        mvprintw(pad2_y + i, max_x - 3, "["); 
    }

    
    mvprintw(ball_y, ball_x, "O");
    mvprintw(0, (max_x / 2) - 10, "PLAYER: %d", score1);
    mvprintw(0, (max_x / 2) + 5, "CPU: %d", score2);
    refresh();
}

void logic() {
    int prev_x = ball_x;
    ball_x += ball_dx;
    ball_y += ball_dy;

    
    if (ball_dx > 0) {
        if (pad2_y < ball_y && pad2_y < max_y - 3) pad2_y++;
        else if (pad2_y > ball_y && pad2_y > 2) pad2_y--;
    }

    
    if (ball_y >= max_y - 1 || ball_y <= 0) ball_dy = -ball_dy;

    
    if (ball_x <= 2 && prev_x > 2) {
        if (ball_y >= pad1_y - 2 && ball_y <= pad1_y + 2) {
            ball_dx = 1;
            ball_x = 3; 
        }
    }
    
    else if (ball_x >= max_x - 3 && prev_x < max_x - 3) {
        if (ball_y >= pad2_y - 2 && ball_y <= pad2_y + 2) {
            ball_dx = -1;
            ball_x = max_x - 4;
        }
    }

    
    if (ball_x >= max_x) {
        score1++;
        ball_x = max_x / 2; ball_y = max_y / 2; ball_dx = -1;
        usleep(500000);
    } else if (ball_x <= 0) {
        score2++;
        ball_x = max_x / 2; ball_y = max_y / 2; ball_dx = 1;
        usleep(500000);
    }
}

int main() {
    setup();
    int ch;
    while(1) {
    
        ch = getch();
        if (ch == KEY_UP && pad1_y > 2) pad1_y--;
        else if (ch == KEY_DOWN && pad1_y < max_y - 3) pad1_y++;
        else if (ch == 'q') break;

        logic();
        draw();
        usleep(30000); 
    }
    endwin();
    return 0;
}
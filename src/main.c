#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <wrc/wrc.h>

int main(void) {

    initscr();
    noecho();

    printw("Wirecroc");
    
    getch();
    endwin();
    
    return 0;
}

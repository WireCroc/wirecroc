#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <wrc/wrc.h>
#include <pthread.h>
#include <wrc/wrc/wrc.h>

typedef struct {
    size_t x;
    size_t y;
} WVec2;

void* wrc_sys(void*);

int main(int argc, char** argv) {
    wrc sys;
    wrc_default(&sys);
    wc_iflist ifc = wrc_get_interfaces();

    if (argc <= 1) {
        fprintf(stderr, "Error: please choose interface\n\nExample \n\t$ wirecroc <interface index> \n\nChoose Interface:\n");
        for (int i = 0; i < ifc.len; i++)
            fprintf(stderr, "%d:\n    %s ->\n       mtu: %lu\n       flag: %d\n\n", i, ifc.ifc[i].name, ifc.ifc[i].mtu, ifc.ifc[i].flag);
        return 1;
    } else {
        int ifcn = atoi(argv[1]);

        int opts = wrc_setopts(&sys, ifc.ifc[ifcn], PA_NULL, 0);
        if (opts != 0) {
            printf("wrc_setopts error\n");
            exit(0);
        }
    }
    
    pthread_t thr_wrc;
    pthread_create(&thr_wrc, NULL, wrc_sys, (void*)&sys);
    
    initscr();
    noecho();
    cbreak();

    WVec2 vec;
    getmaxyx(stdscr, vec.y, vec.x);

    int choice, plc = 0;
    char* pa[3] = {
        "Ajameti",
        "Gonio",
        "Liabio",
    };
    
    keypad(stdscr, TRUE);
    printw("x: %zu, y: %zu, pa size: %zu", vec.x, vec.y, sizeof(pa));

    WINDOW* pwin = newwin(0, vec.x - 10, vec.y / 6, 5);
    box(pwin, 0, 0);

    refresh();
    wrefresh(pwin);
    keypad(pwin, TRUE);
    
    while (TRUE) {
        for (int i = 0; i < 3; i++) {
            if (i == plc && !(i > 3))
                wattron(pwin, A_REVERSE);

            mvwprintw(pwin, i + 1, 1, "%s", pa[i]);
            wattroff(pwin, A_REVERSE);
        }
        choice = wgetch(pwin);

        switch (choice) {
        case KEY_UP:
            plc--;
            if (plc == -1)
                plc = 0;
            break;
        case KEY_DOWN:
            plc++;
            if (plc == 3)
                plc = 2;
            break;
        case KEY_LEFT:
            wprintw(pwin, "%s", pa[plc]);
            break;
        default:
            break;
        }
        
        if (choice == 30)
            break;
    }
    
    getch();
    endwin();
    wrc_destroy(&sys);
    
    return 0;
}

void* wrc_sys(void* w) {
    wrc* th_w = (wrc*)w;

    printf("%d\n", th_w->fd);
    
    return NULL;
}

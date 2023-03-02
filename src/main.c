#include "wirecroc.h"

int main(int argc, char** argv) {    
    wrc sys;
    wc_err err;
    wrc_default(&sys);
    wc_iflist ifc = wrc_get_interfaces();

    err = wrc_error(ERR_PASS);
    
    if (argc <= 1) {
        fprintf(stderr, "Error: please choose interface\n\nExample \n\t$ sudo wirecroc <IFC IDX> \n\nChoose Interface:\n");
        for (int i = 0; i < ifc.len; i++)
            fprintf(stderr, "%d:\n    %s ->\n       mtu: %lu\n       flag: %d\n\n", i, ifc.ifc[i].name, ifc.ifc[i].mtu, ifc.ifc[i].flag);
        return ERR_IFC;
    } else if (err.code == ERR_SUDO) {
        fprintf(stderr, "Error: please run program with sudo\n\nExample \n\t$ sudo wirecroc <IFC IDX> \n\n");
        return ERR_SUDO;
    } else {
        int ifcn = atoi(argv[1]);

        int opts = wrc_setopts(&sys, ifc.ifc[ifcn], PA_NULL, 0);
        if (opts != 0) {
            printf("wrc_setopts error\n");
            exit(ERR_SETOPTS);
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

    WINDOW* pwin = newwin(vec.y / 1.2, vec.x - 10, 0, 5);
    WINDOW* cwin = newwin(0, vec.x - 10, vec.y / 1.2, 5);
    box(pwin, 0, 0);
    box(cwin, 0, 0);
    
    refresh();
    wrefresh(pwin);
    keypad(pwin, TRUE);
    scrollok(pwin, TRUE);
    
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

void cap_sys(wc_pa p, FILE * fp) {
    memset(buf, 0, MAX_BUF_SIZE); 
    for (int i = 0; i < MAX_PA; i++) {
        switch (p.p[i]) {
        case PA_ETH:
            char* tmpb = wc_format(ETH_FMT,
                                   p.eth.source,
                                   p.eth.dest
                );
            fprintf(fp, tmpb);
            buf[i] = tmpb;
            break;
        case PA_IP:
            char* tmpb = wc_format(IP_FMT,
                                   p.ip.source,
                                   p.ip.dest,
                                   p.ip.version,
                                   p.ip.ttl,
                                   p.ip.tos,
                                   p.ip.tl,
                                   p.ip.ihl,
                                   p.ip.hchs,
                                   p.ip.ident,
                                   p.ip.proto
                );
            fprintf(fp, tmpb);
            buf[i] = tmpb;
            break;
        case PA_ARP:
            char* tmpb = wc_format(ARP_FMT,
                                   p.arp.hw_t,
                                   p.arp.p_t,
                                   p.arp.hw_len,
                                   p.arp.p_len,
                                   p.arp.opcode,
                                   p.arp.sender_mac,
                                   p.arp.target_mac,
                                   p.arp.sender_ip,
                                   p.arp.target_ip
                );
            fprintf(fp, tmpb);
            buf[i] = tmpb;
            break;
        case PA_TCP:
            char* tmpb = wc_format(TCP_FMT,
                                   pa.tcp.window,
                                   pa.tcp.ack_sequence,
                                   pa.tcp.sequence,
                                   pa.tcp.source,
                                   pa.tcp.dest
                );
            fprintf(fp, tmpb);
            buf[i] = tmpb;
            break;
        case PA_UDP:
            char* tmpb = wc_format(UDP_FMT,
                                   pa.udp.source,
                                   pa.udp.dest,
                );
            fprintf(fp, tmpb);
            buf[i] = tmpb;
            break;
        default:
            break;
        }
    }
}

void* wrc_sys(void* w) {
    wrc* th_w = (wrc*)w;
    
    return NULL;
}

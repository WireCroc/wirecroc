#include "wirecroc.h"

int main(int argc, char** argv) {    
    unsigned char* buf = malloc(sizeof(unsigned char) * 128);
    
    wrc sys;
    wc_err err;
    wrc_default(&sys);
    wc_iflist ifc = wrc_get_interfaces();

    err = wrc_error(ERR_PASS);
    
    if (argc <= 1) {
        fprintf(stderr, "%sError%s please choose interface\n\n%sExample%s \n\t$ sudo wirecroc <IFC IDX> \n\nChoose Interface:\n", KRED, RESET, KCYN, RESET);
        for (int i = 0; i < ifc.len; i++)
            fprintf(stderr, "%d:\n    %s%s%s ->\n       mtu: %lu\n       flag: %d\n\n", i, KGRN, ifc.ifc[i].name, RESET, ifc.ifc[i].mtu, ifc.ifc[i].flag);
        return ERR_IFC;
    } else if (err.code == ERR_SUDO) {
        fprintf(stderr, "%sError%s please run program with sudo\n\n%sExample%s \n\t$ sudo wirecroc <IFC IDX> \n\n", KRED, RESET, KCYN, RESET);
        return ERR_SUDO;
    } else {
        int ifcn = atoi(argv[1]);

        int opts = wrc_setopts(&sys, ifc.ifc[ifcn], PA_NULL, 0);
        if (opts != 0) {
            fprintf(stderr, "%sError%s wrc_setopts error\n", KRED, RESET);
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
                plc = 0;
            break;
        case KEY_ENTER:
            wprintw(pwin, "%s       ", pa[plc]);
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
    free(buf);
    
    return 0;
}

void buffer_new(Buffer* buf) {
    buf->len = 0;
}

void buffer_add(Buffer* buf, void* data, buffer_t type) {
    if (buf->len <= MAX_BUF_SIZE - 1)
        goto end;
    
    buf->data[buf->len] = data;
    buf->t[buf->len] = type;
end:
    buf->len++;
}

void cap_sys(wc_pa p, FILE * fp, void* dt) {
    char* tmpb;
    Buffer* buff = (Buffer*)dt;
    
    for (int i = 0; i < MAX_PA; i++) {
        switch (p.p[i]) {
        case PA_ETH:
            tmpb = wc_format(ETH_FMT,
                             p.eth.source,
                             p.eth.dest
                );
            buffer_add(buff, (void*) tmpb, CHAR_P_T);
            fprintf(fp, "%s\n", tmpb);
            break;
        case PA_IP:
            tmpb = wc_format(IP_FMT,
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
            buffer_add(buff, (void*) tmpb, CHAR_P_T);
            fprintf(fp, "%s\n", tmpb);
            break;
        case PA_ARP:
            tmpb = wc_format(ARP_FMT,
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
            buffer_add(buff, (void*) tmpb, CHAR_P_T);
            fprintf(fp, "%s\n", tmpb);
            break;
        case PA_TCP:
            tmpb = wc_format(TCP_FMT,
                             p.tcp.window,
                             p.tcp.ack_sequence,
                             p.tcp.sequence,
                             p.tcp.source,
                             p.tcp.dest
                );
            buffer_add(buff, (void*) tmpb, CHAR_P_T);
            fprintf(fp, "%s\n", tmpb);
            break;
        case PA_UDP:
            tmpb = wc_format(UDP_FMT,
                             p.udp.source,
                             p.udp.dest
                );
            buffer_add(buff, (void*) tmpb, CHAR_P_T);
            fprintf(fp, "%s\n", tmpb);
            break;
        default:
            break;
        }
    }
}

void* wrc_sys(void* w) {
    wrc* th_w = (wrc*)w;
    Buffer b;
    buffer_new(&b);

    printf("%d\n", th_w->fd);

    wrc_cap(w, 1, cap_sys, (void*)&b);
      
    return NULL;
}

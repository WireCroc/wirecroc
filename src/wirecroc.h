#ifndef WIRECROC_H
#define WIRECROC_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <wrc/wrc.h>
#include <pthread.h>
#include "../libwrc/src/wrc.h"

#define MAX_BUF_SIZE 4

typedef struct {
    size_t x;
    size_t y;
} WVec2;

const char* ARP_FMT = "Address Resolution Protocol -> Hardware Type: %s Protocol Type: %s, Hardware Address Length: %d, Protocol Address Length: %d, Opcode: %s, Sender Mac: %s, Target Mac: %s, Sender IP: %s, Target IP: %s\n";
const char* TCP_FMT = "TCP -> Window: %u, ACK Sequence: %u, Sequence: %u, Src Port: %d, Dst Port: %d\n";
const char* UDP_FMT = "UDP - > Src Port: %d, Dst Port: %d\n";
const char* IP_FMT = "Internet Protocol -> Source: %s, Dest: %s, Version: %d, TTL: %d, TOS: %d, TL: %li, IHL: %d, ICHS: %d, Ident: %d, Proto: %d\n";
const char* ETH_FMT = "Ethernet -> Mac Src: %s, Mac Dst: %s\n";

unsigned char* buf[MAX_BUF_SIZE];

void* wrc_sys(void*);
void cap_sys(wc_pa, FILE*);

#endif

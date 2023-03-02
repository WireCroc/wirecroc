#ifndef WIRECROC_H
#define WIRECROC_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <wrc/wrc.h>
#include <pthread.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\x1B[0m"

#define MAX_BUF_SIZE 128

#define buffer_get(BUF, IDX) {                  \
    switch (BUF.t[IDX]) {                       \
    case INT_T:                                 \
        return (int)BUF.data[IDX];                \
        break;                                  \
    case CHAR_T:                                \
        return (char)BUF.data[IDX];               \
        break;                                  \
    case INT_P_T:                               \
        return (int*)BUF.data[IDX];               \
        break;                                  \
    case CHAR_P_T:                              \
        return (char*) BUF.data[IDX];             \
        break;                                  \
    }                                           \
}                                               \

typedef enum {
    INT_T = 0,
    INT_P_T,
    CHAR_T,
    CHAR_P_T
} buffer_t;

typedef struct {
    void* data[MAX_BUF_SIZE];
    size_t len;
    buffer_t* t;
} Buffer;

typedef struct {
    size_t x;
    size_t y;
} WVec2;

const char* ARP_FMT = "Address Resolution Protocol -> Hardware Type: %s Protocol Type: %s, Hardware Address Length: %d, Protocol Address Length: %d, Opcode: %s, Sender Mac: %s, Target Mac: %s, Sender IP: %s, Target IP: %s\n";
const char* TCP_FMT = "TCP -> Window: %u, ACK Sequence: %u, Sequence: %u, Src Port: %d, Dst Port: %d\n";
const char* UDP_FMT = "UDP - > Src Port: %d, Dst Port: %d\n";
const char* IP_FMT = "Internet Protocol -> Source: %s, Dest: %s, Version: %d, TTL: %d, TOS: %d, TL: %li, IHL: %d, ICHS: %d, Ident: %d, Proto: %d\n";
const char* ETH_FMT = "Ethernet -> Mac Src: %s, Mac Dst: %s\n";

void* wrc_sys(void*);
void cap_sys(wc_pa, FILE*, void*);

void buffer_new(Buffer*);
void buffer_add(Buffer*, void*, buffer_t);

#endif

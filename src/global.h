#ifndef GLOBAL_H
#define GLOBAL_H

#include <time.h>
#include <termios.h>

/* defines */
#define CHII_AUTHOR "Tom Ice"
#define CHII_TAB_STOP 8
#define CHII_QUIT_TIMES 3
#define CTRL_KEY(k) ((k) & 0x1f)

/* VT100 Escape Sequences */
/* More info here: https://vt100.net/docs/vt100-ug/chapter3.html */
#define CURSOR_ERASE_ALL_IN_DISPLAY "\x1b[2J"
#define CURSOR_ERASE_ACTIVE_POS_IN_LINE "\x1b[K"
#define CURSOR_HORIZONTAL_TAB_SET "\x1b[H"
#define CURSOR_POSITION "\x1b[%d;%dH"
#define CURSOR_POS_REPORT "\x1b[6n"
#define CURSOR_SET_MODE "\x1b[?25h"
#define CURSOR_RESET_MODE "\x1b[?25l"
#define CURSOR_BOTTOM_RIGHT "\x1b[999C\x1b[999B"
#define CURSOR_SELECT_GRAPHIC_REND "\x1b[m"
#define CURSOR_INVERT_COLORS "\x1b[7m"

enum EditorKey {
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};

typedef struct erow {
    int size;
    int rsize;
    char *chars;
    char *render;
} erow;

typedef struct editorConfig {
    int cx;
    int cy;
    int rx;
    int offsetY;
    int offsetX;
    int screenY;
    int screenX;
    int numRows;
    erow *row;
    int dirty;
    char *fileName;
    char statusMsg[80];
    time_t statusMsgTime;
    struct termios orig_termios;
} editorConfig;

/* FIXME: Remove this global */
editorConfig E;

#endif /* GLOBAL_H */

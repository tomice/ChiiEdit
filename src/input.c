#include "input.h"
#include "global.h"
#include "output.h"
#include "term.h"
#include "editor.h"
#include "fileIO.h"
#include "find.h"

#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char *editorPrompt(char *prompt, void (*callback)(char *, int))
{
    int c;
    size_t bufSize = 128;
    size_t bufLen = 0;
    char *buf = malloc(bufSize);
    if (buf == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }
    buf[0] = '\0';

    for (;;) {
        editorSetStatusMessage(prompt, buf);
        editorRefreshScreen();
        /* FIXME: Confirm that c is never larger than it should be
                  before trying to process the key */
        c = editorReadKey();
        if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
            if (bufLen != 0) {
                buf[--bufLen] = '\0';
            }
        }
        else if (c == '\x1b') {
            editorSetStatusMessage("");
            if (callback) {
                callback(buf, c);
            }
            free(buf);
            return NULL;
        }
        else if (c == '\r') {
            if (bufLen != 0) {
                editorSetStatusMessage("");
                if (callback) {
                    callback(buf, c);
                }
                return buf;
            }
        }
        /* Make sure there are no special characters */
        else if (!iscntrl(c) && c < 128) {
            if (bufLen == bufSize - 1) {
                bufSize *= 2;
                buf = realloc(buf, bufSize);
                if (buf == NULL) {
                    perror("realloc()");
                    exit(EXIT_FAILURE);
                }
            }
            buf[bufLen++] = c;
            buf[bufLen] = '\0';
        }
        if (callback) {
            callback(buf, c);
        }
    }
}

void editorMoveCursor(int key)
{
    int rowLen;
    erow *row = (E.cy >= E.numRows) ? NULL : &E.row[E.cy];

    switch (key) {
        case ARROW_LEFT:
            if (E.cx != 0) {
                E.cx--;
            }
            else if (E.cy > 0) {
                E.cy--;
                E.cx = E.row[E.cy].size;
            }
            break;
        case ARROW_RIGHT:
            if (row && E.cx < row->size) {
                E.cx++;
            }
            else if (row && E.cx == row->size) {
                E.cy++;
                E.cx = 0;
            }
            break;
        case ARROW_UP:
            if (E.cy != 0) { E.cy--; }
            break;
        case ARROW_DOWN:
            if (E.cy < E.numRows) { E.cy++; }
            break;
    }

    row = (E.cy >= E.numRows) ? NULL : &E.row[E.cy];
    rowLen = row ? row->size : 0;
    if (E.cx > rowLen) { E.cx = rowLen; }
}

void editorProcessKeypress(void)
{
    static int quitTimes = CHII_QUIT_TIMES;
    int c = editorReadKey();
    int times;
    /* FIXME: Add carriage returns and whatnot to EditorKey to be consistent */
    switch (c) {
        /* Terminal enter */
        case '\r':
            editorInsertNewline();
            break;
        /* Quit */
        case CTRL_KEY('q'):
            if (E.dirty && quitTimes > 0) {
                editorSetStatusMessage("WARNING: File has unsaved changes. "
                                       "Press [Ctrl-Q] %d more times to quit.",
                                       quitTimes);
                quitTimes--;
                return;
            }
            /* FIXME: Take a closer look at this later... */
            write(STDOUT_FILENO, CURSOR_ERASE_ALL_IN_DISPLAY, 4);
            write(STDOUT_FILENO, CURSOR_HORIZONTAL_TAB_SET, 3);
            exit(0);
            break;
        /* Save */
        case CTRL_KEY('s'):
            editorSave();
            break;
        case HOME_KEY:
            E.cx = 0;
            break;
        case END_KEY:
            if (E.cy < E.numRows) {
                E.cx = E.row[E.cy].size;
            }
            break;
        /* Find */
        case CTRL_KEY('f'):
            editorFind();
            break;
        case BACKSPACE:
        /* Terminal old school delete */
        case CTRL_KEY('h'):
        case DEL_KEY:
            if (c == DEL_KEY) { editorMoveCursor(ARROW_RIGHT); }
            editorDelChar();
            break;
        case PAGE_UP:
        case PAGE_DOWN:
            {
                if (c == PAGE_UP) {
                    E.cy = E.offsetY;
                }
                else if (c == PAGE_DOWN) {
                    E.cy = E.offsetY + E.screenY - 1;
                    if (E.cy > E.numRows) { E.cy = E.numRows; }
                }
                times = E.screenY;
                while (times--) {
                    editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
                }
            }
            break;
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            editorMoveCursor(c);
            break;
        /* Terminal refresh */
        case CTRL_KEY('l'):
        case '\x1b':
            break;
        default:
            editorInsertChar(c);
            break;
    }
    quitTimes = CHII_QUIT_TIMES;
}

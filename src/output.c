#include "output.h"
#include "global.h"
#include "version.h"
#include "row.h"

#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void editorScroll(void)
{
    E.rx = 0;
    if (E.cy < E.numRows) {
        E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
    }

    if (E.cy < E.offsetY) {
        E.offsetY = E.cy;
    }
    if (E.cy >= E.offsetY + E.screenY) {
        E.offsetY = E.cy - E.screenY + 1;
    }
    if (E.rx < E.offsetX) {
        E.offsetX = E.rx;
    }
    if (E.rx >= E.offsetX + E.screenX) {
        E.offsetX = E.rx - E.screenX + 1;
    }

}

void editorDrawRows(abuf *ab)
{
    int fileRow;
    int welcomeLen;
    int padding;
    int len;
    char welcome[80];

    for (int y = 0; y < E.screenY; y++) {
        fileRow = y + E.offsetY;
        if (fileRow >= E.numRows) {
            if (E.numRows == 0 && y == E.screenY / 3) {
                /* Display welcome screen */
                welcomeLen = snprintf(welcome, sizeof(welcome),
                                      "ChiiEdit version: %s Written by %s",
                                      getVersion(), CHII_AUTHOR);
                if (welcomeLen > E.screenX) {
                    welcomeLen = E.screenX;
                }
                /* Display tildes similar to vim */
                padding = (E.screenX - welcomeLen) / 2;
                if (padding) {
                    abAppend(ab, "~", 1);
                    padding--;
                }
                while (padding--) {
                    abAppend(ab, " ", 1);
                }
                abAppend(ab, welcome, welcomeLen);
            }
            else {
                abAppend(ab, "~", 1);
            }
        }
        else {
            len = E.row[fileRow].rsize - E.offsetX;
            if (len < 0) { len = 0; }
            if (len > E.screenX) { len = E.screenX; }
            abAppend(ab, &E.row[fileRow].render[E.offsetX], len);
        }
        /* Clear one line at a time */
        abAppend(ab, CURSOR_ERASE_ACTIVE_POS_IN_LINE, 3);
        abAppend(ab, "\r\n", 2);
    }
}

void editorDrawStatusBar(abuf *ab)
{
    char status[80];
    char rstatus[80];
    int len;
    int rlen;

    abAppend(ab, CURSOR_INVERT_COLORS, 4);
    /* Display up to 20 chars of the filename */
    len = snprintf(status, sizeof(status), "%.20s - %d lines %s",
                   E.fileName ? E.fileName : "[No Name]", E.numRows,
                   E.dirty ? "(modified)" : "");
    /* Display the line number */
    rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d", E.cy + 1, E.numRows);
    /* Align displayed info left/right of screen */
    if (len > E.screenX) {
        len = E.screenX;
    }
    abAppend(ab, status, len);
    while (len < E.screenX) {
        if (E.screenX - len == rlen) {
            abAppend(ab, rstatus, rlen);
            break;
        }
        else {
            abAppend(ab, " ", 1);
            len++;
        }
    }
    abAppend(ab, CURSOR_SELECT_GRAPHIC_REND, 3);
    abAppend(ab, "\r\n", 2);
}

void editorDrawMessageBar(abuf *ab)
{
    int msgLen;

    abAppend(ab, CURSOR_ERASE_ACTIVE_POS_IN_LINE , 3);
    msgLen = strlen(E.statusMsg);
    if (msgLen > E.screenX) {
        msgLen = E.screenX;
    }
    /* Clear help message at bottom upon key press after 5s */
    if (msgLen && time(NULL) - E.statusMsgTime < 5) {
        abAppend(ab, E.statusMsg, msgLen);
    }
}

void editorRefreshScreen(void)
{
    char buf[32];

    editorScroll();
    abuf ab = ABUF_INIT;

    /* Reset mode to hide cursor before screen refresh*/
    abAppend(&ab, CURSOR_RESET_MODE, 6);
    /* Reposition cursor to upper left */
    abAppend(&ab, CURSOR_HORIZONTAL_TAB_SET, 3);

    editorDrawRows(&ab);
    editorDrawStatusBar(&ab);
    editorDrawMessageBar(&ab);
    /* Moves position of cursor according to given coordinates */
    snprintf(buf, sizeof(buf), CURSOR_POSITION, (E.cy - E.offsetY) + 1,
                                                (E.rx - E.offsetX) + 1);
    abAppend(&ab, buf, strlen(buf));
    /* Set mode to hide cursor before screen refresh */
    abAppend(&ab, CURSOR_SET_MODE, 6);

    if (write(STDOUT_FILENO, ab.b, ab.len) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    abFree(&ab);
}

void editorSetStatusMessage(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(E.statusMsg, sizeof(E.statusMsg), fmt, ap);
    va_end(ap);
    E.statusMsgTime = time(NULL);
}

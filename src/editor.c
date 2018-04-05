#include "editor.h"
#include "global.h"
#include "row.h"
#include "term.h"

#include <stdlib.h>
#include <stdio.h>

void initEditor(void)
{
    E.cx = 0;
    E.cy = 0;
    E.rx = 0;
    E.offsetY = 0;
    E.offsetX = 0;
    E.numRows = 0;
    E.row = NULL;
    E.dirty = 0;
    E.fileName = NULL;
    E.statusMsg[0] = '\0';
    E.statusMsgTime = 0;
    if (getWindowSize(&E.screenY, &E.screenX) == -1) {
        perror("getWindowSize");
        exit(EXIT_FAILURE);
    }
    E.screenY -= 2;
}

void editorInsertChar(int c)
{
    if (E.cy == E.numRows) {
        editorInsertRow(E.numRows, "", 0);
    }
    editorRowInsertChar(&E.row[E.cy], E.cx, c);
    E.cx++;
}

void editorInsertNewline(void)
{
    if (E.cx == 0) {
        editorInsertRow(E.cy, "", 0);
    }
    /* Split line into two rows if not at beginning of line */
    else {
        erow *row = &E.row[E.cy];
        editorInsertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
        row = &E.row[E.cy];
        row->size = E.cx;
        row->chars[row->size] = '\0';
        editorUpdateRow(row);
    }
    E.cy++;
    E.cx = 0;
}

void editorDelChar(void)
{
    erow *row;

    if (E.cy == E.numRows) { return; }
    if (E.cx == 0 && E.cy == 0) { return; }

    row = &E.row[E.cy];
    if (E.cx > 0) {
        editorRowDelChar(row, E.cx - 1);
        E.cx--;
    }
    /* Append chars to prev row and remove the row E.cy is on */
    else {
        E.cx = E.row[E.cy - 1].size;
        editorRowAppendString(&E.row[E.cy - 1], row->chars, row->size);
        editorDelRow(E.cy);
        E.cy--;
    }
}

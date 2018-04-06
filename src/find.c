#include "find.h"
#include "global.h"
#include "row.h"
#include "input.h"

#include <string.h>
#include <stdlib.h>

void editorFindCallback(char *query, int key)
{
    char *match;
    static int lastMatch = -1;
    static int direction = 1;
    int current;
    erow *row;

    if (key == CARRIAGE_RET || key == ESC) {
        lastMatch = -1;
        direction = 1;
        return;
    }
    else if (key == ARROW_RIGHT || key == ARROW_DOWN) {
        direction = 1;
    }
    else if (key == ARROW_LEFT || key == ARROW_UP) {
        direction = -1;
    }
    else {
        lastMatch = -1;
        direction = 1;
    }

    if (lastMatch == -1) { direction = 1; }
    current = lastMatch;

    for (int i = 0; i < E.numRows; i++) {
        current += direction;
        if (current == -1) { current = E.numRows - 1; }
        else if (current == E.numRows) { current = 0; }

        row = &E.row[current];
        match = strstr(row->render, query);
        if (match) {
            lastMatch = current;
            E.cy = current;
            E.cx = editorRowRxToCx(row, match - row->render);
            E.offsetY = E.numRows;
            break;
        }
    }
}

void editorFind(void)
{
    /* Save vals to return when done searching */
    int savedCx = E.cx;
    int savedCy = E.cy;
    int savedOffsetX = E.offsetX;
    int savedOffsetY = E.offsetY;
    char *query = editorPrompt("Search: %s [ESC] Cancel | [Arrows] Nav)",
                               editorFindCallback);

    if (query) {
        free(query);
    }
    else {
        E.cx = savedCx;
        E.cy = savedCy;
        E.offsetX = savedOffsetX;
        E.offsetY = savedOffsetY;
    }
}

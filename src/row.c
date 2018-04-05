#include "row.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int editorRowCxToRx(erow *row, int cx)
{
    int rx = 0;

    for (int i = 0; i < cx; i++) {
        if (row->chars[i] == '\t') {
            rx += (CHII_TAB_STOP - 1) - (rx % CHII_TAB_STOP);
        }
        rx++;
    }

    return rx;
}

int editorRowRxToCx(erow *row, int rx)
{
    int cur_rx = 0;
    int i;

    for (i = 0; i < row->size; i++) {
        if (row->chars[i] == '\t') {
            cur_rx += (CHII_TAB_STOP - 1) - (cur_rx % CHII_TAB_STOP);
        }
        cur_rx++;

        if (cur_rx > rx) {
            return i;
        }
    }

    return i;
}

void editorUpdateRow(erow *row)
{
    int tabs = 0;
    int j = 0;

    for (int i = 0; i < row->size; i++) {
        if (row->chars[i] == '\t') {
            tabs++;
        }
    }
    free(row->render);
    /* Max chars needed per tab is 8 and row->size counts as one tab  */
    row->render = malloc(row->size + tabs * (CHII_TAB_STOP - 1) + 1);
    if (row->render == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < row->size; i++) {
        if (row->chars[i] == '\t') {
            row->render[j++] = ' ';
            while (j % CHII_TAB_STOP != 0) {
                row->render[j++] = ' ';
            }
        }
        else {
            row->render[j++] = row->chars[i];
        }
    }
    row->render[j] = '\0';
    row->rsize = j;
}

void editorInsertRow(int at, char *s, size_t len)
{
    if (at < 0 || at > E.numRows) { return; }

    E.row = realloc(E.row, sizeof(erow) * (E.numRows + 1));
    if (E.row == NULL) {
        perror("realloc()");
        exit(EXIT_FAILURE);
    }
    memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numRows - at));

    E.row[at].size = len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len);
    E.row[at].chars[len] = '\0';

    E.row[at].rsize = 0;
    E.row[at].render = NULL;

    editorUpdateRow(&E.row[at]);
    E.numRows++;
    E.dirty++;
}

void editorFreeRow(erow *row)
{
    free(row->render);
    free(row->chars);
}

void editorDelRow(int at)
{
    if (at < 0 || at >= E.numRows) { return; }
    editorFreeRow(&E.row[at]);
    memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numRows - at - 1));
    E.numRows--;
    E.dirty++;
}

void editorRowInsertChar(erow *row, int at, int c)
{
    if (at < 0 || at > row->size) {
        at = row->size;
    }
    row->chars = realloc(row->chars, row->size + 2);
    if (row->chars == NULL) {
        perror("realloc()");
        exit(EXIT_FAILURE);
    }
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
    row->size++;
    row->chars[at] = c;
    editorUpdateRow(row);
    E.dirty++;
}

void editorRowAppendString(erow *row, char *s, size_t len)
{
    row->chars = realloc(row->chars, row->size + len + 1);
    if (row->chars == NULL) {
        perror("realloc()");
        exit(EXIT_FAILURE);
    }
    memcpy(&row->chars[row->size], s, len);
    row->size += len;
    row->chars[row->size] = '\0';
    editorUpdateRow(row);
    E.dirty++;
}

void editorRowDelChar(erow *row, int at)
{
    if (at < 0 || at > row->size) { return; }
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    editorUpdateRow(row);
    E.dirty++;
}

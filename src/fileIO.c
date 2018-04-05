#include "fileIO.h"
#include "global.h"
#include "row.h"
#include "input.h"
#include "output.h"

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *editorRowsToString(int *buflen)
{
    char *buf;
    char *p;
    int totalLen = 0;

    for (int i = 0; i < E.numRows; i++) {
        totalLen += E.row[i].size + 1;
    }
    *buflen = totalLen;
    buf = malloc(totalLen);
    p = buf;
    for (int i = 0; i < E.numRows; i++) {
        memcpy(p, E.row[i].chars, E.row[i].size);
        p += E.row[i].size;
        *p = '\n';
        p++;
    }

    return buf;
}

void editorOpen(char *fileName)
{
    char *line = NULL;
    size_t lineCap = 0;
    ssize_t lineLen;
    FILE *fp;

    free(E.fileName);
    E.fileName = strdup(fileName);

    fp = fopen(fileName, "r");
    if (!fp) {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }
    while ((lineLen = getline(&line, &lineCap, fp)) != -1) {
        /* Strip newline and carriage returns before copy */
        while (lineLen > 0 && (line[lineLen - 1] == '\n' ||
                               line[lineLen - 1] == '\r')) {
            lineLen--;
        }
        editorInsertRow(E.numRows, line, lineLen);
    }
    free(line);
    fclose(fp);
    E.dirty = 0;
}

void editorSave(void)
{
    int len;
    int fd;
    char *buf;

    if (E.fileName == NULL) {
        E.fileName = editorPrompt("Save as: %s [ESC] Cancel)", NULL);
        if (E.fileName == NULL) {
            editorSetStatusMessage("Save aborted");
            return;
        }
    }
    buf = editorRowsToString(&len);
    fd = open(E.fileName, O_RDWR | O_CREAT, 0644);
    if (fd != -1) {
        if (ftruncate(fd, len) != -1) {
            if (write(fd, buf, len) == len) {
                close(fd);
                free(buf);
                E.dirty = 0;
                editorSetStatusMessage("%d bytes written", len);
                return;
            }
        }
        close(fd);
    }
    free(buf);
    editorSetStatusMessage("I/O Error: Cannot save. Error %s", strerror(errno));
}

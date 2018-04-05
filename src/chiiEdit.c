#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include "term.h"
#include "memBuffer.h"
#include "fileIO.h"
#include "find.h"
#include "editor.h"
#include "input.h"
#include "output.h"
#include "row.h"
#include "version.h"
#include "commandLine.h"
#include "global.h"

#include <stdio.h>
#include <stdlib.h>

static void initEditor(void)
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

int main(int argc, char *argv[])
{
    globalVars_t *glob = processCommandLineArgs(argc, argv);

    enableRawMode();
    initEditor();
    if (argc >= 2) {
        editorOpen(argv[1]);
    }
    editorSetStatusMessage("[Ctrl-S] Save | [Ctrl-F] Find | [Ctrl-Q] Quit");

    /* Refresh per key press until exit */
    for (;;) {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    return 0;
}

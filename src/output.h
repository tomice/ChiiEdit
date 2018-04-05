#ifndef OUTPUT_H
#define OUTPUT_H

#include "memBuffer.h"

void editorScroll(void);
void editorDrawRows(abuf *ab);
void editorDrawStatusBar(abuf *ab);
void editorDrawMessageBar(abuf *ab);
void editorRefreshScreen(void);
void editorSetStatusMessage(const char *fmt, ...);

#endif /* OUTPUT_H */

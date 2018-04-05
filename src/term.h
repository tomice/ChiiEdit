#ifndef TERM_H
#define TERM_H

void disableRawMode(void);
void enableRawMode(void);
int editorReadKey(void);
int getCursorPosition(int *rows, int *cols);
int getWindowSize(int *rows, int *cols);

#endif /* TERM_H */

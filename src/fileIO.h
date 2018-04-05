#ifndef FILE_IO_H
#define FILE_IO_H

char *editorRowsToString(int *buflen);
void editorOpen(char *fileName);
void editorSave(void);

#endif /* FILE_IO_H */

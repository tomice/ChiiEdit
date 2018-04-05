#ifndef MEM_BUFFER_H
#define MEM_BUFFER_H

#define ABUF_INIT {NULL, 0}

typedef struct abuf {
    char *b;
    int len;
} abuf;

void abAppend(abuf *ab, const char *s, int len);
void abFree(abuf *ab);

#endif /* MEM_BUFFER_H */

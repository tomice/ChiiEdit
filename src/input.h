#ifndef INPUT_H
#define INPUT_H

char *editorPrompt(char *prompt, void (*callback)(char *, int));
void editorMoveCursor(int key);
void editorProcessKeypress(void);

#endif /* INPUT_H */

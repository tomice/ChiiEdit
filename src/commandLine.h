#ifndef COMMAND_LINE_H_INCLUDED
#define COMMAND_LINE_H_INCLUDED

#include <getopt.h>

typedef struct {
  char *appName;
  struct option *cmdLineOpts;
} globalVars_t;

extern globalVars_t *processCommandLineArgs (int argc, char *argv[]);
extern globalVars_t *getGlobalVars(void);
extern void printUsage(void);

#endif /* COMMAND_LINE_H_INCLUDED */

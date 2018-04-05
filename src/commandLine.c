#include "version.h"
#include "commandLine.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static struct option cmdLineOpts[] = {
    {"help", no_argument, 0, 'h'},
    SW_VERSION_OPTIONS, /* As defined in version.h */
    {0, 0, 0, 0}
};

static globalVars_t _defaultGlob = {
    .appName = 0,
    .cmdLineOpts = cmdLineOpts
};

static globalVars_t *_glob = 0;

void printUsage(void)
{
    globalVars_t *glob = getGlobalVars();
    
    printf("OPTIONS:\n\
        -h|--help               : print help (this text)\n\
        -r|--build-report       : print the build report\n\
        -v|--version            : print the version\n");
    printf("USAGE:\n\
        %s /path/to/foo.txt\n\
        Or simply %s\n", glob->appName, glob->appName);
}

void printHelp(void)
{
    globalVars_t *glob = getGlobalVars();
    
    printf("ABOUT:\n\
        \n\
        %s is a simple text editor that can open files,\n\
        edit files, save files, create files, and search text.\n", 
        glob->appName);
}

globalVars_t *getGlobalVars(void)
{
    if (!_glob) {
        _glob = &_defaultGlob;
    }

    return _glob;
}

globalVars_t *processCommandLineArgs(int argc, char *argv[])
{
    int opt = 0;
    int longIdx = 0;
    globalVars_t *glob = 0;

    glob = getGlobalVars();

    glob->appName = argv[0];
    while ((opt = getopt_long(argc, argv, "h" "vr",
           glob->cmdLineOpts, &longIdx)) != -1) {
        switch (opt) {
            case 'h':
                printHelp();
                printUsage();
                exit(EXIT_SUCCESS);
                break;
                SW_VERSION_OPTION_HANDLERS;
            default:
                fprintf(stderr, "ERROR: no arguments\n");
                printUsage();
                exit(EXIT_FAILURE);
        }
    }

    return glob;
}

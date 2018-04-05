#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED

extern char *getVersionReleaseString(void);
extern void printBuildReport(void);

/* Preprocessor macros in getopt_long struct for ver and build report info */
#define SW_VERSION_OPTIONS \
  { "version",      no_argument, 0, 'v' }, \
  { "build-report", no_argument, 0, 'r' }

#define SW_VERSION_OPTION_HANDLERS \
  case 'v': \
    printf("%s: %s\n", argv[0], getVersionReleaseString()); \
    exit(0); \
    break; \
  case 'r': \
    printBuildReport(); \
    exit(0); \
    break

extern const char *getVersion (void);
extern const char *getGitCommit(void);
extern const char *getGitUrl(void);
extern const char *getGitBranch(void);
extern const char *getBuildTag(void);

#endif /* VERSION_H_INCLUDED */

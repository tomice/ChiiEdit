/*
 * Info to be embedded in the binary. Can be viewed a plethora of ways.
 * strings works, as does querying the command line
 */
#include "version.h"
#include "versionConfig.h"
#include <stdio.h>

/* Macros for string subst */
#define xstr(x) str(x)
#define str(x) #x

/* Processes the version number from configure.ac */
#ifndef SW_VERSION
#error SW_VERSION preprocessor variable not defined.
#endif
static const char version[] = xstr (SW_VERSION);
const char *getVersion(void) { return version; }

/* Grabs the commit this was built using */
#ifndef GIT_COMMIT
#error GIT_COMMIT preprocessor variable not defined.
#endif
static const char gitCommit[] = xstr (GIT_COMMIT);
const char *getGitCommit(void) { return gitCommit; }

/* Contains the same info as git remote -v */
#ifndef GIT_URL
#error GIT_URL preprocessor variable not defined.
#endif
static const char gitUrl[] = xstr (GIT_URL);
const char *getGitUrl(void) { return gitUrl; }

/* Lists the branch you were on during the build */
#ifndef GIT_BRANCH
#error GIT_BRANCH preprocessor variable not defined.
#endif
static const char gitBranch[] = xstr (GIT_BRANCH);
const char *getGitBranch(void) { return gitBranch; }

/* Contains a Jenkins git tag or username-date */
#ifndef BUILD_TAG
#error BUILD_TAG preprocessor variable not defined.
#endif
static const char buildTag[] = xstr (BUILD_TAG);
const char *getBuildTag(void) { return buildTag; }

void printBuildReport(void)
{
    printf ("Build Report:\n");
    printf ("\tBuild_Date:\t%s\n", __DATE__);
    printf ("\tBuild_Time:\t%s\n", __TIME__);
    printf ("\tSW_VERSION:\t%s\n", version);
    printf ("\tGIT_COMMIT:\t%s\n", gitCommit);
    printf ("\tGIT_URL:\t%s\n", gitUrl);
    printf ("\tGIT_BRANCH:\t%s\n", gitBranch);
    printf ("\tBUILD_TAG:\t%s\n", buildTag);
}

char *getVersionReleaseString(void) { return xstr (SW_VERSION); }

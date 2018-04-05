AC_PREREQ([2.69])

AC_INIT([ChiiEdit], [0.0.1], [contact@thomasice.com])
AC_CONFIG_SRCDIR([src/version.c])
AM_INIT_AUTOMAKE([foreign parallel-tests color-tests -Wall -Werror])

dnl -- For custom m4 macros
AC_CONFIG_MACRO_DIR([m4])

dnl -- Check for programs
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_CC_STDC

dnl -- Initialize libtool
LT_INIT

dnl -- additonal recursive targets
AM_EXTRA_RECURSIVE_TARGETS([check-valgrind])

dnl -- Check header files
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

dnl -- Check typedefs, structs, and compiler characteristsics
AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_HEADER_TIME

dnl -- Check library functions
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([strerror strstr memcpy])

dnl - Set by jenkins or default to regular git
AC_SUBST(GIT_COMMIT, $GIT_COMMIT)
AC_SUBST(GIT_URL, $GIT_URL)
AC_SUBST(GIT_BRANCH, $GIT_BRANCH)
AC_SUBST(BUILD_TAG, $BUILD_TAG)

AC_ARG_WITH(git_commit,
    [AS_HELP_STRING([--with-git_commit=GIT_COMMIT],
                    [git commit hash])],
    GIT_COMMIT=$withval, GIT_COMMIT=${GIT_COMMIT:-$(git rev-parse HEAD)})

AC_ARG_WITH(git_url,
    [AS_HELP_STRING([--with-git_url=GIT_URL],
                    [The URL of the git repo])],
    GIT_URL=$withval, GIT_URL=${GIT_URL:-$(git config --get remote.origin.url)})

AC_ARG_WITH(git_branch,
    [AS_HELP_STRING([--with-git_branch=GIT_BRANCH],
                    [The git branchname where this is built])],
    GIT_BRANCH=$withval, GIT_BRANCH=${GIT_BRANCH:-$(git rev-parse --abbrev-ref HEAD)})

AC_ARG_WITH(build_tag,
    [AS_HELP_STRING([--with-build_tag=BUILD_TAG],
                    [The tagname set by jenkins or username-date if absent])],
    BUILD_TAG=$withval, BUILD_TAG=${BUILD_TAG:-${USER}-$(date +%Y%m%d%H%M)})

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/versionConfig.h
])
AC_OUTPUT


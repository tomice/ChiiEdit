#!/bin/bash
# Author: Tom Ice
# Date: 04/03/2018
#
# Purpose:
#
# Convenience script to make ChiiEdit
#
################################################################################
# GLOBAL VARIABLES:

TOP=$(cd "$(dirname "$0")" || exit ; pwd -P)
CONFIG_PATH="/home/${USER}/usr/local"
INSTALL_PATH="${CONFIG_PATH}/bin"
PROGRAM="ChiiEdit"
VERBOSE=0
DRY_RUN=0
VERIFIED=0
VERSION=$(grep AC_INIT "${TOP}"/configure.ac | cut -d\[ -f3 | head -c-4)
AUTHOR=$(grep CHII_AUTHOR "${TOP}"/src/global.h | cut -d\" -f2)
GIT_COMMIT=$(git rev-parse HEAD)
GIT_URL=$(git config --get remote.origin.url)
GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
DATE=$(date +%Y%m%d%H%M)
BUILD_TAG="${USER}-${DATE}"

# Redirect stdout and stderr to a log file while also displaying all info
exec &> >(tee -ia "${TOP}"/logs/"${PROGRAM}"_"${DATE}".log)
LOG_FILE="${TOP}/logs/${PROGRAM}_${DATE}.log"

################################################################################
# LOCAL FUNCTION: usage()
# PURPOSE: print out usage to stdout describing
#          how to run this program.

usage() {
cat - <<EOF | less
NAME
        ${PROGRAM} - A basic text editor that doesn't rely on any libraries

SYNOPSYS
        ${PROGRAM} [OPTIONS]

DESCRIPTION
        This script allows you to install in a normal autotools way
        Running this without options performs a simple install locally i.e.
        autoreconf -iv ; ./configure --prefix=~/usr/local ; make ; make install

OPTIONS
        -v|--verbose
             Show additional information during a build
        -h|--help
             Display this help information and exit
        -d|--dry-run
             Print the commands that would be executed

AUTHOR
        Written by ${AUTHOR}
EOF
}

################################################################################
# LOCAL FUNCTION: clean_up()
# PURPOSE: remove all temporary files

clean_up () {
  echo_verbose "Cleaning out any unnecessary build artifacts..."
  cd "${TOP}" || exit

  [[ -d "${TOP}"/autom4te.cache ]] && make maintainer-clean
  [[ -d "${TOP}"/m4 ]] && rm -rf "${TOP}"/m4
  [[ -f "${TOP}"/aminclude_static.am ]] && rm "${TOP}"/aminclude_static.am
  [[ -f "${TOP}"/src/"${PROGRAM}" ]] && rm "${TOP}"/src/"${PROGRAM}"
}

################################################################################
# LOCAL FUNCTION: echo_verbose()
# PURPOSE: Allows for printing additional information

echo_verbose () {
    [[ "${VERBOSE}" != 0 ]] && echo "${1}"
}

################################################################################
# LOCAL FUNCTION: cmd()
# PURPOSE: If DRY_RUN is 1, the commands will simply be displayed on the screen
#          This is useful for debugging purposes

cmd() {
  if [[ "${DRY_RUN}" == 1 ]]; then
    echo "${1}"
    return "${?}"
  else
    eval "${1}"
    return "${?}"
  fi
}

################################################################################
# LOCAL FUNCTION: verify_build()
# PURPOSE: Verify that the build completed successfully

verify_build () {
  echo_verbose "Verifying Build..."
  if [[ "${DRY_RUN}" == 0 ]]; then
    if [[ -e "${INSTALL_PATH}"/"${PROGRAM}" ]]; then
      echo_verbose "${PROGRAM} installed successfully!"
      VERIFIED=1
    else
      echo_verbose "FAILED"
      VERIFIED=0
    fi
  fi
  return "${VERIFIED}"
}

################################################################################
# MAIN PROGRAM: SETUP

# Parse the command line arguments using GNU getopt:
if ! OPTIONS=$(getopt -o hvd -l help,verbose,dry-run -- "${@}"); then
  echo "ERROR: GNU getopt error"
  exit 1
fi

eval set -- "${OPTIONS}"

# Never process more than one flag
[[ "${#}" -gt 2 ]] && { echo "ERROR: Too many parameters"; exit 1; }

while [[ "${#}" -gt 0 ]]; do
  case "${1}" in
    -h|--help) usage; exit 0;;
    -v|--verbose) VERBOSE=1; shift;;
    -d|--dry-run) DRY_RUN=1; shift;;
    --) shift; break;;
    -*) echo "ERROR: Invalid option: ${1}" 1>&2; usage; exit 1;;
     *) break;;
  esac
done

# Don't build if there's no configure.ac
if [[ ! -f "${TOP}"/configure.ac ]]; then
  echo "ERROR: ${TOP}/configure.ac does not exist"
  clean_up
  exit 1
fi

cmd "clean_up"

################################################################################
# MAIN PROGRAM

cd "$TOP" || exit

echo_verbose "=============================================================="
echo_verbose "BUILD LOG"
echo_verbose
echo_verbose "PROJECT:    ${PROGRAM}"
echo_verbose "VERSION:    ${VERSION}"
echo_verbose "GIT_COMMIT: ${GIT_COMMIT}"
echo_verbose "GIT_URL:    ${GIT_URL}"
echo_verbose "GIT_BRANCH: ${GIT_BRANCH}"
echo_verbose "BUILD_TAG:  ${BUILD_TAG}"
echo_verbose "LOG_FILE:   ${LOG_FILE}"
echo_verbose "USER:       ${USER}"
echo_verbose "DATE:       $(date)"
echo_verbose "=============================================================="

echo_verbose "Performing autoreconf on project..."
cmd "autoreconf --install --verbose"
echo_verbose "Configuring for ~/usr/local..."
cmd "./configure --prefix=${CONFIG_PATH}"
echo_verbose "Making target..."
cmd "make"
echo_verbose "Installing..."
cmd "make install"

################################################################################
# VERIFY

verify_build
if [[ "${VERIFIED}" == 1 || "${DRY_RUN}" == 1 ]]; then
  echo "${PROGRAM} can be found here: ${INSTALL_PATH}"
  echo "BUILD SUCCESS"
else
  echo "BUILD FAILED"
  clean_up
  exit 1
fi

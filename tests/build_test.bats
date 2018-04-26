#!/usr/bin/env bats
# Author: Tom Ice
# Date: 04/25/2018
# 
# Purpose: Test suite for the build.sh file using bats
#          For more programming information regarding bats, see this link:
#          https://github.com/bats-core/bats-core        
################################################################################

@test "Verify autoreconf is installed" {
  command -v autoreconf
}

@test "Verify make is installed" {
  command -v make
}

@test "Verify grep is installed" {
  command -v grep
}

@test "Verify cut is installed" {
  command -v cut
}

@test "Verify less is installed" {
  command -v less
}

@test "Verify head is installed" {
  command -v head
}

@test "Verify cut is installed" {
  command -v getopt
}

@test "Verify git is installed" {
  command -v git
}

@test "Verify date is installed" {
  command -v date
}

@test "Verify cat is installed" {
  command -v cat
}

@test "Verify tee is installed" {
  command -v tee
}

@test "-h switch" {
  run "${BATS_TEST_DIRNAME}"/../build.sh -h
  [[ "${status}" == 0 ]]
  [[ "${output}" =~ "Display this help information and exit" ]]
}

@test "--help switch" {
  run "${BATS_TEST_DIRNAME}"/../build.sh --help
  [[ "${status}" == 0 ]]
  [[ "${output}" =~ "Display this help information and exit" ]]
}

@test "-d switch" {
  run "${BATS_TEST_DIRNAME}"/../build.sh -d
  [[ "${status}" == 0 ]]
  [[ "${output}" =~ "BUILD SUCCESS" ]]
}

@test "--dry-run switch" {
  run "${BATS_TEST_DIRNAME}"/../build.sh --dry-run
  [[ "${status}" == 0 ]]
  [[ "${output}" =~ "BUILD SUCCESS" ]]
}

@test "-v switch" {
  skip
  run "${BATS_TEST_DIRNAME}"/../build.sh -v
  [[ "${status}" == 0 ]]
  [[ "${output}" =~ "BUILD LOG" ]]
}

@test "--verbose switch" {
  skip
  run "${BATS_TEST_DIRNAME}"/../build.sh --verbose
  [[ "${status}" == 0 ]]
  [[ "${output}" =~ "BUILD LOG" ]]
}

@test "Invalid switch" {
  run "${BATS_TEST_DIRNAME}"/../build.sh -x
  [[ "${status}" != 0 ]]
  [[ "${output}" =~ "getopt: invalid option" ]]
}

@test "Too many parameters" {
  run "${BATS_TEST_DIRNAME}"/../build.sh -d -v
  [[ "${status}" != 0 ]]
  [[ "${output}" =~ "ERROR: Too many parameters" ]]
}

#!/bin/bash -xe
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
TEST_DIR="$( dirname "${DIR}" )/test"
cd "$( dirname "${DIR}" )"

# update git submodules
if [ ! -d "$(dirname $DIR)/test/html5lib-tests" ]; then
  echo "error html5lib-tests submodule node found"
  echo "try running this command: git submodule update --init"
  exit 1
fi

OUT_DIR="../out"

if [ -d "${OUT_DIR}/coverage" ]; then
  rm -rf "${OUT_DIR}/coverage"
fi

mkdir -p "${OUT_DIR}/coverage"
LLVM_PROFILE_FILE="${OUT_DIR}/coverage/coverage-%p.profraw" \
  ib --cfg test --test_all test $@

FILE_LIST=(../out/coverage/*.profraw)
TARGET_LIST=(../out/test/test/*-test)

llvm-profdata merge -sparse "${FILE_LIST[@]}" -o "${OUT_DIR}/coverage/all.profdata"
llvm-cov show "${TARGET_LIST[@]}" -instr-profile="${OUT_DIR}/coverage/all.profdata" --ignore-filename-regex='yourhtml_entities/*'
llvm-cov report "${TARGET_LIST[@]}" -instr-profile="${OUT_DIR}/coverage/all.profdata" --ignore-filename-regex='yourhtml_entities/*'
llvm-cov export "${TARGET_LIST[@]}" -instr-profile="${OUT_DIR}/coverage/all.profdata" --ignore-filename-regex='yourhtml_entities/*' > "${OUT_DIR}/coverage/report.json"
echo ''
echo 'coverage report written to ../out/coverage/report.json'
echo ''

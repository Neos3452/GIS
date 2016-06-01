#!/usr/bin/env bash

set -ueo pipefail

OPTIND=1

CURRENT_DIR=`pwd`
BUILD_DIR="$CURRENT_DIR/../build"
BIN_NAME="find_strongly_connected_components"
SRC_DIR="$CURRENT_DIR/.."
BOOST_PATH="/home/przemek/Development/boost_1_60_0/"

function build() {
    cd $BUILD_DIR
    make
}

function clean() {
    cd $BUILD_DIR
    make clean
}

function clear() {
    if [ -d "$BUILD_DIR" ]; then
        printf '%s\n' "Removing directory ($BUILD_DIR)"
        rm -rf $BUILD_DIR
    fi

    CMAKE_ADDITIONAL_OPTIONS="-DBOOST_ROOT=$BOOST_PATH"
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    cmake $SRC_DIR $CMAKE_ADDITIONAL_OPTIONS
}

function test() {
    cd $BUILD_DIR
    ctest --output-on-failure
}

function benchmark() {
    local BENCHMARK_DIR=$CURRENT_DIR"/benchmark_"$(date +"%Y-%m-%d_%H-%M-%S")
    local BENCHMARK_PARTIAL_DIR=$BENCHMARK_DIR"/partials"
    local BENCHMARK_RESULT_TXT_FILE=$BENCHMARK_DIR"/result.txt"
    local BENCHMARK_RESULT_CSV_FILE=$BENCHMARK_DIR"/result.csv"

    mkdir -p $BENCHMARK_PARTIAL_DIR
    touch $BENCHMARK_RESULT_TXT_FILE
    touch $BENCHMARK_RESULT_CSV_FILE
    echo "time,mem,edges" >> $BENCHMARK_RESULT_CSV_FILE

    for i in {10000..10000..100}
    do
        echo "Test with $i nodes"
        $BUILD_DIR"/"$BIN_NAME --autoTest -n $i -t 10 -p 0.35 --resultFile $BENCHMARK_PARTIAL_DIR"/result_$i.csv" >> $BENCHMARK_RESULT_TXT_FILE
    done

    sed 's/\ /,/g' $BENCHMARK_RESULT_TXT_FILE >> $BENCHMARK_RESULT_CSV_FILE
    echo "Benchmarking finished"
    echo "Result directory: $BENCHMARK_DIR"
}

while getopts ":cbxhtm" opt; do
    case "$opt" in
    c) clean
        ;;
    b) build
        ;;
    x) clear
        ;;
    t) test
        ;;
    m) benchmark
        ;;
    h)  echo "GIS HELP TOOL message"
        echo "-c clean"
        echo "-b build"
        echo "-x clear"
        echo "-t run unit tests"
        echo "-b run benchmark"
        ;;
    \?) echo "unknown option"
        ;;
    esac
done

shift $((OPTIND-1))
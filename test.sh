#!/bin/bash

EXECUTABLE="./build/bin/warmup2"
if [ ! -f $EXECUTABLE ]; then
    echo "EXECUTABLE \"$EXECUTABLE\" CANNOT BE FOUND"
    exit 1
fi

OUTPUT=$(cat test.txt | $EXECUTABLE 2> /dev/null)
echo "$OUTPUT" | diff -s test_key.txt -

#!/bin/bash

OUTPUT=$(cat test.txt | ./build/bin/warmup2 2> /dev/null)
echo $OUTPUT | diff -s test_key.txt -

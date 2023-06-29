#!/bin/bash
g++ -std=c++17 -Wall -Wextra -O2 "imppuzzle.cpp" -lm
ERROR="impossible"
while true;
do
    pypy3 puzzle_generator.py
    RESPONSE=`./a.out<test`
    RESP="$RESPONSE"
    if [[ "$RESPONSE" =~ "$ERROR" ]];
    then
        break
    fi

    if [[ "$RESPONSE" == "" ]];
    then
        break
    fi
    echo "has solution"

    #echo $RESPONSE
done
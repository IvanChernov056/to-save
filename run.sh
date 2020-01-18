#!/bin/bash
if [ -d ./bin ]; then 
    if [ -n "$1" ]; then
        ./bin/ESN $1 2>err.log
    else 
        echo "You have to set data file"
    fi
else 
    echo "You have to build first"
fi
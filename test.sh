#!/bin/bash

if [ -d ./TEST ]; then
    rm -r TEST;
fi

mkdir TEST;
cd TEST
if [ -n $1 ]; then
    ../bin/ESN $1 2>err.log
else 
    echo "You have to set data file"
fi

cd ../


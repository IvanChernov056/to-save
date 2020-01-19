#!/bin/bash

if [ -d ./BUILD ]; then
    rm -r BUILD;
fi

./build.sh

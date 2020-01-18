#!/bin/bash 

if ! [ -d ./BUILD ]; then
mkdir BUILD
fi

cd ./BUILD
cmake ../
make
cd ../
#!/bin/ksh
if [ $# -eq 0 ];
then 
    A=10000
else
    A=$1
fi
let mod=$RANDOM%A
echo $mod

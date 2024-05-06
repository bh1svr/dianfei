#!/bin/sh
if [ $# -eq 0 ];
then 
    A=10000
else
    A=$1
fi
echo $(($RANDOM%$A))
